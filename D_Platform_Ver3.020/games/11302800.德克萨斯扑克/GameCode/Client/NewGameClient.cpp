#include "stdafx.h"
#include "NewGameClient.h"
#include "IGameFrame.h"
#include "Common/UIMessageDefines.h"
#include "DKSSPK_ControlsDefine.h"

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
    return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
    delete *ppImp;
    *ppImp = NULL;
}


CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
    m_pGameFrame = pGameFrame;
    m_pUI = pUI;

	// ÿ�������ע���
	memset(m_nBetMoney, 0, sizeof(m_nBetMoney));


	m_pSoundEngine = createIrrKlangDevice();
	m_pSEBkMusic = createIrrKlangDevice();
	m_pBetSMusic	= createIrrKlangDevice();
	m_pTimerSMusic = createIrrKlangDevice();

	m_bSoundPlay = true; 
	m_bAllowWatch = false ; 
	m_bIsOffline = false;

	m_iUserStation =  -1 ;


	CString filepath = CINIFile::GetAppPath() ; 
	CString strfileName ;
	strfileName.Format("%d.bcf",NAME_ID) ; 
	CBcfFile file(filepath + strfileName) ;
	m_bSoundPlay = file.GetKeyVal("config","PlaySound",m_bSoundPlay); 
	m_bAllowWatch = file.GetKeyVal("config","AllowWatch",m_bAllowWatch);

	UserSetGameInfo(m_bSoundPlay,m_bAllowWatch,false);
}

CNewGameClient::~CNewGameClient()
{
	if(m_pSEBkMusic)
	{
		m_pSEBkMusic->drop();
	}

	if (m_pSoundEngine)
	{
		m_pSoundEngine->drop();
	}

	if (m_pBetSMusic)
	{
		m_pBetSMusic->drop();
	}

	if (m_pTimerSMusic)
	{
		m_pTimerSMusic->drop();
	}
}

/// ��ʼ��
int CNewGameClient::Initial()
{
    // ����UI
    m_pUI->LoadData(L"DKSSPK_UIGameComm.dat");

    return 0;
}
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	m_byMeStation = 255;	// �Լ���λ��
	m_iUser = 0;			// ���λ��
	m_iMinMoney = 0;		// ��С������
	m_iMaxMoney = 0;		// ��������
	m_iCurrentMoney = 0;	// ��ǰҪ����Ľ��
	m_iSelfMoney = 0;		// �Լ�����ӵ�еĽ��
	m_iMinusMoney = 0;		// �޸�ע��Ӽ����
	m_nTypeCard = UNKNOWN;	// δ֪����

	memset(m_bpUserInfo, 255,sizeof(m_bpUserInfo));

	//�õ����Ƶ����
	m_byTokenUser = 255;

	//�Ƿ���֪ͨ���������׼����Ϸ
	m_bAgreeGame = false;

	// ��¼ÿ����Ҵ�����
	memset(m_iMoneys, 0, sizeof(m_iMoneys));

	// ��ʼ������UI
	OnInitYuanSu(false);

    return 0;
}
// ȫ��ʼ��
void CNewGameClient::OnInitData()
{
	OnShowDeskTop();	
	m_bIsOffline = false;
	m_bTransceiving = false;
	m_bNtDaXiao = false;
	m_nCount = 0;

	m_bAutoStartTimer = false;

	// ��¼��ע���
	m_iBetCounts = 0;

	// ��ʼ���ۼӽ��
	m_nCurrentMoney = 0;

	// ��ע���
	m_nCallMoney = 0;

	//ׯ��λ��
	m_byNTUser = 255;

	//Сäעλ��
	m_bySmallBlind = 255;

	//��äעλ��
	m_byBigBlind = 255;

	//ÿ���û����ý��
	memset(m_nMoney, 0, sizeof(m_nMoney));

	// ȫ�����
	memset(m_bAllBet, 0, sizeof(m_bAllBet));

	// �������
	memset(m_bGiveUp, 0, sizeof(m_bGiveUp));

	// �йܱ�־
	memset(m_bTrusteeship, 0, sizeof(m_bTrusteeship));

	// ��һ�β�������
	memset(&m_emLastOperation, ET_UNKNOWN,sizeof(m_emLastOperation));

	//������
	m_iCardCounts = 0;

	//��������
	memset(m_iUserDataCard, 0, sizeof(m_iUserDataCard));

	//������3
	m_iCardDataCounts = 0;

	// ���������ʾ
	memset(m_iCardData, 0, sizeof(m_iCardData));

	// ÿ�������ע���
	memset(m_nBetMoney, 0, sizeof(m_nBetMoney));

	// ���ջ
	ClearAllStack();

	//��ʼ����ʾ����
	::memset(m_byShowed,0,MAX_HANDCARDS_NUM);
}
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch (nSubId)
	{
	case ASS_GM_GAME_STATION:   // ����״̬
		{		
			// ������Ϸ״̬,��Ҫ�Ƕ����������������Ϣ
			SetGameStation(buffer,nLen);
			break;
		}
	case ASS_GAME_BEGIN:
		{
			//��Ϸ��ʼ
			OnHandleGameBegin(buffer,nLen);
		}
		break;

	case ASS_SYS_DESK_CFG:
		{
			//�յ�ϵͳ�������ӵ���Ϣ����������
			OnHandleGetSysDeskCfg(buffer,nLen);
		}

		break;
	case ASS_CFG_DESK: 
		{
			//���õ�ע
			OnHandleCfgDesk(buffer,nLen);
		}
		break;
	case ASS_CFG_DESK_TIMEOUT:
		{
			//�������õ�ע��ʱ            
			OnHandleWaitCfgDeskTimeout();
		}
		break;
	case ASS_DESK_CFG: 
		{
			//�յ�ע����Ϣ����������
			OnHandleGetDeskCfg(buffer,nLen);
		}
		break;
	case ASS_SHOW_IN_MONEY: 
		{
			//�յ���������Ϣ����������
			OnHandleInMoney(buffer,nLen);
		}
		break;
	case ASS_SEND_A_CARD:
		{
			//�յ���������һ���ư���������
			OnHandleSendACards(buffer,nLen);
		}
		break;
	case ASS_BETPOOL_UP:
		{
			//�յ��������߳ظ��°���������
			OnHandleBetPoolUp(buffer,nLen);
		}
		break;
	case ASS_SEND_3_CARD:
		{
			//�յ���������һ���ư���������
			OnHandleSend3Cards(buffer,nLen);
		}
		break;
	case ASS_SEND_4_5_CARD:
		{
			//�յ���������һ���ư���������
			OnHandleSend1Cards(buffer,nLen);
		}
		break;
	case ASS_SEND_CARD_FINISH:
		{
			//�յ�����������������ҷ�����ɵ���Ϣ��������
			OnHandleSendCardFinish();
		}
		break;

	case ASS_BET_INFO:
		{
			//�յ������ע����Ϣ
			OnHandleGetBetInfo(buffer,nLen);
		}
		break;

	case ASS_TOKEN:
		{
			//�յ�������Ϣ�������û�
			OnHandleGetToken(buffer,nLen);
		}
		break;
	case ASS_COMPARE_CARD:
		{
			//������Ϣ
			OnHandleCompareCard(buffer,nLen);
		}
		break;
	case ASS_RESULT:
		{
			//�յ�������Ϣ����������
			OnHandleRoundResult(buffer,nLen);
		}
		break;
	case ASS_NEW_ROUND_BEGIN:
		{
			//�յ��µ�һ�غϿ�ʼ��Ϣ����������
			OnHandleNewRoundBegin(buffer,nLen);
		}
		break;
	case ASS_NO_PLAYER:
		{
			//û����ҽ�����Ϸ���˳�����
			OnHandleNoPlayer(buffer,nLen);
		}
		break;
	case ASS_AGREE:
		{
			//����Ƿ�ͬ�����Ϸ�ģ�����
			//return m_GameMachine.OnHandleAgree(pNetData,uDataSize);
		}
		break;
	case ASS_NOT_ENOUGH_MONEY:
		{
			//������ϵ�Ǯ���㹻ʱ������
			OnHandleNotEnoughMoney(buffer,nLen);
		}
		break;
	case ASS_USER_LEFT_DESK:
		{
			OnShowUserLogo();

			UserLeftDeskStruct *pLeftData = (UserLeftDeskStruct *)buffer ; 

			if(pLeftData->bClearLogo)
			{
				BYTE byView = m_pGameFrame->DeskStation2View(pLeftData->bDeskStation);

				SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + byView,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + byView,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IText,DZPK_USER_NAME + byView,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + byView,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_USER_CHAIR + byView,SetControlVisible(true));	// ��ʾ�������

				GameUserLeft(pLeftData->bDeskStation);
			}
		}
		break ; 

	case ASS_SORT_OUT:
		{
			//ϵͳ�Զ�����Ƭ
			OnUserSortOut();
		}
		break;
#ifdef CLOSE_CARD
	case ASS_LOOK_CARD:	//��ҿ���
		{
			OnHandleMovePlayerCard(buffer,nLen);
		}
		break;
	case ASS_CAN_SHOW_CARD:	//��չʾ�� 
		{
			//չʾ�ư�ť ʹ��
			IButton *pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD_BUT));
			if(NULL != pButton)
			{
				pButton->SetEnable(true);
				pButton->SetControlVisible(true);
			}
		}
		break;
	case ASS_SHOW_CARD:
		{
			ShowCardStruct* pShowCard = (ShowCardStruct*)buffer;

			//��ȫ�ж�
			if(nLen != sizeof(ShowCardStruct) || 255 == pShowCard->byDeskStation)
			{
				return false;
			}

			//��ʾ չʾ����ҵ� ����
			INoMoveCard* pCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD+m_pGameFrame->DeskStation2View(pShowCard->byDeskStation)));
			if(NULL != pCard)
			{
				pCard->SetCardList(pShowCard->byCardList,pShowCard->iCardCount);
				pCard->SetControlVisible(true);
			}
		}
		break;
#endif
	default:
		break;
	}
    return 0;
}
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	if (pMessage->_uMessageType == UI_CATOONFINISH)
	{
		if (pMessage->_uControlID == DZPK_SHOW_IN_TIME)
		{
			// -----------------------�رն�ʱ�� -----------------------
			ITime *pTime = dynamic_cast<ITime*>(m_pUI->GetIControlbyID(DZPK_SHOW_IN_TIME));
			if (pTime != NULL)
			{
				pTime->SetPLay(false);
				pTime->SetControlVisible(false);
			}
			// -----------------------------------------------------------------

			if (m_tagDeskCfg.dz.iUserClose)
			{
				m_pGameFrame->CloseClient();
			}
			else
			{
				if (m_iMoneys[m_byMeStation] <= m_tagDeskCfg.dz.iUntenMoney)
				{
					// �Զ����������ҿ�
					OnLoadSelectionDlg(true);
					OnShowDeskTop();
					//m_pGameFrame->SetTimer(IDT_AOUT_IN, 5000);
				}
				else
				{
					if (m_iSelfMoney < m_iMinMoney)
					{
						return 0;
					}

					// �Զ�׼��(����)
					TSubMoney cmd;
					cmd.nMoney[m_byMeStation] = m_iMoneys[m_byMeStation];
					cmd.bDeskStation = m_byMeStation;
					m_pGameFrame->SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);	

					// ���ش������ͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_IN_MONEY_BU_ZU,SetControlVisible(false));

					// ���ؽ����
					SAFE_CTRL_OPERATE(IContain,DZPK_BALANCE_CONTAINER,SetControlVisible(false));

					OnShowDeskTop();

					if (m_bSoundPlay)
					{
						// ȷ����ť����
						PlayPublicSound(BT_OK);
					}
				}
			}
		}
	}

	if (pMessage->_uMessageType == UI_CATOONFINISH)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if ((pMessage->_uControlID == (DZPK_NT_MOVE_COLLECT + i)) || (pMessage->_uControlID == (DZPK_XIAO_MOVE_COLLECT + i)) || (pMessage->_uControlID == (DZPK_DA_MOVE_COLLECT + i)))
			{
				if (m_bWatch)
				{
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + i,SetControlVisible(false));
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + i,SetControlVisible(false));
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + i,SetControlVisible(false));

				}
			}
		}
 		switch (pMessage->_uControlID)
		{
		case DZPK_FA_PAI_ANIMATION:
			{
				// ��ʾV����
				IImage * pImage = NULL;
				TSendCardAnimNode* pAnimNode = &m_qSendCards.front();

				BYTE byUser = m_pGameFrame->DeskStation2View(pAnimNode->byUser);

				BYTE CardUpType[]={0,0};

				if (m_bSoundPlay)
				{
					// ��������Ч
					PlayPublicSound(BT_FLOP_BACK_CARD);
				}

				if (pAnimNode->iPos == 0)
				{
					pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(DZPK_V1_PAI + m_pGameFrame->DeskStation2View(pAnimNode->byUser)));
					if (pImage != NULL)
					{
						pImage->SetControlVisible(true);
					}
					
					if (pAnimNode->byUser == m_byMeStation)
					{
						// �����Լ�, ����Ҫ���з��ƶ���
						INoMoveCard * pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + byUser));
						if (pCard != NULL)
						{
#ifdef CLOSE_CARD
							pCard->SetCardList(CardUpType,pAnimNode->iPos + 1);
#else
							pCard->SetCardList(m_iUserDataCard,pAnimNode->iPos + 1);
#endif						
							pCard->SetUpCard(0,0,0);
							pCard->SetControlVisible(true);
						}
					}

					if (m_bWatch)
					{
						BYTE iShowCardList[52] ;
						memset(iShowCardList , 0 ,sizeof(iShowCardList)) ; 

						memcpy(iShowCardList , m_iUserDataCard , sizeof(iShowCardList)) ;

						if(!m_bAllowWatch)
						{
							iShowCardList[0] = 0x00 ; 
							iShowCardList[1] = 0x00 ; 
						}
						
						// �����Լ�, ����Ҫ���з��ƶ���
						INoMoveCard * pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + byUser));
						if (pCard != NULL)
						{
							pCard->SetCardList(iShowCardList,pAnimNode->iPos + 1);
							pCard->SetUpCard(0,0,0);
							pCard->SetControlVisible(true);
						}
					}
				}
				else if (pAnimNode->iPos == 1)
				{
					pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(DZPK_V2_PAI + m_pGameFrame->DeskStation2View(pAnimNode->byUser)));
					if (pImage != NULL )
					{
						pImage->SetControlVisible(true);
					}

					if (pAnimNode->byUser == m_byMeStation)
					{
						// �����Լ�, ����Ҫ���з��ƶ���
						INoMoveCard * pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + byUser));
						if (pCard != NULL)
						{
#ifdef CLOSE_CARD
							pCard->SetCardList(CardUpType,pAnimNode->iPos + 1);
#else
							pCard->SetCardList(m_iUserDataCard,pAnimNode->iPos + 1);
#endif	
							pCard->SetUpCard(0,0,0);
							pCard->SetControlVisible(true);
						}
					}

					if (m_bWatch)
					{
						BYTE iShowCardList[52] ;
						memset(iShowCardList , 0 ,sizeof(iShowCardList)) ; 

						if(m_bAllowWatch)
						{
							memcpy(iShowCardList , m_iUserDataCard , sizeof(iShowCardList)) ;
						}
						// �����Լ�, ����Ҫ���з��ƶ���
						INoMoveCard * pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + byUser));

						if (pCard != NULL)
						{
							pCard->SetCardList(iShowCardList,pAnimNode->iPos + 1);
							pCard->SetUpCard(0,0,0);
							pCard->SetControlVisible(true);
						}

					}
				}

				m_qSendCards.pop();

				// ��������
				OnFaPaiAnim();
			}
			break;
		case DZPK_CHIP_RECYCLE_MOVE:
		case DZPK_CHIP_RECYCLE_MOVE + 1:
		case DZPK_CHIP_RECYCLE_MOVE + 2:
		case DZPK_CHIP_RECYCLE_MOVE + 3:
		case DZPK_CHIP_RECYCLE_MOVE + 4:
		case DZPK_CHIP_RECYCLE_MOVE + 5:
		case DZPK_CHIP_RECYCLE_MOVE + 6:
		case DZPK_CHIP_RECYCLE_MOVE + 7:
		case DZPK_MONEY_ISSUE_MOVE:
		case DZPK_MONEY_ISSUE_MOVE + 1:
		case DZPK_MONEY_ISSUE_MOVE + 2:
		case DZPK_MONEY_ISSUE_MOVE + 3:
		case DZPK_MONEY_ISSUE_MOVE + 4:
		case DZPK_MONEY_ISSUE_MOVE + 5:
		case DZPK_MONEY_ISSUE_MOVE + 6:
		case DZPK_MONEY_ISSUE_MOVE + 7:
			{
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					SAFE_CTRL_OPERATE(IMoveAction,DZPK_CHIP_RECYCLE_MOVE + i,SetPlayState(false));
					SAFE_CTRL_OPERATE(IMoveAction,DZPK_MONEY_ISSUE_MOVE + i,SetPlayState(false));

					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DESKTOP_MONEY_LOGO + i,SetControlVisible(false));
					SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetControlVisible(false));

					SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_MAP + i,SetControlVisible(false));
					SAFE_CTRL_OPERATE(INum,DZPK_MONEY_NUM + i,SetControlVisible(false));
				}

				if (m_bTransceiving)
				{
					// ��ʾ������ͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_TOTAL_MAP,SetControlVisible(true));

					__int64 nBetMoney = 0 ;

					CString str;

					for (int i = 0; i < PLAY_COUNT; i++)
					{
						if (m_nBetMoney[i] > 0)
						{
							nBetMoney += m_nBetMoney[i];
						}
					}

					str.Format("Client:��ʾ�ռ������ܽ��:%I64d",nBetMoney);
					OutputDebugString(str);

					INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_MONEY_COLLECT));
					if (pNum != NULL)
					{
						pNum->SetNum(nBetMoney);
						pNum->SetRankType(0);
						pNum->SetControlVisible(nBetMoney > 0);
					}
				}
			}
			break;
#ifdef CLOSE_CARD
		case DZPK_LOOK_CARD_ANIMA_0:	//0��ͼ���ƶ���
		case DZPK_LOOK_CARD_ANIMA_1:	//1��ͼ���ƶ���
		case DZPK_LOOK_CARD_ANIMA_2:	//2��ͼ���ƶ���
		case DZPK_LOOK_CARD_ANIMA_3:	//3��ͼ���ƶ���
		case DZPK_LOOK_CARD_ANIMA_5:	//4��ͼ���ƶ���
		case DZPK_LOOK_CARD_ANIMA_6:	//5��ͼ���ƶ���
		case DZPK_LOOK_CARD_ANIMA_7:	//6��ͼ���ƶ���
			{
				BYTE byView = pMessage->_uControlID - DZPK_LOOK_CARD_ANIMA_0;

				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_CARD+byView,SetControlVisible(true));//��ʾС��
				SAFE_CTRL_OPERATE(IExAnimate,pMessage->_uControlID,SetControlVisible(false)); //���ض���
			}
			break;
#endif
		default:{break;}
		}
	}
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		if (pMessage->_uControlID == DZPK_PLAN_MONEY_INPUT_TEXT)
		{
			// ��ǰ׼����������
			IEditorText * pEdit = dynamic_cast<IEditorText *>(m_pUI->GetIControlbyID(DZPK_PLAN_MONEY_INPUT_TEXT));
			if (pEdit != NULL)
			{
				CString strSubPlanMoney;
				wchar_t *pchar = pEdit->GetText();
				
				if(NULL != pchar)
				{
					__int64 iNoteMoney = 0;
					iNoteMoney =_wtoi64(pchar);
					m_iCurrentMoney = iNoteMoney;

					// ������ݲ���Ⱦ͸���
				  /*if (m_iCurrentMoney < m_iMinMoney)
					{
						m_iCurrentMoney = m_iMinMoney;
					}
					else */if (m_iCurrentMoney > m_iMaxMoney)
					{
						m_iCurrentMoney = m_iMaxMoney;
					}

					// ��ֹ�����ҳ���������
					if (m_iCurrentMoney > m_iSelfMoney)
					{
						m_iCurrentMoney = m_iSelfMoney;
					}

					strSubPlanMoney.Format("%I64d",m_iCurrentMoney);
					pEdit->SetText(CA2W(strSubPlanMoney));
					pEdit->SetControlVisible(true);	
				}			
			}
			
			IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_TOTAL_MONEY_TEXT));
			if (pText != NULL)
			{
				// �����Լ�������
				CString strMoney;
				strMoney.Format("%I64d", m_iSelfMoney - m_iCurrentMoney);
				pText->SetText(CA2W(strMoney.GetBuffer()));
				pText->SetControlVisible(true);
			}
		}
	}
	if (pMessage->_uMessageType == UI_LBUTTONUP)
	{
		switch (pMessage->_uControlID)
		{
		case DZPK_GAME_BEGIN_BUT:	// ��Ϸ��ʼ��ť
			{
				if (m_iMoneys[m_byMeStation] <= m_tagDeskCfg.dz.iUntenMoney)
				{
					// �Զ����������ҿ�
					OnLoadSelectionDlg(true);
					OnShowDeskTop();
				}
				else
				{
					if (m_iSelfMoney < m_iMinMoney)
					{
						return 0;
					}

					// �Զ�׼��(����)
					TSubMoney cmd;
					cmd.nMoney[m_byMeStation] = m_iMoneys[m_byMeStation];
					cmd.bDeskStation = m_byMeStation;
					m_pGameFrame->SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);	

					// -----------------------�رն�ʱ�� -----------------------
					ITime *pTime = dynamic_cast<ITime*>(m_pUI->GetIControlbyID(DZPK_SHOW_IN_TIME));
					if (pTime != NULL)
					{
						pTime->SetPLay(false);
						pTime->SetControlVisible(false);
					}
					// -----------------------------------------------------------------

					// ���ش�����ͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_IN_MONEY_BU_ZU,SetControlVisible(false));

					// ���ؽ����
					SAFE_CTRL_OPERATE(IContain,DZPK_BALANCE_CONTAINER,SetControlVisible(false));

					OnShowDeskTop();

					if (m_bSoundPlay)
					{
						// ȷ����ť����
						PlayPublicSound(BT_OK);
					}	
				}
			}
			break;
		case DZPK_DAI_RU_MONEY_BUT: // �����ť
			{
				OnLoadSelectionDlg();
				OnShowDeskTop();

				ITime *pTime = dynamic_cast<ITime*>(m_pUI->GetIControlbyID(DZPK_SHOW_IN_TIME));
				if (pTime != NULL)
				{
					pTime->SetPLay(false);
					pTime->SetShowMaxNum(0);
					pTime->SetControlVisible(false);
				}
			}
			break;
		case DZPK_OK_LOAD_BUT:		// ȷ�����밴ť
			{
				OnOKLOAD();
				SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(true));
			}
			break;
		case DZPK_EXIT_GAME_BUT:	// �˳���Ϸ��ť
			{
				m_pGameFrame->CloseClient();
			}
			break;
		case DZPK_ADD_BUT:			// ����Ӱ�ť
			{
				IEditorText * pEdit = dynamic_cast<IEditorText *>(m_pUI->GetIControlbyID(DZPK_PLAN_MONEY_INPUT_TEXT));
				IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_TOTAL_MONEY_TEXT));

				if (pEdit == NULL || pText == NULL)
				{
					AfxMessageBox("IDC_PLAN_MONEY OR IDC_TOTAL_MONEY �ؼ�������");
					break;
				}

				// ��ȡ��ǰ������
				__int64 nMoney;
				wchar_t* wszMoney = pEdit->GetText();
				nMoney = _wtoi64(wszMoney);
				m_iCurrentMoney = nMoney;

				CString strMoney;

				if ((m_iCurrentMoney + m_iMinusMoney) > m_iMaxMoney)
				{
					m_iCurrentMoney = m_iMaxMoney;
				}	
				else
				{
					m_iCurrentMoney += m_iMinusMoney;
				}

				// ��ֹ�����ҳ���������
				if (m_iCurrentMoney > m_iSelfMoney)
				{
					m_iCurrentMoney = m_iSelfMoney;
				}

				// ���õ�ǰ������
				strMoney.Format("%I64d",m_iCurrentMoney);
				pEdit->SetText(CA2W(strMoney.GetBuffer()));
				pEdit->SetControlVisible(true);

				// �����Լ�������
				strMoney.Format("%I64d",m_iSelfMoney - m_iCurrentMoney);
				pText->SetText(CA2W(strMoney.GetBuffer()));
				pText->SetControlVisible(true);

				// ��ʾ����������
				SAFE_CTRL_OPERATE(IContain,DZPK_DAI_RU_MONEY_CONTAINER,SetControlVisible(true));
			}
			break;
		case DZPK_SUB_BUT:			// �������ť
			{
				IEditorText * pEdit = dynamic_cast<IEditorText *>(m_pUI->GetIControlbyID(DZPK_PLAN_MONEY_INPUT_TEXT));
				IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_TOTAL_MONEY_TEXT));

				if (pEdit == NULL || pText == NULL)
				{
					AfxMessageBox("IDC_PLAN_MONEY OR IDC_TOTAL_MONEY �ؼ�������");
					break;
				}

				// ��ȡ��ǰ������
				__int64 nMoney;
				wchar_t* wszMoney = pEdit->GetText();
				nMoney = _wtoi64(wszMoney);
				m_iCurrentMoney = nMoney;

				CString strMoney;

				if ((m_iCurrentMoney - m_iMinusMoney) < m_iMinMoney)
				{
					m_iCurrentMoney = m_iMinMoney;
				}	
				else
				{
					m_iCurrentMoney -= m_iMinusMoney;
				}

				// ���õ�ǰ������
				strMoney.Format("%I64d",m_iCurrentMoney);
				pEdit->SetText(CA2W(strMoney.GetBuffer()));
				pEdit->SetControlVisible(true);

				// �����Լ�������
				strMoney.Format("%I64d", m_iSelfMoney - m_iCurrentMoney);
				pText->SetText(CA2W(strMoney.GetBuffer()));
				pText->SetControlVisible(true);

				// ��ʾ����������
				SAFE_CTRL_OPERATE(IContain,DZPK_DAI_RU_MONEY_CONTAINER,SetControlVisible(true));
			}
			break;
		case DZPK_OFF_BUT: // �رմ���������
			{
				// ���ش���������
				SAFE_CTRL_OPERATE(IContain,DZPK_DAI_RU_MONEY_CONTAINER,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(true));
			}
			break;
		case DZPK_BET_BUT: // ��ע��ť
			{
				OnHandleBet();
			}
			break;
		case DZPK_CALL_BUT: // ��ע��ť
			{
				OnHandleCall();
			}
			break;
		case DZPK_RAISE_BUT: // ��ע��ť
			{
				OnHandleRaise();
			}
			break;
		case DZPK_CHECK_BUT: // ���ư�ť
			{
				OnHandleCheck();
			}
			break;
		case DZPK_FOLD_BUT: // ���ư�ť
			{
				OnHandleFolo();
			}
			break;
		case DZPK_ALL_IN_BUT: // ȫ�°�ť
			{
				OnHandleAllin();
			}
			break;
		case DZPK_MONEY_BUT: // �����ť
		case DZPK_MONEY_BUT + 1:
		case DZPK_MONEY_BUT + 2:
		case DZPK_MONEY_BUT + 3:
		case DZPK_MONEY_BUT + 4:
			{
				if (m_bSoundPlay)
				{
					// ���ѡ������
					PlayPublicSound(BT_MONEY_BUTTON);
				}

				OnHandleButtonMoney(pMessage->_uControlID - DZPK_MONEY_BUT);
			}
			break;
		case DZPK_JSLIDER_BUT: // ��������ť
			{
				OnHandleSlidingAxle();
			}
			break;
		case DZPK_ADD_MONEY: // ��ע(+)��ť
			{
				OnHandleAddMinus();
			}
			break;
		case DZPK_SUB_MONEY: // ��ע(-)��ť
			{
				OnHandleSubMinus();
			}
			break;
		case DZPK_CALL_TUO_GUAN_BUT: // ��ע�йܰ�ť
			{
				OnHandleCallMandate(true);
			}
			break;
		case DZPK_GOU_QU_CALL_TUO_GUAN_BUT: // �򹳸�ע�йܰ�ť
			{
				OnHandleCallMandate(false);
			}
			break;
		case DZPK_CHECK_TUO_GUAN_BUT: // �����йܰ�ť
			{
				OnHandleCheckMandate(true);
			}
			break;
		case DZPK_GOU_QU_CHECK_TUO_GUAN_BUT: // �򹳹����йܰ�ť
			{
				OnHandleCheckMandate(false);
			}
			break;
		case DZPK_CALL_ANY_TUO_GUAN_BUT: // ���κ�ע�йܰ�ť
			{
				OnHandleCallAnyMandate(true);
			}
			break;
		case DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT: // �򹳸��κ�ע�йܰ�ť
			{
				OnHandleCallAnyMandate(false);
			}
			break;
		case DZPK_CHECK_FOLD_TUO_GUAN_BUT: // ���ơ������йܰ�ť
			{
				OnHandlePassAbandonMandate(true);
			}
			break;	
		case DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT: // �򹳹��ơ������йܰ�ť
			{
				OnHandlePassAbandonMandate(false);
			}
			break;
		case DZPK_OFF_BALANCE_CONTAINER:	// �رս����
			{
				// �رս�������
				SAFE_CTRL_OPERATE(IContain,DZPK_BALANCE_CONTAINER,SetControlVisible(false));
			}
			break;;
		case DZPK_SET_BUT:	// ���ð�ť
			{
				UserSetGameInfo(m_bSoundPlay,m_bAllowWatch,true);
			}
			break;
		case DZPK_SET_OK_BUT:	  // ����ȷ����ť
			{
				IRadioButton *pRadioBtnVoice = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(DZPK_MUSIC_SELECT)) ;
				IRadioButton *pRadioBtnWatch = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(DZPK_KE_WATCH)) ; 
				if(NULL != pRadioBtnVoice && NULL!= pRadioBtnWatch)
				{
					m_bSoundPlay  = pRadioBtnVoice->GetIsSelect() ; 
					m_bAllowWatch = pRadioBtnWatch->GetIsSelect() ; 
 
					CString strCfgName;
					strCfgName.Format("%s%d.bcf",CINIFile::GetAppPath(),NAME_ID);
					DWORD dwCfgFile = ::cfgOpenFile(strCfgName);

					if(dwCfgFile >=0x10) //�ļ��򿪳ɹ�
					{
						//������������
						::cfgSetValue(dwCfgFile,"config","PlaySound",m_bSoundPlay);

						//�����Թۿ�������
						::cfgSetValue(dwCfgFile,"config","AllowWatch",m_bAllowWatch);

						::cfgClose(dwCfgFile);						
					}

				}
				if(m_bSoundPlay)
				{
					m_pSoundEngine->setAllSoundsPaused(false);
					m_pSEBkMusic->setAllSoundsPaused(false);	
					m_pBetSMusic->setAllSoundsPaused(false);
					m_pTimerSMusic->setAllSoundsPaused(false);

					// ���ű�������
					PlayBeiJingSound();
				}
				else
				{
					m_pSoundEngine->setAllSoundsPaused(true);
					m_pSEBkMusic->setAllSoundsPaused(true);
					m_pBetSMusic->setAllSoundsPaused(true);
					m_pTimerSMusic->setAllSoundsPaused(true);

					m_pSoundEngine->stopAllSounds();
					m_pSEBkMusic->stopAllSounds();
					m_pBetSMusic->stopAllSounds();
					m_pTimerSMusic->stopAllSounds();
				}

				UserSetGameInfo(m_bSoundPlay,m_bAllowWatch,false);
			}
			break;
		case DZPK_SET_CANCEL_BUT: // ����ȡ����ť
			{
				UserSetGameInfo(m_bSoundPlay,m_bAllowWatch,false);
			}
			break;
#ifdef CLOSE_CARD
		case DZPK_SHOW_CARD + 4:	//�Լ��ĵ���
			{
				OnHitMySelfBackCard();
				break;
			}
		case DZPK_SHOW_CARD_BUT:	//չʾ�ư�ť
			{
				OnHitShowCard();
				break;
			}
#endif
		default:{break;}
		}
	}
	if (pMessage->_uMessageType == UI_MOUSEENTER  && pMessage->_ch == 0)
	{
		TCHAR pchar[MAX_PATH]={0};
		wsprintf(pchar, TEXT("pzl pMessage->_uControlID=%d"),pMessage->_uControlID);
		OutputDebugString(pchar);
		if (pMessage->_uControlID == DZPK_JSLIDER_BUT)
		{
			// ��������ť
			OnHandleSlidingAxle();	
		}
	}
    return 0;
}
/// ����
int CNewGameClient::ResetGameFrame(void)
{
    return 0;
}
/// ��ҽ���
int CNewGameClient::GameUserCome()
{
	OnShowUserLogo();
	OnShowUserInfo();
	
    return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	OnShowUserLogo();
	
	CloseAllLogo(nIndex) ; 

	BYTE bViewStation =  m_pGameFrame->DeskStation2View(nIndex);
	SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + bViewStation,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IText,DZPK_USER_NAME + bViewStation,SetControlVisible(false));
	SAFE_CTRL_OPERATE(INum,DZPK_USER_MONEY + bViewStation,SetControlVisible(false));
		      
    return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	OnShowUserLogo() ; 
    return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
    return true;
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch (nTimerID)
	{
	case IDT_OK_LOAD: // �ȴ�����޸Ĵ�����
		{
			m_pGameFrame->KillTimer(IDT_OK_LOAD);
			
			// ��ǰ׼����������
			IEditorText * pEdit = dynamic_cast<IEditorText *>(m_pUI->GetIControlbyID(DZPK_PLAN_MONEY_INPUT_TEXT));
			if (pEdit != NULL)
			{
				// ���ñ༭��ֻ����������
				pEdit->SetEnableASCII(1, '0', '9');
				pEdit->SetNumberLimit(true,1,m_iMaxMoney);

				// ��ȡ��ǰ������
				__int64 nMoney;
				wchar_t* wszMoney = pEdit->GetText();
				nMoney = _wtoi64(wszMoney);
				m_iCurrentMoney = nMoney;

				// ������ݲ���Ⱦ͸���
				if (m_iCurrentMoney < m_iMinMoney)
				{
					m_iCurrentMoney = m_iMinMoney;
				}
				else if (m_iCurrentMoney > m_iMaxMoney)
				{
					m_iCurrentMoney = m_iMaxMoney;
				}

				// ��ֹ�����ҳ���������
				if (m_iCurrentMoney > m_iSelfMoney)
				{
					m_iCurrentMoney = m_iSelfMoney;
				}

				CString strSubPlanMoney;
				strSubPlanMoney.Format("%I64d",m_iCurrentMoney);
				pEdit->SetText(CA2W(strSubPlanMoney));
				pEdit->SetControlVisible(true);	
			}
			// ȷ������
			OnOKLOAD();
		}
		break;
	case DZPK_SHOW_CLCKTIME: // ����ʱ
		{
			if(m_uTime > 0)
			{
				m_uTime --;

				if (m_uTime < 5)
				{
					m_pTimerSMusic->setAllSoundsPaused(true);
					m_pTimerSMusic->stopAllSounds();

					if (m_bSoundPlay)
					{
						// ��������
						PlayTimerSound();
					}
				}
			}
			else
			{				
				m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);
				m_pTimerSMusic->setAllSoundsPaused(true);
				m_pTimerSMusic->stopAllSounds();

				for (int i = 0; i < PLAY_COUNT; i++)
				{
					ITime * pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(DZPK_SHOW_USER_TIMER + i));
					if (pTime != NULL)
					{
						pTime->SetPLay(false);
						pTime->SetControlVisible(false);
					}
				}

				OnHandleFolo();
			}
    	}
		break; 
	case IDT_COLLECT_JETONS:
		{
			//�ռ����붯�����
			m_pGameFrame->KillTimer(IDT_COLLECT_JETONS);

			m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);
			m_pTimerSMusic->setAllSoundsPaused(true);
			m_pTimerSMusic->stopAllSounds();

			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ZHUANG_LOGO, SetControlVisible(true));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BIG_BLIND,SetControlVisible(true));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_BLIND,SetControlVisible(true));
			
			m_pGameFrame->SetTimer(IDT_WAIT_DISPATCH_JETONS,1500);
		}
		break;
	case IDT_WAIT_DISPATCH_JETONS:
		{
			//�ȴ��������
			m_pGameFrame->KillTimer(IDT_WAIT_DISPATCH_JETONS);
			OnHandleDispatchJetons();
		}
		break;
	case IDT_DISPATCH_JETONS:
		{
			//�䷢�������
			m_pGameFrame->KillTimer(IDT_DISPATCH_JETONS);
			OnShowRoundResult(&m_tagResult);
		}
		break;
	case IDT_HIDE_BALANCE_TIMER:
		{
			m_pGameFrame->KillTimer(IDT_HIDE_BALANCE_TIMER);

			// ���ؽ����
			SAFE_CTRL_OPERATE(IContain,DZPK_BALANCE_CONTAINER,SetControlVisible(false));

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				SAFE_CTRL_OPERATE(IText,DZPK_WIN_USER_ID + i,SetText(CA2W(NULL)));
				SAFE_CTRL_OPERATE(IText,DZPK_WIN_USER_ID + i,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IText,DZPK_GONG_WIN_MONEY + i,SetText(CA2W(NULL)));
				SAFE_CTRL_OPERATE(IText,DZPK_GONG_WIN_MONEY + i,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IText,DZPK_LOSE_WIN_INFO + i,SetText(CA2W(NULL)));
				SAFE_CTRL_OPERATE(IText,DZPK_LOSE_WIN_INFO + i,SetControlVisible(false));
			}

			// ��������
			OnShowDeskTop();

			m_pGameFrame->SetTimer(IDT_IN_MONEY_BOX,500);
		}
		break;
	case IDT_MONEY_LACK:
		{
			SAFE_CTRL_OPERATE(IImage,DZPK_SELF_MONEY_BU_ZU,SetControlVisible(false));

			m_pGameFrame->SendGameData(ASS_MONEY_LACK, NULL,0);
			//SendGameData(MDM_GR_ROOM, ASS_GR_USER_EXIT_BYSELF,0);

			m_pGameFrame->CloseClient();
		}
		break;
	case IDT_IN_MONEY_BOX:
		{
			// �����������

			m_pGameFrame->KillTimer(IDT_IN_MONEY_BOX);
			
			if (m_bAutoStartTimer == false && !m_bWatch)
			{
				// �Զ�������
				ShowTimers(DZPK_SHOW_IN_TIME,m_tagDeskCfg.dz.iUserCloseTime);
			}

		}
		break;
	case  IDT_WIN_DONG_HUA_TIMER:
		{
			m_pGameFrame->KillTimer(IDT_WIN_DONG_HUA_TIMER);

			if (m_bWatch)
			{
				SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT, SetControlVisible(false));
				SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT, SetControlVisible(false));

				for (int i = 0; i < PLAY_COUNT; i++)
				{
					SAFE_CTRL_OPERATE(IExAnimate,DZPK_WIN_DONG_HUA + i,SetControlVisible(false));
					SAFE_CTRL_OPERATE(IExAnimate,DZPK_WIN_DONG_HUA + i,SetPlayState(false));
				}
			}

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				SAFE_CTRL_OPERATE(IImage, DZPK_SHOW_TOKEN + i, SetControlVisible(false));	 // �������
				SAFE_CTRL_OPERATE(IExAnimate,DZPK_WIN_DONG_HUA + i,SetControlVisible(false));
				SAFE_CTRL_OPERATE(IExAnimate,DZPK_WIN_DONG_HUA + i,SetPlayState(false));
			}
			
			if (!m_bWatch)
			{
				SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT, SetControlVisible(true));
			}

			//������������
			SAFE_CTRL_OPERATE(IContain,DZPK_BALANCE_CONTAINER,SetControlVisible(m_nCount > 0));
			m_pGameFrame->SetTimer(IDT_HIDE_BALANCE_TIMER,5000);

			m_bAutoStartTimer = false;
		}
		break;
	case IDT_AOUT_IN:
		{
			m_pGameFrame->KillTimer(IDT_AOUT_IN);
			OnOKLOAD();
		}
		break;
	case IDT_LOOK_CARD:	//����
		{
			m_pGameFrame->KillTimer(IDT_LOOK_CARD);
			
			BYTE byCardList[2] = {0,0};
			SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD+4,SetCardList(byCardList,2));
			break;
		}
	//case DZPK_SHOW_IN_TIME:
	//	{
	//		m_pGameFrame->KillTimer(DZPK_SHOW_IN_TIME);
	//		ITime * pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(DZPK_SHOW_IN_TIME));
	//		if (pTime)
	//		{
	//			pTime->SetPLay(false);
	//			pTime->SetControlVisible(false);
	//		}
	//	}
	//	break;
	default:{break;}
	}
}
/// ����״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
}

// ����˳�
bool CNewGameClient::OnGameQuiting()
{
	bool bForceQuitAsAuto = true;
	///�����ϲ�·��
	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);

	CString strName;
	strName.Format("%d",NAME_ID);

	CBcfFile File(szAppPathParent + "\\SpecialRule.bcf"); 

	bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strName,0) ; 

	CString strInfo = "";
	
	if(bForceQuitAsAuto == true)
	{
		strInfo += "��������Ϸ�У��Ƿ�Ҫ�˳���Ϸ?\n";
	}
	else
	{
		strInfo += "��������Ϸ�У�ǿ�˺󽫻�۳���������ע����(������������ע�߿ɲ���)���Ƿ���Ҫǿ�˳���Ϸ?\n";
	}

	if(m_bGameStation == GS_PLAY_GAME && !m_bWatch && m_iUserStation == TYPE_PLAY_GAME)
	{
		
		if(IDOK != AFCMessageBox(strInfo, "��ʾ",AFC_YESNO))
		{
			return false;
		}

		m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}	
	return true;
}

/// ������Ϸ״̬,��Ҫ�Ƕ����������������Ϣ
bool CNewGameClient::SetGameStation(void * pStationData, UINT uDataSize)
{
	m_bWatch = m_pGameFrame->GetIsWatching();
	m_byMeStation = m_pGameFrame->GetMyDeskStation();

	//OnPlayMoveFlagAnim(m_byMeStation,DZPK_SHOW_ZHUANG_LOGO,false);
	const TGSBase* pGSBase = (TGSBase*)pStationData;

	//������Ϸ�Ļ�������
	RecurSaveBase(pStationData);

	OnShowUserInfo();

	SetStationParameter(pGSBase->bGameStation);

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT_ROUND:
		{
			TGSWaitAgree* pGSWaitAgree = (TGSWaitAgree*)pStationData;

			// ����������Ҵ�����״̬
			TSubMoney cmd;
			cmd.bDeskStation = -1;
			memcpy(cmd.nMoney, pGSWaitAgree->nSubMoney, sizeof(cmd.nMoney));
			memcpy(cmd.bIn, pGSWaitAgree->bReady, sizeof(cmd.bIn));

			OnShowSubMoney(&cmd);

			SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT,SetControlVisible(true));	  // ��ʾ׼����ť
			SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(true)); // ��ʾ�޸Ľ��

			// �����������
			OnLoadSelectionDlg(true);
			
			CString strMoney;
			strMoney.Format("����ʾ��:��ǰ��ʹ�����:%I64d",pGSWaitAgree->tagDeskCfg.dz.iSubMinMoney);
			m_pGameFrame->InsertNormalMessage(strMoney.GetBuffer());
			strMoney.Format("����ʾ��:��ǰ��ߴ�����:%I64d",pGSWaitAgree->tagDeskCfg.dz.iSubMaxMoney);
			m_pGameFrame->InsertNormalMessage(strMoney.GetBuffer());
			strMoney.Format("����ʾ��:��ǰԤ��������:%I64d\n",pGSWaitAgree->tagDeskCfg.dz.iSubPlanMoney);
			m_pGameFrame->InsertNormalMessage(strMoney.GetBuffer());
			//m_pGameFrame->SetTimer(IDT_OK_LOAD,5000);
		}
		break;
	case GS_PLAY_GAME:
		{
			
			if (m_bSoundPlay)
			{
				// ��������
				PlayBeiJingSound();
			}

			m_bIsOffline = true;

			TGSPlaying * pGSPlaying = (TGSPlaying *)pStationData;
			// ��õõ����Ƶ����
			m_byTokenUser = pGSPlaying->byTokenUser;

			// ׯ�Ҵ�С��
			m_byNTUser = pGSPlaying->byNTUser;
			m_bySmallBlind = pGSPlaying->bySmallBlind;
			m_byBigBlind = pGSPlaying->byBigBlind;

			m_iUserStation = pGSPlaying->iUserStation; 

			// ȫ�����
			memcpy(m_bAllBet, pGSPlaying->bNoBet, sizeof(pGSPlaying->bNoBet));

			// �������
			memcpy(m_bGiveUp, pGSPlaying->bGiveUp, sizeof(pGSPlaying->bGiveUp));

			// ��ȡ������
			memcpy(m_iMoneys, pGSPlaying->nSubMoney, sizeof(pGSPlaying->nSubMoney));

			IImage * pImage = NULL;
			
			CString str;
			str.Format("Client:ׯ��m_byNTUser == %d,Сäעm_bySmallBlind == %d,��äעm_byBigBlind == %d",m_byNTUser,m_bySmallBlind,m_byBigBlind);
			OutputDebugString(str);
			
			if (m_byNTUser != 255)
			{
				OutputDebugString("Client:��ʾׯ��ͼƬ");
				// ��ʾׯ��ͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + m_pGameFrame->DeskStation2View(m_byNTUser),SetControlVisible(true));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ZHUANG_LOGO,SetControlVisible(false));
			}
			else
			{	
				OutputDebugString("Client:��ʾׯ��ԭʼλ��ͼƬ");
				// ��ʾׯ��ԭʼλ��ͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + m_pGameFrame->DeskStation2View(m_byNTUser),SetControlVisible(false));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ZHUANG_LOGO,SetControlVisible(true));
			}

			if (m_bySmallBlind != 255)
			{
				// ��ʾСäעͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + m_pGameFrame->DeskStation2View(m_bySmallBlind),SetControlVisible(true));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_BLIND,SetControlVisible(false));
			}
			else
			{
				// ��ʾСäעԭʼλ��ͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + m_pGameFrame->DeskStation2View(m_bySmallBlind),SetControlVisible(false));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_BLIND,SetControlVisible(true));
			}

			if (m_byBigBlind != 255)
			{
				// ��ʾ��äעͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + m_pGameFrame->DeskStation2View(m_byBigBlind),SetControlVisible(true));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BIG_BLIND,SetControlVisible(false));
			}
			else
			{
				// ��ʾ��äעԭʼλ��ͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + m_pGameFrame->DeskStation2View(m_byBigBlind),SetControlVisible(false));
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BIG_BLIND,SetControlVisible(true));
			}

			// �����ť
			OnShowSysDeskCfg(&m_tagDeskCfg);


			// ��������
			m_iCardCounts = pGSPlaying->nHandCardNums;
			memcpy(m_iUserDataCard, pGSPlaying->byHandCard, sizeof(BYTE) * m_iCardCounts);

			INoMoveCard * pMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + m_pGameFrame->DeskStation2View(m_byMeStation)));
			if (pMoveCard != NULL)
			{
				pMoveCard->SetCardList(m_iUserDataCard,m_iCardCounts);
				pMoveCard->SetControlVisible(true);
			}

			// ���ƹ�����
			m_iCardDataCounts = pGSPlaying->nCardsCount;
			memcpy(m_iCardData, pGSPlaying->byCards, sizeof(BYTE) *  pGSPlaying->nCardsCount);

			pMoveCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_BASE_CARD));
			if (pMoveCard != NULL)
			{
				pMoveCard->SetCardList(m_iCardData,m_iCardDataCounts);
				pMoveCard->SetControlVisible(true);
			}

			UserInfoForGame user;
			memset(&user,0,sizeof(user));
			CString strMoney;

			// ��ע���
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (!pGSPlaying->bHaveUser[i]/*!m_pGameFrame->GetUserInfo(i,user)*/)
				{
					continue ;
				}

				BYTE byView = m_pGameFrame->DeskStation2View(i);

				bool bUser = m_pGameFrame->GetUserInfo(i,user);

				//if(m_byMeStation == i && m_iUserStation != TYPE_PLAY_GAME)
				//{
				//	continue;
				//}
				//else
				bool  bControlShow = ((m_byMeStation != i)||(m_byMeStation == i && m_iUserStation == TYPE_PLAY_GAME)); 

			/*	CString str;
				str.Format("Client:�Լ�:%d != ��ǰ���:%d���Լ�:%d == ��ǰ���:%d���Ƿ������:%d == TYPE_PLAY_GAME:%d",m_byMeStation,i,m_byMeStation, i ,m_iUserStation ,TYPE_PLAY_GAME);
				OutputDebugString(str);*/

				SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI + byView,SetControlVisible(bControlShow));
				SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI + byView,SetControlVisible(bControlShow));

				// ��ʾ������
				INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_USER_MONEY + byView));
				if (pNum != NULL)
				{
					pNum->SetNum(m_iMoneys[i]);
					pNum->SetRankType(0);
					pNum->SetControlVisible(bControlShow);
				}
				
				if (m_bGiveUp[i])
				{
					//��ʾ����UIͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_FOLD_LOGO + byView,SetControlVisible(bControlShow));

					//��ʾ����ͷ��UIͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + byView,SetControlVisible(bControlShow));
				}

				if (m_bAllBet[i])
				{
					//��ʾ����UIͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ALLIN_LOGO + byView,SetControlVisible(bControlShow));
				}

				// ��ʾ����ǳ�
				IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_USER_NAME + byView));
				if (pText != NULL)
				{
					CString strName;
					strName.Format("%s",user.szNickName);
					pText->SetText(CA2W(strName));
					pText->SetControlVisible(true);
				}
				
				//��ʾ������UIͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + byView,SetControlVisible(bControlShow));

				// ��ʾ��ǰ���ý��ͼƬ
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_KE_YONG_MONEY,SetControlVisible(bControlShow));

				// ��ʾ��ע���
				m_nBetMoney[i] = 0;
				for (int j = 0; j < PLAY_COUNT; j++)
				{
					m_nBetMoney[i] += pGSPlaying->nBetMoney[i][j];
					/*strMoney.Format("Client:���%d ��ʾ��ע���%I64d",pGSPlaying->nBetMoney[i][j]);
					OutputDebugString(strMoney);*/
				}

				if (m_nBetMoney[i] > 0 && m_bpUserInfo[i] == TYPE_PLAY_GAME)
				{
					// ��ʾ��ע����ͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DESKTOP_MONEY_LOGO + byView,SetControlVisible(bControlShow));

					pNum = dynamic_cast<INum*>(m_pUI->GetIControlbyID(DZPK_SHOW_DESKTOP_MONEY + byView));
					if (pNum != NULL)
					{
						pNum->SetRankType(0);
						pNum->SetNum(m_nBetMoney[i]);
						pNum->SetControlVisible(bControlShow);
					}
				}
				else
				{
					// ������ע����ͼƬ
					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DESKTOP_MONEY_LOGO + byView,SetControlVisible(false));

					// ������ע���
					SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + byView,SetControlVisible(false));
				}

			}

			// ��ǰ�������
			TToken toKen;
			toKen.bNewTurn	 = pGSPlaying->bNewTurn;
			toKen.bNewTurns	 = pGSPlaying->bNewTurns;
			toKen.byUser	 = pGSPlaying->byTokenUser;
			toKen.byVerbFlag = pGSPlaying->byVerbFlag;
			toKen.nCallMoney = pGSPlaying->nCallMoney;

			if ((m_tagDeskCfg.Time.byOperate - pGSPlaying->iEvPassTime) <= 0)
			{
				OnShowGetToken(&toKen,1);

			}
			else
			{
				OnShowGetToken(&toKen,m_tagDeskCfg.Time.byOperate - pGSPlaying->iEvPassTime);
			}
		}
		break;
	default:{break;}
	}

	return true;
}

// ��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter()
{
	return m_bGameStation;
}

//��Ϸ��ʼ
void CNewGameClient::OnHandleGameBegin(BYTE * buffer,int nLen)
{
	SetStationParameter(GS_PLAY_GAME);

	m_pSoundEngine->setAllSoundsPaused(true);
	m_pSoundEngine->stopAllSounds();

	if (m_bSoundPlay)
	{
		// ��������
		PlayBeiJingSound();
	}
	
	if (m_bWatch)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT,SetControlVisible(false));	// ����׼����ť
		SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(false));	// �����޸Ľ��
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_READY_GO,SetControlVisible(false));		// ������׼��ͼƬ
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + i, SetControlVisible(false));	// ����׼��ͼƬ
		}

		// ֹͣ������ʱ��
		m_pGameFrame->KillTimer(IDT_IN_MONEY_BOX);
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_IN_TIME,SetPLay(false));
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_IN_TIME,SetControlVisible(false));

		// ��ʼ��
		OnInitData();
		return;
	}
	
	SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT,SetControlVisible(false));	// ������Ϸ��ʼ��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(false));	// ���ش����ť
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_READY_GO,SetControlVisible(false));		// ������׼��ͼƬ
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + i, SetControlVisible(false));	// ����׼��ͼƬ
	}

	SAFE_CTRL_OPERATE(IContain,DZPK_DAI_RU_MONEY_CONTAINER, SetControlVisible(false));	// ���ش���������

	// ֹͣ������ʱ��
 	m_pGameFrame->KillTimer(IDT_IN_MONEY_BOX);
	SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_IN_TIME,SetPLay(false));
	SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_IN_TIME,SetControlVisible(false));

	m_iUserStation = TYPE_PLAY_GAME ;   

	OnShowUserInfo();

	// ��ʼ��
	OnInitData();
}

//�յ�ϵͳ�������ӵ���Ϣ����������
void CNewGameClient::OnHandleGetSysDeskCfg(BYTE * buffer,int nLen)
{
	TDeskCfg* pDeskCfg = (TDeskCfg*)buffer;

	/*if (nLen != sizeof(buffer))
	{
		return;
	}*/

	//��������
	::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));

	OnShowSysDeskCfg(&m_tagDeskCfg);
}

//���õ�ע
void CNewGameClient::OnHandleCfgDesk(BYTE * buffer,int nLen)
{
	TCfgDesk* pCfgDesk = (TCfgDesk*)buffer;

	if (nLen != sizeof(TCfgDesk))
	{
		return;
	}
	//������Ϸ��״̬
	SetStationParameter(GS_CONFIG_NOTE);
}

//�������õ�ע��ʱ            
void CNewGameClient::OnHandleWaitCfgDeskTimeout()
{
	//֪ͨ������л滭����
	//ShowTimer(m_byMeStation, 5, IDT_TIMEOUT_WAIT_CONFIG_NOTE);
}

//�յ�ע����Ϣ����������
void CNewGameClient::OnHandleGetDeskCfg(BYTE * buffer,int nLen)
{
	TDeskCfg* pDeskCfg = (TDeskCfg*)buffer;

	//������Ϸ��״̬
	SetStationParameter(GS_CONFIG_NOTE);

	//��������
	::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));
}

//�յ���������Ϣ����������
void CNewGameClient::OnHandleInMoney(BYTE * buffer,int nLen)
{
	TSubMoney* pSubMoney = (TSubMoney*)buffer;
	
	OnShowSubMoney(pSubMoney);
}

//�յ���������һ���ư���������
void CNewGameClient::OnHandleSendACards(BYTE * buffer,int nLen)
{
	TCards* pCards = (TCards*)buffer;

	if (m_byNTUser == 255)
	{
		return ;
	}

	//���±�������			
	//m_iUser = pCards->byUser;

	m_nTypeCard = pCards->nTypeCard;	

	BYTE byUser = m_pGameFrame->DeskStation2View(m_byMeStation);
	
	// �Լ���������
	::memcpy(m_iUserDataCard, pCards->byCards, MAX_HANDCARDS_NUM);

	m_iCardCounts = pCards->iCardsNum;

	for (int i = 0; i < 2; ++i)
	{
		BYTE byNextUser = 255;

		//��ׯ�ҿ�ʼ
		TSendCardAnimNode tagNode;
		tagNode.byUser = m_byNTUser;
		tagNode.iPos = i;
		m_qSendCards.push(tagNode);

		byNextUser = GetNextUserStations(m_byNTUser);

		while(byNextUser != m_byNTUser)
		{
			TSendCardAnimNode tagNode;
			tagNode.byUser = byNextUser;
			tagNode.iPos = i;

			m_qSendCards.push(tagNode);

			byNextUser = GetNextUserStations(byNextUser);
		}
	}

	OnFaPaiAnim();
}

//�յ��������߳ظ��°���������
void CNewGameClient::OnHandleBetPoolUp(BYTE * buffer,int nLen)
{
	TBetPool* pBetPool = (TBetPool *)buffer;

	memcpy(&m_tagBetPool, pBetPool, sizeof(m_tagBetPool));

	for (int i = 0; i < 8; i++)
	{
		if (pBetPool->iBetPools[i] > 0)
		{
			SAFE_CTRL_OPERATE(IImage, DZPK_SHOW_SIDE_POT + i,SetControlVisible(true));

			IText * pStatic = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_POT_DINAR + i));

			if (pStatic != NULL)
			{
				CString str;
				if (i == 0)
				{
					str.Format("����:%I64d", pBetPool->iBetPools[i]);
				}
				else
				{
					str.Format("�߳�%d:%I64d", i, pBetPool->iBetPools[i]);
				}
				pStatic->SetText(CA2W(str.GetBuffer()));
				pStatic->SetControlVisible(true);
			}
		}
	}
}

//�յ���������һ���ư���������
void CNewGameClient::OnHandleSend3Cards(BYTE * buffer,int nLen)
{
	TCards * pCards = (TCards *)buffer;

	memcpy(m_iCardData, pCards->byCards, sizeof(BYTE) * (pCards->iCardsNum));
	m_iCardDataCounts += pCards->iCardsNum;

	// ��ʾ3�Ź�����
	INoMoveCard * pMoveCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_BASE_CARD));
	if (pMoveCard != NULL)
	{
		pMoveCard->SetCardList(pCards->byCards,pCards->iCardsNum);
		pMoveCard->SetUpCard(0,0,0);
		pMoveCard->SetControlVisible(true);
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + i,SetControlVisible(false));
	}
}

//�յ���������һ���ư���������
void CNewGameClient::OnHandleSend1Cards(BYTE * buffer,int nLen)
{
	TCards * pCards = (TCards *)buffer;

	memcpy(m_iCardData, pCards->byCards, sizeof(BYTE) * (pCards->iCardsNum));
	m_iCardDataCounts = pCards->iCardsNum;

	// ��ʾ4 - 5 ������
	INoMoveCard* pMoveCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_BASE_CARD));
	if (pMoveCard != NULL)
	{
		pMoveCard->SetCardList(pCards->byCards, pCards->iCardsNum > 5 ? 5 : pCards->iCardsNum);
		pMoveCard->SetControlVisible(true);
	}

	
}

//�յ�����������������ҷ�����ɵ���Ϣ��������
void CNewGameClient::OnHandleSendCardFinish()
{

}

//�յ������ע����Ϣ
void CNewGameClient::OnHandleGetBetInfo(BYTE * buffer,int nLen)
{
	TBet* pBet = (TBet*)buffer;

	m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);
	m_pTimerSMusic->setAllSoundsPaused(true);
	m_pTimerSMusic->stopAllSounds();
	

	// ������ע��ť
	OnShowOtherPlayerGetToken();

	BYTE byUser = m_pGameFrame->DeskStation2View(pBet->byUser);


	if (pBet->nType == ET_AUTO || pBet->nType == ET_BET || pBet->nType == ET_ADDNOTE || pBet->nType == ET_ALLIN || pBet->nType == ET_CALL)
	{
		m_nBetMoney[pBet->byUser] += pBet->nMoney; 
		m_iMoneys[pBet->byUser] -= pBet->nMoney;

		INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_USER_MONEY + byUser));
		if (pNum != NULL)
		{
			pNum->SetNum(m_iMoneys[pBet->byUser]);
			pNum->SetRankType(0);
			pNum->SetControlVisible(true);
		}

		if (m_bSoundPlay)
		{
			// ��������
			PlayPublicSound(BT_CHIP_MUSIC);
		}
	}

	// ��¼ȫ����������
	if (pBet->nType == ET_ALLIN)
	{
		m_bAllBet[pBet->byUser] = true;
	}
	else if (pBet->nType == ET_FOLD)
	{
		m_bGiveUp[pBet->byUser] = true;		
	}

	if (m_byBigBlind == pBet->byUser)
	{
		OutputDebugString("dxh: -------------����ͼƬ���غ��� Client--------------");
		//OnUserSortOut();
	}

	// ��¼�����Ҳ�������
	m_emLastOperation[pBet->byUser] = pBet->nType;

	// ��¼�������
	m_stackLastStation.push(pBet->byUser);

	IImage * pImage = NULL;

	switch (pBet->nType)
	{
	case ET_BET:		// ��עͼƬ��ʾ
		{
			pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_LOGO + byUser));

			if (pImage != NULL)
			{
				pImage->SetControlVisible(true);
			}

			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + byUser,SetControlVisible(false));

			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetPLay(false));
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetControlVisible(false));

		}
		break;
	case ET_CALL:		// ��עͼƬ��ʾ
		{
			pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_CALL_LOGO + byUser));

			if (pImage != NULL)
			{
				pImage->SetControlVisible(true);
			}

			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + byUser,SetControlVisible(false));

			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetPLay(false));
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetControlVisible(false));

			if (m_bSoundPlay)
			{
				// ��ע��Ч
				PlayBetSound(pBet->byUser,ST_CALL);
			}
		}
		break;
	case ET_ADDNOTE:	// ��עͼƬ��ʾ
		{
			pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_RAISE_LOGO + byUser));
			if (pImage != NULL)
			{
				pImage->SetControlVisible(true);
			}

			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + byUser,SetControlVisible(false));

			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetPLay(false));
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetControlVisible(false));

			if (m_bSoundPlay)
			{
				// ��ע��Ч
				PlayBetSound(pBet->byUser,ST_RAISE);
			}
		}
		break;
	case ET_CHECK:		// ����ͼƬ��ʾ
		{
			pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_CHECK_LOGO + byUser));
			if (pImage != NULL)
			{
				pImage->SetControlVisible(true);
			}

			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + byUser,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + byUser,SetControlVisible(false));

			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetPLay(false));
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetControlVisible(false));

			if (m_bSoundPlay)
			{
				// ������Ч
				PlayBetSound(pBet->byUser,ST_CHECK);
			}
		}
		break;
	case ET_FOLD:		// ����ͼƬ��ʾ
		{
			pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_FOLD_LOGO + byUser));
			if (pImage != NULL && m_iUserStation == TYPE_PLAY_GAME)
			{
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_FOLD_LOGO + byUser,SetControlVisible(true));
				pImage->SetControlVisible(true);
				CString strFold;
				if (m_byNTUser == m_bySmallBlind)
				{
					if (pBet->byUser == m_byNTUser)
					{						
						OnPlayMoveFlagAnim(pBet->byUser,DZPK_SHOW_ZHUANG_LOGO,false);
					}
					else if (pBet->byUser == m_byBigBlind)
					{
						OnPlayMoveFlagAnim(pBet->byUser,DZPK_SHOW_BIG_BLIND,false);
					} 
				}
				else
				{
					if (pBet->byUser == m_byNTUser)
					{
						OnPlayMoveFlagAnim(pBet->byUser,DZPK_SHOW_ZHUANG_LOGO,false);
					}
					else if (pBet->byUser == m_bySmallBlind)
					{
						OnPlayMoveFlagAnim(pBet->byUser,DZPK_SHOW_SMALL_BLIND,false);
					} 
					else if (pBet->byUser == m_byBigBlind)
					{
						OnPlayMoveFlagAnim(pBet->byUser,DZPK_SHOW_BIG_BLIND,false);
					}
				}		
			}
			
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetPLay(false));
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetControlVisible(false));

			SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD + byUser,SetDarkCard(m_iUserDataCard,m_iCardCounts));
			
			if (m_bSoundPlay)
			{
				// ������Ч
				PlayBetSound(pBet->byUser,ST_FOLD);
			}
			
		}
		break;
	case ET_ALLIN:		// ȫ��ͼƬ��ʾ
		{
			pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_ALLIN_LOGO + byUser));
			if (pImage != NULL)
			{
				pImage->SetControlVisible(true);
			}

			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetPLay(false));
			SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + byUser,SetControlVisible(false));

			if (m_bSoundPlay)
			{
				// ȫ����Ч
				PlayBetSound(pBet->byUser,ST_ALL_IN);
			}
		}
		break;
	}

	//��ʾ������UIͼƬ
	pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_DESKTOP_MONEY_LOGO + byUser));
	if (pImage != NULL && pBet->nType != ET_FOLD)
	{	
		pImage->SetControlVisible(true);
	}
	//else
	//{
	//	ShowControl(IDC_MONEY_MAP + ViewStation(pBet->byUser), SW_HIDE);
	//}

	// ���ý����ʾ
	INum * pNum = dynamic_cast<INum*>(m_pUI->GetIControlbyID(DZPK_SHOW_DESKTOP_MONEY + byUser));
	if (pNum != NULL)
	{
		pNum->SetNum(m_nBetMoney[pBet->byUser]);

		if (m_nBetMoney[pBet->byUser] <= 0)
		{
			pNum->SetRankType(0);
			pNum->SetNum(0);
			pNum->SetControlVisible(false);
		}
		else
		{
			pNum->SetRankType(0);
			pNum->SetControlVisible(true);
		}
	}
}

//�յ�������Ϣ�������û�
void CNewGameClient::OnHandleGetToken(BYTE * buffer,int nLen)
{
	TToken* pToken = (TToken*)buffer;
	
	OnShowGetToken(pToken);
}

//���ƴ���ӿ�
void CNewGameClient::OnHandleCompareCard(BYTE * buffer,int nLen)
{
	m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);
	m_pTimerSMusic->setAllSoundsPaused(true);
	m_pTimerSMusic->stopAllSounds();

	// �������ж�ʱ��
	OnClearTimer();

	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT, SetControlVisible(false));		// ��ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT, SetControlVisible(false));		// �����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT, SetControlVisible(false));	// ���κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT, SetControlVisible(false));	// ���ơ������йܰ�ť

	OnShowPdg(true);

	TCompareCard *pCmd = (TCompareCard*)buffer;
	
	UserInfoForGame User;
	memset(&User,0,sizeof(User));

	INoMoveCard* pCard = NULL;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SIDE_POT + i, SetControlVisible(false));	// ���ر߳�ͼƬ * 8
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_POT_DINAR + i, SetControlVisible(false));	// ���ر߳ؽ�� * 8

		if (!m_pGameFrame->GetUserInfo(i,User) || pCmd->nHandCardCount[i] <= 0)
		{
			continue ;
		}

		// ��ʾ����߳�����
		OnShowPots(&m_tagBetPool);

		pCard = dynamic_cast<INoMoveCard * >(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + m_pGameFrame->DeskStation2View(i)));
		if (pCard != NULL)
		{
			BYTE bCard[2] = {0,0};
			if (i == m_byMeStation)
			{
				// �Լ�����2�ſ��õ��Ƶ���
				for (int j = 0; j < 5; j++)
				{
					if (/*bCard[0] == 0 && */pCmd->bHandCards[i][0] == pCmd->bCards[i][j])
					{
						bCard[0] = pCmd->bCards[i][j];
					}

					if (/*bCard[1] == 0 && */pCmd->bHandCards[i][1] == pCmd->bCards[i][j])
					{
						bCard[1] = pCmd->bCards[i][j];
					}

					if (bCard[0] && bCard[1])
					{
						break;
					}
				}
			}

			BYTE byConst[2] = {1,1};
			BYTE byCard[2] = {0,0};
			BYTE byCardCount = 0;

			for (int i = 0; i < 2; i++)
			{
				if (bCard[i] != 0x00)
				{
					byCard[byCardCount++] = bCard[i];
				}
			}

			pCard->SetUpCard(byCard, byCardCount,byConst);
			pCard->SetDarkCard(pCmd->bCardsEx[m_byMeStation],2);
		}
		
		/*BYTE bCardType[] = {0, 9, 8, 7, 6, 5, 5, 4, 3, 2, 1, 1, 0};
		IImage * pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(DZPK_PAI_TYPE_MAP + m_pGameFrame->DeskStation2View(i)));
		if (pImage != NULL)
		{
			pImage->SetSrcX(bCardType[pCmd->nCardKind[i]] * 150);
			pImage->SetControlVisible(true);
		}*/

		IExAnimate * pAnimagte = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_PAI_TYPE_ANIMATION + m_pGameFrame->DeskStation2View(i)));
		if (pAnimagte != NULL && !m_bGiveUp[i] && m_nBetMoney[i] != 0)
		{
			TCHAR path[MAX_PATH];
			TCHAR szName[][20] = {"", "ɢ��", "����", "����", "����","��С˳��","˳��", "ͬ��", "��«", "����", "��Сͬ��˳", "ͬ��˳", "�ʼ�ͬ��˳"};
			int nAminalID[] = {0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9};
			CString str;
			str.Format("jan:�ͻ��� ���λ��: %d, �û�ID:%s,  ����: ID:%d  ����:%s, ���Ŷ���ID:%d ", i, User.szNickName,  pCmd->nCardKind[i], szName[pCmd->nCardKind[i]], nAminalID[pCmd->nCardKind[i]]);
			OutputDebugString(str);

			wsprintf(path,"resources\\Animation\\PaiDongHua\\%d.bani",nAminalID[pCmd->nCardKind[i]]);
			pAnimagte->SetExtFilePath(CA2W(path));
			pAnimagte->LoadFromFile();
			pAnimagte->SetControlVisible(true);
			pAnimagte->SetPlayState(true);
			pAnimagte->SetIsSymmetrical(false);
			pAnimagte->SetIsLoop(true);
		}

		if (i == m_byMeStation)
		{
			pCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_BASE_CARD));
			if (pCard != NULL)
			{
				BYTE bCard[5] = {0,0,0,0,0};
				BYTE bCardCount = 0;

				// �Լ�����2�ſ��õ��Ƶ���
				for (int j = 0; j < 5; j++)
				{
					for (int k = 0; k < 5; k++)
					{
						if (/*bCard[k] == 0 && */m_iCardData[k] == pCmd->bCards[i][j])
						{
							bCard[k] = pCmd->bCards[i][j];
							//bCardCount++;
							break;
						}
					}
				}

				BYTE byConst[5] = {1,1,1,1,1};
				BYTE byCard[5]={0,0,0,0,0};
				BYTE byCardCount = 0;

				for (int i = 0; i < 5; i++)
				{
					if (bCard[i] != 0x00)
					{
						byCard[byCardCount++] = bCard[i];
					}
				}

				pCard->SetUpCard(byCard,byCardCount,byConst);
				pCard->SetDarkCard(pCmd->bCardsEx[m_byMeStation],2);
			}
		}

		pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + m_pGameFrame->DeskStation2View(i)));
		if (pCard != NULL/* && !m_bGiveUp[i]*/)
		{
			pCard->SetCardList(pCmd->bHandCards[i],2);
			pCard->SetControlVisible(true);
		}
	}
}

//�յ�������Ϣ����������
void CNewGameClient::OnHandleRoundResult(BYTE * buffer,int nLen)
{
	if (m_bGameStation != GS_PLAY_GAME)
	{
		return;
	}

	TResult* pResult = (TResult*)buffer;

	//��¼��Ϸ����
	::memcpy(&m_tagResult,pResult,sizeof(TResult));

	//������Ϸ��״̬
	SetStationParameter(GS_WAIT_NEXT_ROUND);

	//֪ͨ������л滭����

	CString str;

	//�ռ�����
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		str.Format("Client:Client-m_nBetMoney[%d] == %I64d",i,m_nBetMoney[i]);
		OutputDebugString(str);
		if (m_nBetMoney[i] > 0)
		{
			OnShowCollectJotens(i);
		}


		for (int j = 0; j < 8; j++)
		{
			if (m_tagResult.nUserBet[i][j] > 0)
			{
				if (m_tagResult.bWinBetPools[i][j])
				{
					// 2012-10-11 duanxaiohui  add ͷ�����ͻ��ʤ���򷵻ؽ�ҵ����
					SAFE_CTRL_OPERATE(IImage, DZPK_SHOW_TOKEN + m_pGameFrame->DeskStation2View(i), SetControlVisible(true));	 // �������
					break;
				}
			}
		}
	}
	
	// �ռ����붯����ʱ��
	m_pGameFrame->SetTimer(IDT_COLLECT_JETONS,1500);

	if (m_bIsOffline && !m_bWatch && m_iUserStation != TYPE_PLAY_GAME)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + i,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + i,SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + i,SetControlVisible(false));
		}
	}
	else
	{
		// ��ʾׯ�ҡ�Сäע����äע
		if (m_byNTUser == m_bySmallBlind)
		{

			//m_pGameEngine->ShowControl(IDC_SMALL_BLIND, SW_HIDE);
			if (!m_bGiveUp[m_byNTUser])
			{
				OnPlayMoveFlagAnim(m_byNTUser,DZPK_SHOW_ZHUANG_LOGO,false);
			}

			if (!m_bGiveUp[m_byBigBlind])
			{
				OnPlayMoveFlagAnim(m_byBigBlind,DZPK_SHOW_BIG_BLIND,false);
			}
		}
		else
		{
			if (!m_bGiveUp[m_bySmallBlind])
			{
				OnPlayMoveFlagAnim(m_bySmallBlind,DZPK_SHOW_SMALL_BLIND,false);
			}
			if (!m_bGiveUp[m_byBigBlind])
			{
				OnPlayMoveFlagAnim(m_byBigBlind,DZPK_SHOW_BIG_BLIND,false);
			}
			if (!m_bGiveUp[m_byNTUser])
			{
				OnPlayMoveFlagAnim(m_byNTUser,DZPK_SHOW_ZHUANG_LOGO,false);
			}
		}
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		// ����V1��
		SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI + i,SetControlVisible(false));
		// ����V2��
		SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI + i,SetControlVisible(false));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + i,SetControlVisible(false));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + i,SetControlVisible(false));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + i,SetControlVisible(false));
		// ����Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + i,SetControlVisible(false));
		// ����Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_FOLD_LOGO + i,SetControlVisible(false));
		// ȫ��Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ALLIN_LOGO + i,SetControlVisible(false));
		// ��ҷ���ͷ��ͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + i,SetControlVisible(false));
		// �������
		//SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_TOKEN + i,SetControlVisible(false));

		m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetPLay(false));
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + i,SetControlVisible(false));
	}	


	m_pGameFrame->KillTimer(IDT_MODIFY_MONEY);				// �ȴ�����޸Ĵ�����
	m_pGameFrame->KillTimer(IDT_AOUT_IN);					// �Զ�������
	m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);			

	// ��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetControlVisible(false));
	// ��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetControlVisible(false));
	// ��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetControlVisible(false));
	// ���ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetControlVisible(false));
	// ���ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(false));
	// ȫ�°�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_ALL_IN_BUT,SetControlVisible(false));
	// ��������ť
	SAFE_CTRL_OPERATE(ISlider,DZPK_JSLIDER_BUT,SetControlVisible(false));
	// ��ע(+)��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(false));
	// ��ע(-)��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(false));
	// ��������ͼ
	SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(false));
	// ��ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(false));
	// �����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(false));
	// ���κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));
	// ���ơ������йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));
	// �򹳸�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));
	// �򹳹����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));
	// �򹳸��κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));
	// �򹳹��ơ������йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));
	//չʾ�ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_SHOW_CARD_BUT,SetControlVisible(false));

	for (int i = 0; i < 5; i++)
	{
		// �����ť
		SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));
	}

	//�����й�
	OnAutoTrusteeship(SW_HIDE);
}

//�յ��µ�һ�غϿ�ʼ��Ϣ����������
void CNewGameClient::OnHandleNewRoundBegin(BYTE * buffer,int nLen)
{
	TNextRoundBeginInfo* pNextRoundBeginInfo = (TNextRoundBeginInfo*)buffer;

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

	// ��ʾ��ҵ���
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		INoMoveCard * pGameCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD + i));
		if (pGameCard)
		{
			pGameCard->SetCardList(0,0);
		}
	}

	// ��ʾׯ�ҡ�Сäע����äע
	if (pNextRoundBeginInfo->byNTUser == pNextRoundBeginInfo->bSmallBlind)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_BLIND,SetControlVisible(false));
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->byNTUser,DZPK_SHOW_ZHUANG_LOGO,true);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->bBigBlind,DZPK_SHOW_BIG_BLIND,true);
	}
	else
	{
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->byNTUser,DZPK_SHOW_ZHUANG_LOGO,true);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->bSmallBlind,DZPK_SHOW_SMALL_BLIND,true);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->bBigBlind,DZPK_SHOW_BIG_BLIND,true);
	}

}

//û����ҽ�����Ϸ���˳�����
void CNewGameClient::OnHandleNoPlayer(BYTE * buffer,int nLen)
{
	TNoPlayer* pNoPlayer = (TNoPlayer*)buffer;

	//֪ͨ������л滭����
	//if (!m_pGameEngine->IsResultDlgShowing())
	//{
	//	//m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);
	//	//m_pGameEngine->ShowFlashTimer(m_byMeStation,5,IDT_TIMEOUT_NO_PLAYER);
	//	OnShowNoPlayer(pNoPlayer);
	//	//m_pMedia->m_bExitDesk = true;
	//}
	//else
	//{
	//	OnShowNoPlayer(pNoPlayer,true);
	//}
}

//������ϵ�Ǯ���㹻ʱ�Ĵ���ӿ�
void CNewGameClient:: OnHandleNotEnoughMoney(BYTE * buffer,int nLen)
{
	TMoneyLack * pMoneyLack = (TMoneyLack *)buffer;
	
	if (m_byMeStation == pMoneyLack->nbyUser)
	{
		/*CString str;
		str.Format("dxh: ���:%d(λ��:%d) �˳�", m_pGameEngine->GetUserInfo(m_byMeStation)->GameUserInfo.dwUserID, pMoneyLack->nbyUser);
		OutputDebugString(str);*/
		//m_pGameFrame->SendGameData(MDM_GR_ROOM, ASS_GR_USER_EXIT_BYSELF,0);
	}

	//֪ͨ������л滭����
	BYTE byUser = m_pGameFrame->DeskStation2View(pMoneyLack->nbyUser);	// ��ȡ���λ��

	SAFE_CTRL_OPERATE(IImage,DZPK_USER_LOGO + byUser,SetControlVisible(false));			// �������ͷ��LOGO
	SAFE_CTRL_OPERATE(IText,DZPK_USER_NAME + byUser,SetControlVisible(false));			// ��������ǳ�
	SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + byUser,SetControlVisible(false));			// ����׼��ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + byUser,SetControlVisible(false));			// ������ҽ��ͼƬ
	SAFE_CTRL_OPERATE(INum,DZPK_USER_MONEY + byUser,SetControlVisible(false));			// ������ҽ����
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_USER_CHAIR + byUser,SetControlVisible(true));	// ��ʾ�������
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + byUser,SetControlVisible(false));		// ���������עͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + byUser,SetControlVisible(false));	// ������Ҹ�עͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_FOLD_LOGO + byUser,SetControlVisible(false));	// �����������ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + byUser,SetControlVisible(false));	// ������ҹ���ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + byUser,SetControlVisible(false));	// ������Ҽ�עͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ALLIN_LOGO + byUser,SetControlVisible(false));	// �������ȫ��ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI + byUser,SetControlVisible(false));			// ���ص���ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI + byUser,SetControlVisible(false));			// ���ص���ͼƬ

	//StopAndHideFlashTimer(m_byMeStation);
}

/**
* ��ID�ű�ʶ��ʱ�������ã�����Ϊλ�ã����ʱ������;
* @param [in] bDeskStation ��ʱ��λ��
* @param [in] uMaxTime     ʱ�� ��s(��)Ϊ��λ
* @param [in] uTimeID      ��ʱ��ID
*/
void CNewGameClient::ShowTimer(BYTE bDeskStation,UINT uTimeID,UINT uMaxTime)
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return;
	}

	BYTE byUser = m_pGameFrame->DeskStation2View(bDeskStation);

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetPLay(false));
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetControlVisible(false));
	}

	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + byUser,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + byUser,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + byUser,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + byUser,SetControlVisible(false));

	ITime * pTime = dynamic_cast<ITime*>(m_pUI->GetIControlbyID(DZPK_SHOW_USER_TIMER + byUser));
	if (pTime != NULL)
	{
		pTime->SetShowMaxNum(uMaxTime);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(true);
		pTime->SetControlVisible(true);
	}

	m_uTime = uMaxTime;

	// �ö�ʱ��Ϊ���Ʋ�������ʹ��
	m_pGameFrame->SetTimer(uTimeID, 1000);
}

void CNewGameClient::ShowTimers(UINT uTimeID, UINT uMaxTime)
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(ITime,uTimeID,SetPLay(false));
		SAFE_CTRL_OPERATE(ITime,uTimeID,SetControlVisible(false));
	}

	ITime * pTime = dynamic_cast<ITime*>(m_pUI->GetIControlbyID(uTimeID));
	if (pTime != NULL)
	{
		pTime->SetShowMaxNum(uMaxTime);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(true);
		pTime->SetControlVisible(true);
		
	}

	// �ö�ʱ��Ϊ���Ʋ�������ʹ��
	m_pGameFrame->SetTimer(uTimeID, 1000 * uMaxTime);
}
//������ҵõ����Ƶ�UI����
void CNewGameClient::OnShowOtherPlayerGetToken(const TToken * pToken)
{
	/*if (pToken != NULL)
	{
		CString str;
		str.Format("Client:������ҵõ�����:%d",pToken->byUser);
		OutputDebugString(str);
	}*/

	SAFE_CTRL_OPERATE(ISlider,DZPK_JSLIDER_BUT, SetControlVisible(false));	//��ʾ�����ᰴť
	SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(false));		//��ʾ��ע��+����ť
	SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(false));		//��ʾ��ע��-����ť
	SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetControlVisible(false));		//��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetControlVisible(false));		//��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetControlVisible(false));		//��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetControlVisible(false));		//���ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(false));		//���ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_ALL_IN_BUT,SetControlVisible(false));	//ȫ�°�ť

	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(false));	// ������ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(false));	// ���ظ�ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(false));// ���ؼ�ע���
	SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(false));	// ���ػ������ͼ


	for (int i = 0; i < 5; i++)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));			// ���ؽ�ť
		SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));	// ����5�����
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_TOKEN + i,SetControlVisible(false)); // �������
	}
}

//ϵͳ�Զ�����Ƭ
void CNewGameClient::OnUserSortOut()
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO   + i, SetControlVisible(false));	// ������עͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + i, SetControlVisible(false));	// ���ؼ�עͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + i, SetControlVisible(false));	// ���ع���ͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO  + i, SetControlVisible(false));	// ���ظ���ͼƬ
	}
}

//�Լ��õ����Ƶ�UI����
void CNewGameClient::OnShowMeGetToken(const TToken * pToken)
{
	m_pTimerSMusic->setAllSoundsPaused(true);
	m_pTimerSMusic->stopAllSounds();
	if (m_bSoundPlay)
	{
		// �ƽ���������
		PlayPublicSound(BT_TRANSFER_TOKEN);
	}

	if (pToken->bNewTurn)
	{
		// һ���е�һ��������, ������в�����־
		memset(m_emLastOperation, ET_UNKNOWN, sizeof(m_emLastOperation));

		// ��ղ����˼�¼ջ
		ClearAllStack();
	}
	else
	{
		// �ж��йܲ���
		if (AutoTrusteeship(pToken))
		{
			// �Զ��йܳɹ�
			return ;
		}
	}

	BYTE byVerbFlag = pToken->byVerbFlag;

	// �����Զ��йܰ�ť�����ְ�����ʾ
	OnShowPdg(SW_HIDE);
	OnAutoTrusteeship(SW_HIDE);

	if (byVerbFlag & UD_VF_BET && !m_bWatch)	// ������ע
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetEnable(true));				// ��ע��ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetEnable(true));				// ���ư�ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetEnable(true));				// ���ư�ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetEnable(true));			// �����ᰴť����
		SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetEnable(true));				// ��ע(+)��ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetEnable(true));				// ��ע(-)��ť����

		SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetControlVisible(true));		// ��ʾ��ע��ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
		SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetControlVisible(true));	// ��ʾ�����ᰴť
		SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(true));		// ��ʾ��ע(+)��ť
		SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(true));		// ��ʾ��ע(-)��ť
		SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(true));		// ��ʾ�������ͼ

		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(true));	// ��ʾ��ע���
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(false));	// ���ظ�ע���
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(false));// ���ؼ�ע���

		ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
		if (pSlider != NULL)
		{
			pSlider->SetCurrentValue(0);
			pSlider->SetControlVisible(true);
		}

#ifdef CLOSE_CARD 
		for (int i = 0; i < 5; i++)
		{
			SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));	// ��ʾ5�����
			SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(false));					// ����ע��ť����
			SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));			// ��ʾ����ע��ť����
		}
#else
		for (int i = 0; i < 5; i++)
		{
			SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(true));	// ��ʾ5�����
			SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(true));					// ����ע��ť����
			SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(true));			// ��ʾ����ע��ť����
		}
#endif
		OnHandleSlidingAxle();
	}
	else
	{
			OutputDebugString("YY:else1");
		__int64 nMoney = m_nBetMoney[GetLastUserStation(m_byMeStation)] - m_nBetMoney[m_byMeStation];
		
		if ((m_iMoneys[m_byMeStation] <= nMoney || m_iMoneys[m_byMeStation] <= pToken->nCallMoney) && !m_bWatch)
		{
			// ֻ��ȫ��
			SAFE_CTRL_OPERATE(IButton,DZPK_ALL_IN_BUT,SetEnable(true));				// ȫ�°�ť����
			SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetEnable(true));				// ���ư�ť����
			SAFE_CTRL_OPERATE(IButton,DZPK_ALL_IN_BUT,SetControlVisible(true));		// ��ʾȫ�°�ť
			SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(true));		// ��ʾ���ư�ť
			SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(false));	// ���ػ������ͼ
		}
		else if (byVerbFlag & UD_VF_CHECK && m_byTokenUser == m_byBigBlind && pToken->bNewTurns && !m_bWatch)
		{
			OutputDebugString("YY:else2");
			if (m_nBetMoney[m_byMeStation] < m_nBetMoney[GetLastUserStation(m_byMeStation)] && m_byMeStation == m_byBigBlind)
			{
				OutputDebugString("YY:else3");
				SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetEnable(true));				// ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetEnable(true));			// �����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetEnable(true));				// ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetEnable(true));				// ��ע(-)��ť����

				SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetControlVisible(true));		// ��ʾ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetControlVisible(true));	// ��ʾ�����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(true));		// ��ʾ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(true));		// ��ʾ��ע(-)��ť����
				SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(true));		// ��ʾ�������ͼ

				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(false));	// ������ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(true));	// ��ʾ��ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(true)); // ��ʾ��ע���

				ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
				if (pSlider != NULL)
				{
					pSlider->SetMaxValue(m_iMoneys[m_byMeStation]);
					pSlider->SetCurrentValue(0);
					pSlider->SetControlVisible(true);
				}

#ifdef CLOSE_CARD 
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(false));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));			// ��ʾ����ע��ť����
				}
#else
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(true));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(true));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(true));			// ��ʾ����ע��ť����
				}
#endif

				// ������Ը�ע�Ľ��, �����ø�ע��ť

				m_nCallMoney = pToken->nCallMoney;

				IButton *pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_CALL_BUT));
				IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_CALL_MONEY));
				if (pButton != NULL)
				{			
					CString strMoney;
					strMoney.Format("%I64d",m_nCallMoney);
					pText->SetText(CA2W(strMoney.GetBuffer()));
					pText->SetFontShowStyle(AlignmentCenter);
					pButton->SetControlVisible(true);
				}
			}
			else
			{
				SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetEnable(true));				// ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetEnable(true));			// �����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetEnable(true));				// ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetEnable(true));				// ��ע(-)��ť����

				SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetControlVisible(true));		// ��ʾ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetControlVisible(true));	// ��ʾ�����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(true));		// ��ʾ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(true));		// ��ʾ��ע(-)��ť����
				SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(true));		// ��ʾ�������ͼ

				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(false));	// ������ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(false));	// ��ʾ��ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(true)); // ��ʾ��ע���

				ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
				if (pSlider != NULL)
				{
					pSlider->SetMaxValue(m_iMoneys[m_byMeStation]);
					pSlider->SetCurrentValue(0);
					pSlider->SetControlVisible(true);
				}

#ifdef CLOSE_CARD 
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(false));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));			// ��ʾ����ע��ť����
				}
#else
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(true));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(true));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(true));			// ��ʾ����ע��ť����
				}
#endif
			}

			OnHandleSlidingAxle();
		}
		else if (byVerbFlag & UD_VF_ADD && !m_bWatch)
		{
			// ������Ը�ע�Ľ��, �����ø�ע��ť
			m_nCallMoney = pToken->nCallMoney;
			if (m_nCallMoney == 0)
			{
				SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetEnable(true));				// ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetEnable(true));			// �����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetEnable(true));				// ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetEnable(true));				// ��ע(-)��ť����

				SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetControlVisible(true));		// ��ʾ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetControlVisible(true));	// ��ʾ�����ᰴť
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(true));		// ��ʾ��ע(+)��ť
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(true));		// ��ʾ��ע(-)��ť
				SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(true));		// ��ʾ�������ͼ

				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(true));	// ��ʾ��ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(false));	// ���ظ�ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(false));// ���ؼ�ע���

				ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
				if (pSlider != NULL)
				{
					pSlider->SetMaxValue(m_iMoneys[m_byMeStation]);
					pSlider->SetCurrentValue(0);
					pSlider->SetControlVisible(true);
				}

#ifdef CLOSE_CARD 
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(false));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));			// ��ʾ����ע��ť����
				}
#else
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(true));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(true));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(true));			// ��ʾ����ע��ť����
				}
#endif
				OnHandleSlidingAxle();

			}
			else
			{
				SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetEnable(true));				// ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetEnable(true));				// ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetEnable(true));				// ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetEnable(true));			// �����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetEnable(true));				// ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetEnable(true));				// ��ע(-)��ť����

				SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetControlVisible(true));		// ��ʾ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetControlVisible(true));		// ��ʾ��ע��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(true));		// ��ʾ���ư�ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetControlVisible(true));	// ��ʾ�����ᰴť����
				SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(true));		// ��ʾ��ע(+)��ť����
				SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(true));		// ��ʾ��ע(-)��ť����
				SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(true));		// ��ʾ�������ͼ

				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(true));	// ������ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(true));	// ��ʾ��ע���
				SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(false));// ��ʾ��ע���


				ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
				if (pSlider != NULL)
				{
					pSlider->SetMaxValue(m_iMoneys[m_byMeStation]);
					pSlider->SetCurrentValue(0);
					pSlider->SetControlVisible(true);
				}

#ifdef CLOSE_CARD 
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(false));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));			// ��ʾ����ע��ť����
				}
#else
				for (int i = 0; i < 5; i++)
				{
					SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(true));	// ��ʾ5�����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetEnable(true));					// ����ע��ť����
					SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(true));			// ��ʾ����ע��ť����
				}
#endif

				// ������Ը�ע�Ľ��, �����ø�ע��ť
				m_nCallMoney = pToken->nCallMoney;

				IButton *pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_CALL_BUT));
				IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_CALL_MONEY));
				if (pButton != NULL)
				{			
					CString strMoney;
					strMoney.Format("%I64d",m_nCallMoney);
					pText->SetText(CA2W(strMoney.GetBuffer()));
					pText->SetFontShowStyle(AlignmentCenter);
					pButton->SetControlVisible(true);
				}

				OnHandleSlidingAxle();
			}
		}

		if (!m_bWatch)
		{
			//��˸����
			FlashWindow();
		}
	}

	return;
}

//�Զ��йܴ���
bool CNewGameClient::AutoTrusteeship(const TToken * pToken)
{
	if (m_bWatch)
	{
		return false;
	}

	if (!m_stackLastStation.empty())
	{
		// ������Ը�ע�Ľ��, �����ø�ע��ť
		m_nCallMoney = 0;

		// ����ϼҲ����Ƿ���, ��������ϼҵĲ�����ע���
		stack<BYTE> skTemp(m_stackLastStation);
		if (!skTemp.empty())
		{
			BYTE bLastStation = skTemp.top();
			skTemp.pop();

			// �����ϼ���ע����ע��ȫ�µ����
			while (bLastStation != 255 && m_emLastOperation[bLastStation] == ET_FOLD)
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
				m_nCallMoney = m_nBetMoney[bLastStation] - m_nBetMoney[m_byMeStation];
			}
			else
			{
				OutputDebugString("Client: AutoTrusteeship ��ע����, ����ջû���ҵ��ϼҲ�����");
			}
		}
		else
		{
			OutputDebugString("Client: AutoTrusteeship ��ע����, ����ջΪ��");
		}

		stack<BYTE> skTemp2(m_stackLastStation);
		BYTE bLastStation = skTemp2.top();
		skTemp2.pop();

		if (bLastStation != 255)
		{
			if (pToken->byVerbFlag & UD_VF_BET/* || pToken->byVerbFlag & UD_VF_CHECK*/)	// ������ע�����
			{
				OutputDebugString("Client: ������ע�����");
				// �ڴ˽����Զ��йܹ������Զ��й������ж�
				if (m_emLastOperation[bLastStation] == ET_CHECK)
				{
					// �Զ�����, �ϼ���������²�
					if (m_bTrusteeship[2] || m_bTrusteeship[3])
					{
						// �����Զ����Ʋ���
						m_bTrusteeship[2] = false;  // (ֻ��һ��)

						OutputDebugString("Client: ִ���Զ����Ʋ���");

						OnHandleCheck();
						return true;
					}
				}
				else if (m_emLastOperation[bLastStation] == ET_FOLD)
				{
					// �Զ�����
					if (m_bTrusteeship[3])
					{
						// �����Զ����Ʋ���
						OutputDebugString("Client: ִ���Զ����Ʋ���");

						OnHandleFolo();
						return true;
					} 
				}
			}
			else
			{
				// �ڴ˽����Զ��йܹ������Զ��й������ж�
				if (m_emLastOperation[bLastStation] == ET_CHECK)
				{
					// �Զ�����, �ϼ���������²�
					if (m_bTrusteeship[2] || m_bTrusteeship[3])
					{
						// �����Զ����Ʋ���
						m_bTrusteeship[2] = false;  // (ֻ��һ��)

						OutputDebugString("Client: ִ���Զ����Ʋ���");

						OnHandleCheck();
						return true;
					}
				}
				// �Զ���ע(ֻ��һ��) �� �Զ����κ�ע(ÿ�ζ���)
				else if (m_bTrusteeship[0] || m_bTrusteeship[1])
				{
					// �ڴ˽����Զ���ע����
					OutputDebugString("Client: ִ���Զ���ע����");
					m_bTrusteeship[0] = false;

					OnHandleCall();
					return  true;
				}
				else if (m_bTrusteeship[3])
				{
					// �Զ�����/����
					if (m_emLastOperation[bLastStation] == ET_FOLD)
					{
						// �����Զ����Ʋ���
						OutputDebugString("Client: ִ���Զ����Ʋ���2");

						OnHandleFolo();
						return true;
					}
				}
			}
		}
		else
		{
			OutputDebugString("Client: Error: �ǵ�һ������, �������˵�ջΪ��");
		}
	}

	return false;
}

//�йܰ�ť��ʾ/����
void CNewGameClient::OnAutoTrusteeship(int nCmdShow)
{
	if (m_bWatch)
	{
		for (int i = 0; i < 5; i++)
		{
			SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));			// ����5�����
		}
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(false));						// ������ע���
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(false));						// ���ظ�ע���
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(false));					// ���ؼ�ע���
		SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(false));						// ���ػ������ͼ

		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(false));					// ��ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(false));				// �����йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));				// ���κ�ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));			// ���ơ������йܰ�ť

		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));			// �򹳸�ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));			// �򹳹����йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));		// �򹳸��κ�ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));	// �򹳹��ơ������йܰ�ť
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		SAFE_CTRL_OPERATE(IText, DZPK_SHOW_FIVE_MONEY + i, SetControlVisible(false));			// ����5�����
	}
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(false));						// ������ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(false));						// ���ظ�ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(false));					// ���ؼ�ע���
	SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP,SetControlVisible(false));						// ���ػ������ͼ

	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(false));					// ��ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(false));				// �����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));				// ���κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));			// ���ơ������йܰ�ť

	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));			// �򹳸�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));			// �򹳹����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));		// �򹳸��κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));	// �򹳹��ơ������йܰ�ť

	// ��ע�йܰ�ť
	if (m_bTrusteeship[0])
	{
		// �򹳸�ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}
	else
	{
		// ��ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}

	// �����κ��йܰ�ť
	if (m_bTrusteeship[1])
	{
		// �򹳸��κ�ע�йܰ�ť 
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}
	else
	{
		// ���κ�ע�йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}

	// �����йܰ�ť
	if (m_bTrusteeship[2])
	{
		// �򹳹����йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}
	else
	{
		// �����йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}

	// ����/�����йܰ�ť
	if (m_bTrusteeship[3])
	{
		// �򹳹��ơ������йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}
	else
	{
		// ���ơ������йܰ�ť
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(nCmdShow));
	}
}

// ������ҵõ����Ƶ�UI����
void CNewGameClient::OnShowPdg(int nCmdShow)
{
	for (int i = 0; i < 9; i++)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_GAME_HELP + i,SetControlVisible(false));	
	}

	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_GAME_HELP + (rand() % 9), SetControlVisible(nCmdShow));	
}

// ˳ʱ��˳��õ��¼�λ��
 BYTE CNewGameClient::GetNextUserStation(BYTE byStation, BOOL bIsAll)
 {
 	int nCount = 0;
 	int iNextStation = (byStation + PLAY_COUNT + 1) % PLAY_COUNT;
 
 	UserInfoForGame User;
 	memset(&User,0,sizeof(User)); 
	//m_pGameFrame->GetUserInfo(iNextStation,User
 
 	// �ƹ���Ч������ѷ��������

	while (m_pGameFrame->GetUserInfo(iNextStation,User) == NULL 
	|| m_bGiveUp[iNextStation] 
	|| (!bIsAll && m_bAllBet[iNextStation]) 
	|| m_iUserStation != TYPE_PLAY_GAME)

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

BYTE CNewGameClient::GetNextUserStations(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	BYTE iNextStation = (byStation + PLAY_COUNT + 1) % PLAY_COUNT;

	UserInfoForGame User;
	memset(&User,0,sizeof(User));
	bool bUser = m_pGameFrame->GetUserInfo(iNextStation,User);

	// �ƹ���Ч������ѷ��������
	while (!bUser && m_iUserStation == TYPE_PLAY_GAME)
	{
		iNextStation = (iNextStation + PLAY_COUNT + 1) % PLAY_COUNT;
		bUser = m_pGameFrame->GetUserInfo(iNextStation,User);
	}

	return iNextStation;
}


//��ʱ��˳��õ��ϼ�λ��
BYTE CNewGameClient::GetLastUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iLastStation = (byStation + PLAY_COUNT - 1) % PLAY_COUNT;

	UserInfoForGame User;
	memset(&User,0,sizeof(User));

	// �ƹ���Ч��� �� �ѷ�������� �� ��;������Ϸ���
	while (m_pGameFrame->GetUserInfo(iLastStation,User) == NULL 
	|| m_bGiveUp[iLastStation] 
	|| (!bIsAll && m_bAllBet[iLastStation]) 
	|| m_iUserStation != TYPE_PLAY_GAME)
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
void CNewGameClient::ClearAllStack()
{
	while (!m_stackLastStation.empty())
	{
		m_stackLastStation.pop();   // �Ƴ�ջ������
	}
}

//�ռ���ҳ������Ҵ���ӿ�
void CNewGameClient::OnShowCollectJotens(BYTE bDeskStation)
{
	BYTE byView = m_pGameFrame->DeskStation2View(bDeskStation);
	IBCInterFace * pFace = NULL;
	POINT pStart, pEnd;

	m_pAction[bDeskStation] = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_CHIP_RECYCLE_MOVE + byView));
	if (m_pAction[bDeskStation] != NULL)
	{
		m_pAction[bDeskStation]->SetControlingCtrlID(DZPK_SHOW_DESKTOP_MONEY_LOGO + byView);

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_SHOW_DESKTOP_MONEY_LOGO + byView));
		if (pFace != NULL)
		{
			pStart.x = pFace->GetGameX();
			pStart.y = pFace->GetGameY();
			m_pAction[bDeskStation]->SetStartP(pStart);
		}

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_CHIP_MONEY_MAP));
		if (pFace != NULL)
		{
			pEnd.x = pFace->GetGameX();
			pEnd.y = pFace->GetGameY();
			m_pAction[bDeskStation]->SetEndP(pEnd);
		}
		m_pAction[bDeskStation]->SetTotalMoveTime(500);
		m_pAction[bDeskStation]->SetPlayState(true);
		m_pAction[bDeskStation]->SetControlVisible(true);
	}

	m_pMoveAction[bDeskStation] = dynamic_cast<IMoveAction*>(m_pUI->GetIControlbyID(DZPK_MONEY_ISSUE_MOVE + byView));
	if (m_pMoveAction[bDeskStation] != NULL)
	{	
		m_pMoveAction[bDeskStation]->SetControlingCtrlID(DZPK_SHOW_DESKTOP_MONEY + byView);

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_SHOW_DESKTOP_MONEY + byView));
		if (pFace != NULL)
		{
			pStart.x = pFace->GetGameX();
			pStart.y = pFace->GetGameY();
			m_pMoveAction[bDeskStation]->SetStartP(pStart);
		}

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_CHIP_MONEY_MAP));
		if (pFace != NULL)
		{
			pEnd.x = pFace->GetGameX();
			pEnd.y = pFace->GetGameY();
			m_pMoveAction[bDeskStation]->SetEndP(pEnd);
		}
		m_pMoveAction[bDeskStation]->SetTotalMoveTime(500);
		m_pMoveAction[bDeskStation]->SetPlayState(true);
		m_pMoveAction[bDeskStation]->SetControlVisible(true);
	}

	m_bTransceiving = true;

	if (m_bSoundPlay)
	{
		// ������Ч
		PlayPublicSound(BT_CHIP_MONEY_MUSIC);
	}
	return ;
}

//�䷢����UI����ӿ�
void CNewGameClient::OnShowDispatchJotens(BYTE bDeskStation,__int64 iMoney)
{
	BYTE byView = m_pGameFrame->DeskStation2View(bDeskStation);
	IBCInterFace * pFace = NULL;
	POINT pStart, pEnd;

	SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_TOTAL_MAP,SetControlVisible(false));
	SAFE_CTRL_OPERATE(INum,DZPK_MONEY_COLLECT,SetNum(0));
	SAFE_CTRL_OPERATE(INum,DZPK_MONEY_COLLECT,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_MAP + byView,SetControlVisible(true));
	CString str;
	str.Format("Client:�䷢������:���λ��:%d,����Ұ䷢���:%I64d",bDeskStation,iMoney);
	OutputDebugString(str);

	INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_MONEY_NUM + byView));
	if (pNum != NULL && iMoney > 0)
	{
		pNum->SetNum(iMoney);
		pNum->SetRankType(0);
		pNum->SetControlVisible(true);
	}

	m_pAction[bDeskStation] = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_CHIP_RECYCLE_MOVE + byView));
	if (m_pAction[bDeskStation] != NULL)
	{
		m_pAction[bDeskStation]->SetControlingCtrlID(DZPK_CHIP_MAP + byView);

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_CHIP_MAP + byView));
		if (pFace != NULL)
		{
			pStart.x = pFace->GetGameX();
			pStart.y = pFace->GetGameY();
			m_pAction[bDeskStation]->SetStartP(pStart);
		}

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_USER_MAP + byView));
		if (pFace != NULL)
		{
			pEnd.x = pFace->GetGameX();
			pEnd.y = pFace->GetGameY();
			m_pAction[bDeskStation]->SetEndP(pEnd);
		}
		m_pAction[bDeskStation]->SetTotalMoveTime(2000);
		m_pAction[bDeskStation]->SetPlayState(true);
		m_pAction[bDeskStation]->SetControlVisible(true);
	}

	m_pMoveAction[bDeskStation] = dynamic_cast<IMoveAction*>(m_pUI->GetIControlbyID(DZPK_MONEY_ISSUE_MOVE + byView));
	if (m_pMoveAction[bDeskStation] != NULL)
	{	
		m_pMoveAction[bDeskStation]->SetControlingCtrlID(DZPK_MONEY_NUM + byView);

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_MONEY_NUM + byView));
		if (pFace != NULL)
		{
			pStart.x = pFace->GetGameX();
			pStart.y = pFace->GetGameY();
			m_pMoveAction[bDeskStation]->SetStartP(pStart);
		}

		pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_USER_MONEY + byView));
		if (pFace != NULL)
		{
			pEnd.x = pFace->GetGameX();
			pEnd.y = pFace->GetGameY();
			m_pMoveAction[bDeskStation]->SetEndP(pEnd);
		}
		m_pMoveAction[bDeskStation]->SetTotalMoveTime(2000);
		m_pMoveAction[bDeskStation]->SetPlayState(true);
		m_pMoveAction[bDeskStation]->SetControlVisible(true);
	}

	m_bTransceiving = false;

	if (m_bSoundPlay)
	{
		// ������Ч
		PlayPublicSound(BT_CHIP_MONEY_MUSIC);
	}

	return ;
}

//��ʾ����������ݴ���
void CNewGameClient::OnShowPots(const TBetPool* pBetPool)
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (pBetPool->iBetPools[i] > 0)
		{
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BALANCE_POT_LOGO + i, SetControlVisible(true));

			IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_BALANCE_POT_MONEY + i));

			if (pText != NULL)
			{
				CString str;
				if (i == 0)
				{
					str.Format("����:%I64d", pBetPool->iBetPools[i]);
				}
				else
				{
					str.Format("�߳�%d:%I64d", i, pBetPool->iBetPools[i]);
				}
				pText->SetText(CA2W(str.GetBuffer()));
				pText->SetControlVisible(true);				
			}
		}
	}
}

// �޸�ע��
bool CNewGameClient::OnLoadSelectionDlg(bool bAuto)
{
	IContain * pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(DZPK_DAI_RU_MONEY_CONTAINER));
	
	if (pContain != NULL/* && (pContain->GetVisible() == 0)*/)
	{
		// ��Ϊ�գ�˵�����ֶ���������ڴ������Զ���ʼ
		m_pGameFrame->SetTimer(IDT_OK_LOAD,m_tagDeskCfg.dz.iUserInMoney * 1000);
		ShowTimers(DZPK_TIME_NUM, m_tagDeskCfg.dz.iUserInMoney);

		CString str;
		str.Format("YY:m_tagDeskCfg.dz.iUserInMoney == %d",m_tagDeskCfg.dz.iUserInMoney);
		OutputDebugString(str);
	}

	UserInfoForGame User;
	memset(&User, 0, sizeof(User));

	m_iMinMoney = m_tagDeskCfg.dz.iSubMinMoney;			 // ��С������
	m_iMaxMoney = m_tagDeskCfg.dz.iSubMaxMoney;			 // ��������
	if (m_pGameFrame->GetUserInfo(m_byMeStation,User))
	{
		m_iCurrentMoney = m_tagDeskCfg.dz.iSubPlanMoney; // ��ǰ׼������Ľ��
		m_iSelfMoney = User.i64Money;					 // �Լ�����ӵ�еĽ��
	}
	m_iMinusMoney = m_tagDeskCfg.dz.iMinusMoney;		 // �޸�ע��Ӽ����

	// ��ֹ��ұ����ң����ڴ�����
	if (m_iSelfMoney == m_iMaxMoney)
	{
		m_iCurrentMoney = m_iSelfMoney;
	}

	__int64 iSelfMoney = 0;
	
	if (m_iCurrentMoney > m_iSelfMoney)
	{
		m_iCurrentMoney = m_iSelfMoney;
	}
	// ��ҽ����ڻ������С������
	if (m_iSelfMoney >= m_iMinMoney)
	{
		iSelfMoney = m_iSelfMoney - m_iCurrentMoney;
	}

	// ��ҽ��������С�����ֱ�ӷ���false
	if (m_iSelfMoney < m_iMinMoney)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SELF_MONEY_BU_ZU,SetControlVisible(true));
		return false;
	}
	else
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_SELF_MONEY_BU_ZU,SetControlVisible(false));
	}

	IText * pText = NULL;
	// �����ܽ����
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_TOTAL_MONEY_TEXT));
	if (pText != NULL)
	{
		CString strToTalMoney;
		strToTalMoney.Format("%I64d",iSelfMoney);
		pText->SetText(CA2W(strToTalMoney));
		pText->SetControlVisible(true);
	}

	// �����������
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_MAX_MONEY_TEXT));
	if (pText != NULL)
	{
		CString strSubMaxMoney;
		strSubMaxMoney.Format("%I64d",m_iMaxMoney);
		pText->SetText(CA2W(strSubMaxMoney));
		pText->SetControlVisible(true);
	}

	// ������С�����
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_MIN_MONEY_TEXT));
	if (pText != NULL)
	{
		CString strSubMinMoney;
		strSubMinMoney.Format("%I64d",m_iMinMoney);
		pText->SetText(CA2W(strSubMinMoney));
		pText->SetControlVisible(true);
	}

	// ��ǰ׼����������
	IEditorText * pEdit = dynamic_cast<IEditorText *>(m_pUI->GetIControlbyID(DZPK_PLAN_MONEY_INPUT_TEXT));
	if (pEdit != NULL)
	{
		// ���ñ༭��ֻ����������
		pEdit->SetEnableASCII(1, '0', '9');
		pEdit->SetNumberLimit(true,1,m_iMaxMoney);
	
		CString strSubPlanMoney;
		strSubPlanMoney.Format("%I64d",m_iCurrentMoney);
		pEdit->SetText(CA2W(strSubPlanMoney));
		pEdit->SetControlVisible(true);	
	}

	// ��ʾ����������
	SAFE_CTRL_OPERATE(IContain,DZPK_DAI_RU_MONEY_CONTAINER,SetControlVisible(true));

	// ���ش����ť
	SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(false));
	
	// ���ؽ����
	SAFE_CTRL_OPERATE(IContain,DZPK_BALANCE_CONTAINER,SetControlVisible(false));

	OnShowDeskTop();
	
	return true;
}

//ȷ��������
void CNewGameClient::OnOKLOAD()
{
	m_pGameFrame->KillTimer(IDT_OK_LOAD);

	// ��ҽ������ʹ�����
	if (m_iSelfMoney < m_iMinMoney)
	{
		OutputDebugString("Client:������������ʹ��뽫����߳�");
		SAFE_CTRL_OPERATE(IImage,DZPK_SELF_MONEY_BU_ZU,SetControlVisible(true));
		m_pGameFrame->SetTimer(IDT_MONEY_LACK, 3000);
		return;
	}

	// ��ǰ������������ʹ�����
	if (m_iCurrentMoney < m_iMinMoney)
	{
		m_iCurrentMoney = m_iMinMoney;
	}

	// ȷ�ϴ���	
	TSubMoney cmd;
	cmd.nMoney[m_byMeStation] = m_iCurrentMoney;
	cmd.bDeskStation = m_byMeStation;

	m_pGameFrame->SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);

	// -----------------------�رն�ʱ�� -----------------------
	ITime *pTime = dynamic_cast<ITime*>(m_pUI->GetIControlbyID(DZPK_SHOW_IN_TIME));
	if (pTime != NULL)
	{
		pTime->SetPLay(false);
		pTime->SetControlVisible(false);
	}

	// ���ش���������
	SAFE_CTRL_OPERATE(IContain,DZPK_DAI_RU_MONEY_CONTAINER,SetControlVisible(false));

	// ���ش������ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_IN_MONEY_BU_ZU,SetControlVisible(false));

	if (m_bSoundPlay)
	{
		// ȷ����ť����
		PlayPublicSound(BT_OK);
	}
}

//��ʾ���Logo
void CNewGameClient::OnShowUserLogo()
{
	UserInfoForGame UserLogo;
	memset(&UserLogo,0,sizeof(UserLogo));

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pGameFrame->GetUserInfo(i,UserLogo))
		{
			BYTE bViewStation =  m_pGameFrame->DeskStation2View(i);
			IImage *pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_USER_LOGO + bViewStation));
			if (pImage)
			{
				UINT LogoId = UserLogo.bLogoID % 8;
				TCHAR path[MAX_PATH];
				CString LeftOrRight; //;

				//�ұ�1��λ�õ��Ǹ�ͷ����һ��
				switch (bViewStation)
				{
				case 3:  LeftOrRight ="left.png";    break;
				case 7:  LeftOrRight ="right.png";   break;
				default: LeftOrRight = "logo.png";   break;	
				}

				//�ұ�1��λ�õ��Ǹ�ͷ����һ��
				if (UserLogo.bBoy)
				{
					wsprintf(path,"resources\\log\\boy_%d\\%s",LogoId,LeftOrRight);
				}
				else
				{
					wsprintf(path,"resources\\log\\girl_%d\\%s",LogoId,LeftOrRight);
				}
				
				CString strInfo ; 
				strInfo.Format("Client:: ����û���%s�����ͷ��ID:%d" , UserLogo.szNickName,LogoId) ; 
				//OutputDebugString(strInfo) ;
				pImage->LoadPic(CA2W(path));
				pImage->SetControlVisible(true);
				//SAFE_CTRL_OPERATE(IContain,DZPK_USER_LOGO_CONTAINER, SetControlVisible(true)); // ��ʾ���ͷ������
				//SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_USER_CHAIR + bViewStation, SetControlVisible(false)); // �����Լ�������
			}		
		}
		else
		{
			CloseAllLogo(i);
		}
	}
}

///�ر��������ͷ��
///@param BYTE bDesk Ҫ�رյ�ͷ��ID��255�����������ʱ��ʾ�ر�����
///@return void
void CNewGameClient::CloseAllLogo(BYTE bDesk)
{
	if (bDesk >= PLAY_COUNT)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			IImage *pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(DZPK_USER_LOGO + i));
			if (pImage != NULL)
			{
				pImage->SetControlVisible(false);
			}
		}
	}
	else
	{
		IImage *pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(DZPK_USER_LOGO + m_pGameFrame->DeskStation2View(bDesk)));
		if (pImage != NULL)
		{
			pImage->SetControlVisible(false);
		}
	}
}

//�ָ���������Ϸ����
void CNewGameClient::RecurSaveBase(const LPCVOID lpData)
{
	const TGSBase* pGSBase = (TGSBase*)lpData;

	BYTE nMeStation = m_pGameFrame->GetMyDeskStation();

	//��ԭ������������
	::memcpy(&m_tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));

	::memcpy(m_bpUserInfo,pGSBase->bHaveUser,sizeof(m_bpUserInfo));
}

// ��ʾ�û���Ϣ
void CNewGameClient::OnShowUserInfo()
{
	UserInfoForGame user;
	::memset(&user,0,sizeof(user));
	IText * pText = NULL;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pGameFrame->GetUserInfo(i, user))
		{
			BYTE byView = m_pGameFrame->DeskStation2View(i);

			pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_USER_NAME + byView));
			if (pText != NULL)
			{
				CString strName;
				strName.Format("%s",user.szNickName);
				pText->SetText(CA2W(strName));
				pText->SetControlVisible(true);
			}

			if (i == m_byMeStation)
			{
				INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_SHOW_REAL_DINAR));
				if (pNum != NULL)
				{
					pNum->SetNum(user.i64Money);
					pNum->SetRankType(0);
					pNum->SetControlVisible(true);
				}
			}

			SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + byView,SetControlVisible(true));
		}
	}
}


// ��ʼ������UI
void CNewGameClient::OnInitYuanSu(bool bShow)
{
	// �û�ͷ������
	SAFE_CTRL_OPERATE(IContain,DZPK_USER_LOGO_CONTAINER,SetControlVisible(true));
	// �û��ǳ�����
	SAFE_CTRL_OPERATE(IContain,DZPK_USER_NAME_CONTAINER,SetControlVisible(true));
	// ��ʾ��������
	SAFE_CTRL_OPERATE(IContain,DZPK_CARD_CONTAINER,SetControlVisible(true));
	// ��ʾ���½Ǳ߳�����
	SAFE_CTRL_OPERATE(IContain,DZPK_SIDE_POT_CONTAINER,SetControlVisible(true));
	// ����������
	SAFE_CTRL_OPERATE(IContain,DZPK_DESKTOP_MONEY_CONTAINER,SetControlVisible(true));
	// �¸��ӹ���ȫ������
	SAFE_CTRL_OPERATE(IContain,DZPK_BET_CALL_RAISE_CHECK_FOLD_ALLIN_CONTAINER,SetControlVisible(true));
	// ����߳�����
	SAFE_CTRL_OPERATE(IContain,DZPK_POT_CONTAINER,SetControlVisible(true));
	// ��������
	SAFE_CTRL_OPERATE(IContain,DZPK_CHAT_CONTAINER,SetControlVisible(true));
	// ׯ��Сäע����
	SAFE_CTRL_OPERATE(IContain,DZPK_NT_DA_XIAO_CONTAINER,SetControlVisible(true));
	// ���׼������
	SAFE_CTRL_OPERATE(IContain,DZPK_ZHUN_BEI_CONTAINER,SetControlVisible(true));
	// С����ʾ����
	SAFE_CTRL_OPERATE(IContain,DZPK_SHOW_SMALL_CARD_CONTAINER,SetControlVisible(true));
	// V������
	SAFE_CTRL_OPERATE(IContain,DZPK_V_PAI_CONTAINER,SetControlVisible(true));
	// ��ע�������
	SAFE_CTRL_OPERATE(IContain,DZPK_SHOW_BET_MONEY_CONTAINER,SetControlVisible(true));
	// ���ʱ������
	SAFE_CTRL_OPERATE(IContain,DZPK_SHOW_USER_TIMER_CONTAINER,SetControlVisible(true));
	// �����������
	SAFE_CTRL_OPERATE(IContain,DZPK_SHOW_TOKEN_CONTAINER,SetControlVisible(true));
	// ���Ͷ�������
	SAFE_CTRL_OPERATE(IContain,DZPK_PAI_TYPE_ANIMATION_CONTAINER,SetControlVisible(true));
	// ʤ����������
	SAFE_CTRL_OPERATE(IContain,DZPK_WIN_DONG_HUA_CONTAINER,SetControlVisible(true));
	// ����ͼƬ����
	SAFE_CTRL_OPERATE(IContain,DZPK_PAI_TYPE_MAP_CONTAINER,SetControlVisible(true));
	// ������հ䷢�ƶ�����
	SAFE_CTRL_OPERATE(IContain,DZPK_CHIP_RECYCLE_MOVE_CONTAINER,SetControlVisible(true));
	// �����հ䷢�ƶ�����
	SAFE_CTRL_OPERATE(IContain,DZPK_MONEY_ISSUE_MOVE_CONTAINER,SetControlVisible(true));
	// ������䷢����
	SAFE_CTRL_OPERATE(IContain,DZPK_CHIP_MONEY_CONTAINER,SetControlVisible(true));
	// ��ҷ���ͷ��ͼƬ����
	SAFE_CTRL_OPERATE(IContain,DZPK_FOLD_LOGO_CONTAINER,SetControlVisible(true));
	// ���ֽ̳�����
	SAFE_CTRL_OPERATE(IContain,DZPK_SHOW_GAME_HELP_CONTAINER,SetControlVisible(true));
	// ׯ��Сäע�ƶ��ռ�����
	SAFE_CTRL_OPERATE(IContain,DZPK_MOVE_COLLECT_CONTAINER,SetControlVisible(true));
	// ׯ��Сä�ƶ�ͼƬ����
	SAFE_CTRL_OPERATE(IContain,DZPK_NT_DA_XIAO_MOVE_CONTAINER,SetControlVisible(true));

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		// �û�ͷ�� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_USER_LOGO + i,SetControlVisible(bShow));
		// �û���Ϣ��ͼ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + i,SetControlVisible(bShow));
		// �û��ǳ� * 8
		SAFE_CTRL_OPERATE(IText,DZPK_USER_NAME + i,SetControlVisible(bShow));
		// �û���� * 8
		SAFE_CTRL_OPERATE(INum,DZPK_USER_MONEY + i,SetControlVisible(bShow));
		// ��ʾ���� * 8
		SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD + i,SetControlVisible(bShow));
		// ��ʾ���½Ǳ߳ؽ�� * 8
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_POT_DINAR + i,SetControlVisible(bShow));
		// ��ʾ���½Ǳ߳� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SIDE_POT + i,SetControlVisible(bShow));
		// ������LOGO * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DESKTOP_MONEY_LOGO + i,SetControlVisible(bShow));
		// ������ * 8
		SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetControlVisible(bShow));
		SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetNum(0));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + i,SetControlVisible(bShow));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + i,SetControlVisible(bShow));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + i,SetControlVisible(bShow));
		// ����Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + i,SetControlVisible(bShow));
		// ����Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_FOLD_LOGO + i,SetControlVisible(bShow));
		// ȫ��Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ALLIN_LOGO + i,SetControlVisible(bShow));
		// ����߳ؽ�����ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BALANCE_POT_LOGO + i,SetControlVisible(bShow));
		// ����߳ؽ�����ʾ��� * 8
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BALANCE_POT_MONEY + i,SetControlVisible(bShow));
		// ����������ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHAT_BUBBLE + i,SetControlVisible(bShow));
		// ��������������ʾ * 8
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CHAT_TYPE + i,SetControlVisible(bShow));
		// ��ʾׯ�� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + i,SetControlVisible(bShow));
		// Сäע��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + i,SetControlVisible(bShow));
		// ��äע��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + i,SetControlVisible(bShow));
		// ���׼��ͼƬ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + i,SetControlVisible(bShow));
		// С����ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_CARD + i,SetControlVisible(bShow));
		// V1�� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI + i,SetControlVisible(bShow));
		// V2�� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI + i,SetControlVisible(bShow));
		// ���ʱ�� * 8
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetControlVisible(bShow));
		// ������� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_TOKEN + i,SetControlVisible(bShow));
		// ���Ͷ��� * 8
		SAFE_CTRL_OPERATE(IExAnimate,DZPK_PAI_TYPE_ANIMATION + i,SetPlayState(bShow));
		SAFE_CTRL_OPERATE(IExAnimate,DZPK_PAI_TYPE_ANIMATION + i,SetControlVisible(bShow));
		// ����ͼƬ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_PAI_TYPE_MAP + i,SetControlVisible(bShow));
		// ������հ䷢�ƶ��ؼ� * 8
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_CHIP_RECYCLE_MOVE + i,SetControlVisible(bShow));
		// �����հ䷢�ƶ��ؼ� * 8
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_MONEY_ISSUE_MOVE + i,SetControlVisible(bShow));
		// ����䷢ͼƬ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_MAP + i,SetControlVisible(bShow));
		// ���䷢���� * 8
		SAFE_CTRL_OPERATE(INum,DZPK_MONEY_NUM + i,SetControlVisible(bShow));
		// ��ҷ���ͷ��ͼƬ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + i,SetControlVisible(bShow));
		// ʤ������ * 8
		SAFE_CTRL_OPERATE(IExAnimate,DZPK_WIN_DONG_HUA + i,SetPlayState(bShow));
		SAFE_CTRL_OPERATE(IExAnimate,DZPK_WIN_DONG_HUA + i,SetControlVisible(bShow));
		
		//ׯ�ƶ��ռ�
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_NT_MOVE_COLLECT + i,SetControlVisible(bShow));
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_NT_MOVE_COLLECT + i,SetPlayState(bShow));

		// Сäע�ƶ��ռ�
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_XIAO_MOVE_COLLECT + i,SetControlVisible(bShow));
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_XIAO_MOVE_COLLECT + i,SetPlayState(bShow));

		// ��äע�ƶ��ռ�
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_DA_MOVE_COLLECT + i,SetControlVisible(bShow));
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_DA_MOVE_COLLECT + i,SetPlayState(bShow));

		// ׯä�ƶ�ͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_NT_MOVE_MAP + i,SetControlVisible(bShow));

		// ��Сä�ƶ�ͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_DA_XIAO_MOVE_MAP + i,SetControlVisible(bShow));
	} 

	for (int i = 0; i < 9; i++)
	{
		// ���ֽ̳� * 9
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_GAME_HELP + i,SetControlVisible(bShow));
	}

	// ��ǰ���ý��
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_KE_YONG_MONEY,SetControlVisible(bShow));
	// ��ʵ�����ʾ
	SAFE_CTRL_OPERATE(INum,DZPK_SHOW_REAL_DINAR,SetControlVisible(bShow));
	// ��ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetControlVisible(bShow));
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BET_MONEY,SetMouseThough(true));
	// ��ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetControlVisible(bShow));
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_CALL_MONEY,SetMouseThough(true));
	// ��ע���
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetControlVisible(bShow));
	SAFE_CTRL_OPERATE(IText,DZPK_SHOW_RAISE_MONEY,SetMouseThough(true));

	SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT,SetControlVisible(bShow));				  // ����׼����ť
	SAFE_CTRL_OPERATE(IButton,DZPK_DAI_RU_MONEY_BUT,SetControlVisible(bShow));				  // �����޸Ľ��
	SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT,SetControlVisible(bShow));						  // ��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT,SetControlVisible(bShow));						  // ��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT,SetControlVisible(bShow));						  // ��ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT,SetControlVisible(bShow));						  // ���ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT,SetControlVisible(bShow));						  // ���ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_ALL_IN_BUT,SetControlVisible(bShow));	    			  // ȫ�°�ť

	SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_BUT,SetControlVisible(bShow));	    			  // ��������ť
	SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY,SetControlVisible(bShow));						  // ��ע��+����ť
	SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY,SetControlVisible(bShow));	        			  // ��ע��-����ť
	SAFE_CTRL_OPERATE(IButton,DZPK_JSLIDER_MAP,SetControlVisible(bShow));	    			  // ��������ͼ

	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(bShow));				  // ��ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(bShow));			  // �����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(bShow));	          // ���κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(bShow));	      // ���ơ������йܰ�ť

	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(bShow));		  // �򹳸�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(bShow));		  // �򹳹����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(bShow));	  // �򹳸��κ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(bShow));  // �򹳹��ơ������йܰ�ť

	for (int i = 0; i < 5; i++)
	{
		// ������
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_FIVE_MONEY + i,SetControlVisible(bShow));
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_FIVE_MONEY + i,SetMouseThough(true));
	}

	// ��ʼ��ׯ����Сäע�ƶ�����
	m_pMove[0] = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_NT_MOVE));
	m_pMove[1] = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_SMALL_BLIND_MOVE));
	m_pMove[2] = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_BIG_BLIND_MOVE));

	if (m_pMove[0] != NULL)
	{
		IBCInterFace* pCInterFace = m_pUI->GetIControlbyID(DZPK_HIDE_NT_BEGIN);
		IBCInterFace* pFace = m_pUI->GetIControlbyID(DZPK_SHOW_NTUSER_IMAGE);
		POINT pt,pt1;
		pt.x = pCInterFace->GetGameX();
		pt.y = pCInterFace->GetGameY();
		pt.x = pCInterFace->GetGameX();
		pt.y = pCInterFace->GetGameY();
		m_pMove[0]->SetGameXY(pt.x,pt.y);
		m_pMove[0]->SetStartP(pt);

		pt1.x = pFace->GetGameX();
		pt1.y = pFace->GetGameY();
		pt1.x = pFace->GetGameX();
		pt1.y = pFace->GetGameY();
		m_pMove[0]->SetGameXY(pt1.x,pt1.y);
		m_pMove[0]->SetStartP(pt1);
	}
	if (m_pMove[1] != NULL)
	{
		IBCInterFace* pCInterFace = m_pUI->GetIControlbyID(DZPK_HIDE_SMALL_BLIND_BEGIN);
		IBCInterFace* pFace = m_pUI->GetIControlbyID(DZPK_SHOW_XIAO_IMAGE);
		POINT pt, pt1;
		pt.x = pCInterFace->GetGameX();
		pt.y = pCInterFace->GetGameY();
		pt.x = pCInterFace->GetGameX();
		pt.y = pCInterFace->GetGameY();
		m_pMove[1]->SetGameXY(pt.x,pt.y);
		m_pMove[1]->SetStartP(pt);

		pt1.x = pFace->GetGameX();
		pt1.y = pFace->GetGameY();
		pt1.x = pFace->GetGameX();
		pt1.y = pFace->GetGameY();
		m_pMove[1]->SetGameXY(pt1.x,pt1.y);
		m_pMove[1]->SetStartP(pt1);
	}
	if (m_pMove[2] != NULL)
	{
		IBCInterFace* pCInterFace = m_pUI->GetIControlbyID(DZPK_HIDE_BIG_BLIND_BEGIN);
		IBCInterFace* pFace = m_pUI->GetIControlbyID(DZPK_SHOW_DA_IMAGE);
		POINT pt,pt1;
		pt.x = pCInterFace->GetGameX();
		pt.y = pCInterFace->GetGameY();
		pt.x = pCInterFace->GetGameX();
		pt.y = pCInterFace->GetGameY();
		m_pMove[2]->SetGameXY(pt.x,pt.y);
		m_pMove[2]->SetStartP(pt);

		pt1.x = pFace->GetGameX();
		pt1.y = pFace->GetGameY();
		pt1.x = pFace->GetGameX();
		pt1.y = pFace->GetGameY();
		m_pMove[2]->SetGameXY(pt1.x,pt1.y);
		m_pMove[2]->SetStartP(pt1);
	}

	IExAnimate * pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_FA_PAI_ANIMATION));
	if (pAnimate != NULL)
	{
		IBCInterFace* pFace = m_pUI->GetIControlbyID(DZPK_VICE_CARD_BEGIN);
		POINT pt;
		pt.x = pFace->GetGameX();
		pt.y = pFace->GetGameY();
		pAnimate->SetGameXY(pt.x,pt.y);
		pAnimate->SetStartP(pt);
	}
}

// ��ʾׯ����Сäע�ı�Ƕ���
void CNewGameClient::OnPlayMoveFlagAnim(BYTE bDeskStation,UINT type,bool bHandout)
{			
	BYTE byView = m_pGameFrame->DeskStation2View(bDeskStation);
	IBCInterFace* pFace = NULL;
	POINT pStart, pEnd;
	IMoveAction * pMoveAction0 = NULL;
	IMoveAction * pMoveAction1 = NULL;
	IMoveAction * pMoveAction2 = NULL;

	if (bHandout)
	{ 
		if (m_pMove[0] != NULL && type == DZPK_SHOW_ZHUANG_LOGO)
		{
			m_pMove[0]->SetControlingCtrlID(DZPK_SHOW_ZHUANG_LOGO);

			pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_HIDE_NT_BEGIN));
			if (pFace != NULL)
			{
				pStart.x = pFace->GetGameX();
				pStart.y = pFace->GetGameY();
				m_pMove[0]->SetStartP(pStart);
			}

			pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_NT_MOVE_MAP + byView));
			if (pFace != NULL)
			{
				pEnd.x = pFace->GetGameX();
				pEnd.y = pFace->GetGameY();
				m_pMove[0]->SetEndP(pEnd);
			}
			m_pMove[0]->SetPlayState(true);
		}

		if (m_pMove[1] != NULL && type == DZPK_SHOW_SMALL_BLIND)
		{
			m_pMove[1]->SetControlingCtrlID(DZPK_SHOW_SMALL_BLIND);

			pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_HIDE_SMALL_BLIND_BEGIN));
			if (pFace != NULL)
			{
				pStart.x = pFace->GetGameX();
				pStart.y = pFace->GetGameY();
				m_pMove[1]->SetStartP(pStart);
			}

			pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_DA_XIAO_MOVE_MAP + byView));
			if (pFace != NULL)
			{
				pEnd.x = pFace->GetGameX();
				pEnd.y = pFace->GetGameY();
				m_pMove[1]->SetEndP(pEnd);
			}
			m_pMove[1]->SetPlayState(true);
		}

		if (m_pMove[2] != NULL && type == DZPK_SHOW_BIG_BLIND)
		{
			m_pMove[2]->SetControlingCtrlID(DZPK_SHOW_BIG_BLIND);

			pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_HIDE_BIG_BLIND_BEGIN));
			if (pFace != NULL)
			{
				pStart.x = pFace->GetGameX();
				pStart.y = pFace->GetGameY();
				m_pMove[2]->SetStartP(pStart);
			}

			pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_DA_XIAO_MOVE_MAP + byView));
			if (pFace != NULL)
			{
				pEnd.x = pFace->GetGameX();
				pEnd.y = pFace->GetGameY();
				m_pMove[2]->SetEndP(pEnd);
			}

			m_pMove[2]->SetPlayState(true);
		}		
	}
	else
	{
		//for (int i = 0; i < PLAY_COUNT; i++)
		{
			pMoveAction0 = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_NT_MOVE_COLLECT + byView));
			if (pMoveAction0 != NULL && type == DZPK_SHOW_ZHUANG_LOGO)
			{
				pMoveAction0->SetControlingCtrlID(DZPK_SHOW_ZHUANG_LOGO);
			
				pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_SHOW_NTUSER_IMAGE + byView));
				if (pFace != NULL)
				{
					pStart.x = pFace->GetGameX();
					pStart.y = pFace->GetGameY();
					pMoveAction0->SetStartP(pStart);
					IImage  *pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_ZHUANG_LOGO));
					pImage->SetGameXY(pStart.x ,pStart.y);

					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ZHUANG_LOGO,SetControlVisible(true));
				}

				pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_HIDE_NT_BEGIN));
				if (pFace != NULL)
				{
					pEnd.x = pFace->GetGameX();
					pEnd.y = pFace->GetGameY();
					pMoveAction0->SetEndP(pEnd);
				}

				pMoveAction0->SetPlayState(true);

				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + m_pGameFrame->DeskStation2View(m_byNTUser),SetControlVisible(false));
				
			}

			pMoveAction1 = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_XIAO_MOVE_COLLECT + byView));
			if (pMoveAction1 != NULL && type == DZPK_SHOW_SMALL_BLIND)
			{
				pMoveAction1->SetControlingCtrlID(DZPK_SHOW_SMALL_BLIND);

				pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_SHOW_XIAO_IMAGE + byView));
				if (pFace != NULL)
				{
					pStart.x = pFace->GetGameX();
					pStart.y = pFace->GetGameY();
					pMoveAction1->SetStartP(pStart);

					IImage  *pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_SMALL_BLIND));
					pImage->SetGameXY(pStart.x ,pStart.y);

					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_BLIND,SetControlVisible(true));
				}

				pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_HIDE_SMALL_BLIND_BEGIN));
				if (pFace != NULL)
				{
					pEnd.x = pFace->GetGameX();
					pEnd.y = pFace->GetGameY();
					pMoveAction1->SetEndP(pEnd);
				}
				pMoveAction1->SetPlayState(true);
				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + m_pGameFrame->DeskStation2View(m_bySmallBlind),SetControlVisible(false));
			}
			
			pMoveAction2 = dynamic_cast<IMoveAction *>(m_pUI->GetIControlbyID(DZPK_DA_MOVE_COLLECT + byView));

			if (pMoveAction2 != NULL && type == DZPK_SHOW_BIG_BLIND)
			{
				pMoveAction2->SetControlingCtrlID(DZPK_SHOW_BIG_BLIND);

				pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_SHOW_DA_IMAGE + byView));
				if (pFace != NULL)
				{
					pStart.x = pFace->GetGameX();
					pStart.y = pFace->GetGameY();
					pMoveAction2->SetStartP(pStart);

					IImage  *pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(DZPK_SHOW_BIG_BLIND));
					pImage->SetGameXY(pStart.x ,pStart.y);

					SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BIG_BLIND,SetControlVisible(true));
				}

				pFace = dynamic_cast<IBCInterFace*>(m_pUI->GetIControlbyID(DZPK_HIDE_BIG_BLIND_BEGIN));
				if (pFace != NULL)
				{
					pEnd.x = pFace->GetGameX();
					pEnd.y = pFace->GetGameY();
					pMoveAction2->SetEndP(pEnd);
				}
				pMoveAction2->SetPlayState(true);

				SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + m_pGameFrame->DeskStation2View(m_byBigBlind),SetControlVisible(false));
			}
		}
	}
}

//�����ע����ť��ӿ�
void CNewGameClient::OnHandleBet()
{
	if (m_byTokenUser == m_byMeStation && m_iUserStation == TYPE_PLAY_GAME)
	{
		OnEndTimer(m_byMeStation);

		if (m_nCurrentMoney > 0)
		{
			TBet tagBet;
			tagBet.nType = ET_BET;
			tagBet.nMoney = m_nCurrentMoney;
			m_pGameFrame->SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);	
		}
	}
	return;
}

// ��Ҹ�ע��ť����ӿ�
void CNewGameClient::OnHandleCall()
{
	if (m_byTokenUser == m_byMeStation && m_iUserStation == TYPE_PLAY_GAME)
	{
		OnEndTimer(m_byMeStation);

		TBet tagBet;
		tagBet.nMoney = m_nCallMoney;
		tagBet.nType = ET_CALL;
		m_pGameFrame->SendGameData(&tagBet,sizeof(tagBet),MDM_GM_GAME_NOTIFY,ASS_BET,0);	
	}
}

//��Ҽ�ע��ť����ӿ�
void CNewGameClient::OnHandleRaise()
{
	if (m_byTokenUser == m_byMeStation && m_iUserStation == TYPE_PLAY_GAME)
	{
		OnEndTimer(m_byMeStation);

		if (m_nCurrentMoney > 0 && m_nCurrentMoney <= m_iMoneys[m_byMeStation])
		{
			//֪ͨͨѶ�๤��
			TBet tagBet;
			tagBet.nType = ET_ADDNOTE;
			tagBet.nMoney = m_nCurrentMoney + m_nCallMoney;  // �����ϼҽ����ͬ��������ټ�m_nCurrentMoney;
			m_pGameFrame->SendGameData(&tagBet,sizeof(tagBet),MDM_GM_GAME_NOTIFY,ASS_BET,0);
		}
	}
	return;
}

//��ҹ��ư�ť����ӿ�
void CNewGameClient::OnHandleCheck()
{
	if (m_byTokenUser == m_byMeStation && m_iUserStation == TYPE_PLAY_GAME)
	{
		OnEndTimer(m_byMeStation);

		//֪ͨͨѶ�๤��
		TBet tagVerb;
		tagVerb.nType = ET_CHECK;
		m_pGameFrame->SendGameData(&tagVerb,sizeof(tagVerb),MDM_GM_GAME_NOTIFY,ASS_BET,0);
	}
}

// ������ư�ť����ӿ�
void  CNewGameClient::OnHandleFolo()
{
	if (m_byTokenUser == m_byMeStation && m_iUserStation == TYPE_PLAY_GAME)
	{
		OnEndTimer(m_byMeStation);

		//֪ͨͨѶ�๤��
		TBet tagVerb;
		tagVerb.nType = ET_FOLD;
		m_pGameFrame->SendGameData(&tagVerb,sizeof(tagVerb),MDM_GM_GAME_NOTIFY,ASS_BET,0);
	}
}


//���ȫ�°�ť����ӿ�
void CNewGameClient::OnHandleAllin()
{
	if (m_byTokenUser == m_byMeStation && m_iUserStation == TYPE_PLAY_GAME)
	{
		OnEndTimer(m_byMeStation);

		//֪ͨͨѶ�๤��
		TBet tagVerb;
		tagVerb.nType = ET_ALLIN;
		m_pGameFrame->SendGameData(&tagVerb,sizeof(tagVerb),MDM_GM_GAME_NOTIFY,ASS_BET,0);
	}
	return;
}

//��ҽ�ť����ӿ�
void CNewGameClient::OnHandleButtonMoney(int nID)
{
	//int nMoney = m_tagDeskCfg.dz.iLower * m_tagDeskCfg.dz.iRoomMultiple * m_tagDeskCfg.dz.iLowers[nID];   // ���㵱ǰ����İ�ť���

	m_nCurrentMoney = m_tagDeskCfg.dz.iLowers[nID];

	if (m_nCurrentMoney > m_iMoneys[m_byMeStation])
	{
		// �����ע��Ҵ����������еĽ�ң�������н�Ҹ�ֵ����ע���
		m_nCurrentMoney = m_iMoneys[m_byMeStation];
	}

	CString strMoney;
	IText * pText = NULL;
	IButton *pButton = NULL;

	//pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
	//if (pText != NULL)
	//{
	//	pText->SetText(NULL);
	//	pText->SetControlVisible(false);
	//}

	//pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_RAISE_MONEY));
	//if (pText != NULL)
	//{
	//	pText->SetText(NULL);
	//	pText->SetControlVisible(false);
	//}


	pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_BET_BUT));
	pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
	if (pButton != NULL && NULL != pText) 
	{
		strMoney.Format("%I64d", m_nCurrentMoney);
		pText->SetFontShowStyle(AlignmentCenter);
		pText->SetText(CA2W(strMoney.GetBuffer()));
	}

	pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_RAISE_BUT));
	pText = dynamic_cast<IText*>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
	if (pButton != NULL  && NULL != pText) 
	{
		strMoney.Format("%I64d", m_nCurrentMoney);
		pText->SetFontShowStyle(AlignmentCenter);
		pText->SetText(CA2W(strMoney.GetBuffer()));	
	}

	ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
	if (pSlider != NULL)
	{
		pSlider->SetCurrentValue(m_nCurrentMoney);
	}
}

//��һ�����ע�ᴦ��ӿ�
void CNewGameClient::OnHandleSlidingAxle()
{
	//֪ͨ������л滭����
	ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
	if (pSlider != NULL)
	{
		__int64 nPercentMoney = m_iMoneys[m_byMeStation];
		__int64 number =  pSlider->GetCurrentValue();

		m_nCurrentMoney = number;

		// �������ؼ��е㲻��׼, ���ܻᳬ��һ����, �����ڴ��жϽ��
		if (m_nCurrentMoney > nPercentMoney)
		{
			m_nCurrentMoney = nPercentMoney;
		}
		else if (m_nCurrentMoney < m_tagDeskCfg.dz.iMinusMoney)
		{
			m_nCurrentMoney = m_tagDeskCfg.dz.iMinusMoney;

			pSlider->SetCurrentValue(m_nCurrentMoney);
		}

		if (nPercentMoney < m_tagDeskCfg.dz.iMinusMoney)
		{
			m_nCurrentMoney = nPercentMoney;
		}

		CString strMoney;
		IText * pText = NULL;
		IButton * pButton =NULL;
		
		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_BET_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
		if (pButton != NULL && pText != NULL) 
		{
			strMoney.Format("%I64d", m_nCurrentMoney);
			pText->SetFontShowStyle(AlignmentCenter);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_RAISE_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_RAISE_MONEY));
		if (pButton != NULL && pText != NULL) 
		{
			strMoney.Format("%I64d", m_nCurrentMoney);
			pText->SetFontShowStyle(AlignmentCenter);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}
	}
}

//��Ҽ�עť����ӿ�
void CNewGameClient::OnHandleAddMinus()
{
	// ֪ͨ������л滭����
	ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
	if (pSlider != NULL)
	{
		// ÿ�μ� m_tagDeskCfg.dz.iMinusMoney
		if ((m_nCurrentMoney + m_tagDeskCfg.dz.iMinusMoney) > m_iMoneys[m_byMeStation])
		{
			m_nCurrentMoney = m_iMoneys[m_byMeStation];
		}
		else
		{
			m_nCurrentMoney += m_tagDeskCfg.dz.iMinusMoney;
		}

		pSlider->SetCurrentValue(m_nCurrentMoney);

		CString strMoney;
		IText * pText = NULL;
		IButton *pButton = NULL;

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_BET_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
		if (pButton != NULL && pText != NULL) 
		{
			strMoney.Format("%I64d", m_nCurrentMoney);
			pText->SetFontShowStyle(AlignmentCenter);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_RAISE_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_RAISE_MONEY));
		if (pButton != NULL) 
		{
			strMoney.Format("%I64d", m_nCurrentMoney);
			pText->SetFontShowStyle(AlignmentCenter);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}
	}
}

//��Ҽ�עť����ӿ�
void CNewGameClient::OnHandleSubMinus()
{
	// ֪ͨ������л滭����
	ISlider * pSlider = dynamic_cast<ISlider *>(m_pUI->GetIControlbyID(DZPK_JSLIDER_BUT));
	if (pSlider != NULL)
	{
		// ÿ�μ�
		if ((m_nCurrentMoney - m_tagDeskCfg.dz.iMinusMoney) < m_tagDeskCfg.dz.iMinusMoney)
		{
			m_nCurrentMoney = m_tagDeskCfg.dz.iMinusMoney;
		}
		else
		{
			m_nCurrentMoney -= m_tagDeskCfg.dz.iMinusMoney;
		}

		pSlider->SetCurrentValue(m_nCurrentMoney);

		CString strMoney;
		IText * pText = NULL;
		IButton *pButton = NULL;

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_BET_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
		if (pButton != NULL && pText  != NULL) 
		{
			strMoney.Format("%I64d", m_nCurrentMoney);
			pText->SetFontShowStyle(AlignmentCenter);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_RAISE_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_RAISE_MONEY));
		if (pButton != NULL && pText  != NULL) 
		{
			strMoney.Format("%I64d", m_nCurrentMoney);
			pText->SetFontShowStyle(AlignmentCenter);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}
	}
}

// ��Ҹ�ע�йܴ���ӿ�
void CNewGameClient::OnHandleCallMandate(bool bFlag)
{
	if (bFlag)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(true));

		// ���������й�ȡ����

		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));

	}
	else
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));
	}

	m_bTrusteeship[0] = bFlag;
	m_bTrusteeship[1] = false;
	m_bTrusteeship[2] = false;
	m_bTrusteeship[3] = false;
}

// ��ҹ����йܴ���ӿ�
void CNewGameClient::OnHandleCheckMandate(bool bFlag)
{
	if (bFlag)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(true));

		// ���������й�ȡ����
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));
	}
	else
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));
	}

	m_bTrusteeship[0] = false;
	m_bTrusteeship[1] = false;
	m_bTrusteeship[2] = bFlag;
	m_bTrusteeship[3] = false;
}

// ��Ҹ��κ�ע�йܴ���ӿ�
void CNewGameClient::OnHandleCallAnyMandate(bool bFlag)
{ 
	if (bFlag)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(true));

		// ���������й�ȡ����
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));
	}
	else
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));
	}

	m_bTrusteeship[0] = false;
	m_bTrusteeship[1] = bFlag;
	m_bTrusteeship[2] = false;
	m_bTrusteeship[3] = false;
}

// ��ҹ��ƻ������йܴ���ӿ�
void CNewGameClient::OnHandlePassAbandonMandate(bool bFlag)
{ 
	if (bFlag)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(true));

		// ���������й�ȡ����

		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT,SetControlVisible(false));
	}
	else
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT,SetControlVisible(false));
	}

	m_bTrusteeship[0] = false;
	m_bTrusteeship[1] = false;
	m_bTrusteeship[2] = false;
	m_bTrusteeship[3] = bFlag;
}

// ���ƶ���
void CNewGameClient::OnFaPaiAnim()
{
	if (m_qSendCards.size() == 0)
	{
		// ���ƽ���, ֪ͨ�������������
		m_pGameFrame->SendGameData(ASS_SEND_CARD_FINISH , NULL , 0);
		return;
	}

	IBCInterFace* pFace = NULL;
	POINT ptStart, ptEnd;
	IExAnimate * pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_FA_PAI_ANIMATION));
	if (pAnimate != NULL)
	{
		pAnimate->SetPlayState(false);

		TSendCardAnimNode* pAnimNode = &m_qSendCards.front();

		pFace = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(DZPK_VICE_CARD_BEGIN));
		if (pFace != NULL)
		{
			// ��ȡ������ʼλ��
			ptStart.x = pFace->GetGameX();
			ptStart.y = pFace->GetGameY();
		}

		pFace = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(DZPK_SHOW_SMALL_CARD + m_pGameFrame->DeskStation2View(pAnimNode->byUser)));
		if (pFace != NULL)
		{
			// ��ȡ��������λ��
			ptEnd.x = pFace->GetGameX();
			ptEnd.y = pFace->GetGameY();
		}
		
		pAnimate->SetStartP(ptStart);
		pAnimate->SetEnd(ptEnd);
		pAnimate->SetPlayState(true);
		pAnimate->SetControlVisible(true);
	}

	if (m_bSoundPlay)
	{
		// ������Ч
		PlayPublicSound(BT_SEND_BACK_CARD);
	}
}

// ֹͣ��ʱ��
void CNewGameClient::OnEndTimer(BYTE bDeskStation)
{
	m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);

	BYTE byView = m_pGameFrame->DeskStation2View(bDeskStation);

	ITime * pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(DZPK_SHOW_USER_TIMER + byView));
	if (pTime != NULL)
	{
		pTime->SetPLay(false);
		pTime->SetControlVisible(false);
	}
}

//����䷢����ӿ�
void CNewGameClient::OnHandleDispatchJetons()
{
	//֪ͨ������л滭����
	//OnRemoveCollectJotens();

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
		
		CString str;
		if (bIsWin)
		{
			str.Format("Client:���λ��:%d,�����Ӯ���:%I64d,",i,m_tagResult.nMoneyEx[i]);
			OutputDebugString(str);

			OnShowDispatchJotens(i,m_tagResult.nMoneyEx[i]);
		}

		for (int i = 0; i < PLAY_COUNT; i++)
		{
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BALANCE_POT_LOGO + i, SetControlVisible(false));		// ���ر߳ؽ���
			SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BALANCE_POT_MONEY + i, SetControlVisible(false));		// ���ر߳ؽ�����ʾ���
		}
	}
	
	m_pGameFrame->SetTimer(IDT_DISPATCH_JETONS,1500);
}

//�غϽ�����ʾ��UI������������Ի���
void CNewGameClient::OnShowRoundResult(const TResult* pResult)
{
	CString str,str1;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (pResult->nbyUserID[i] != NULL && pResult->nBetPools[i] != NULL && pResult->nScore[i] != NULL)
		{
		}
		str.Format("Client - i == %d,Ӯ��ID:%d,������ע��:%I64d,��Ӯ����:%I64d,ʵ����Ӯ���:%I64d,��Ӯ���:%I64d,��ǰÿ����Ҵ�����:%I64d,�Լ��Ľ�Ҹ���:%I64d,��¼Ӯ��:%d,�������:%d",
			i,pResult->nbyUserID[i],pResult->nBetPools[i],pResult->nScore[i],pResult->nMoney[i],pResult->nMoneyEx[i],pResult->nSubMoney[i],pResult->nSelfMoney[i],pResult->bWin[i],pResult->bOrder[i]);
		OutputDebugString(str);
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			str1.Format("Client:i == %d, j == %d,ÿ�������ע����%I64d,Ӯ����ע��:%I64d,Ӯ����ע�ؽ��%I64d",
				i, j, pResult->nUserBet[i][j], pResult->bWinBetPools[i][j], pResult->nWinPoolsMoney[i][j]);
			OutputDebugString(str1);
		}
	}
	
	//������ж�ʱ��
	OnClearTimer();

	//�����й������ְ���
	OnShowPdg(SW_HIDE);
	OnAutoTrusteeship(SW_HIDE);

	UserInfoForGame User;
	::memset(&User,0,sizeof(User));

	for (int i = 0; i < PLAY_COUNT; i++)
	{	
		// ��Ӯ����
		if (m_tagResult.nMoney[i] >= 0 && m_pGameFrame->GetUserInfo(i,User) != NULL && m_tagResult.bWin[i])
		{
			IExAnimate * pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_WIN_DONG_HUA + m_pGameFrame->DeskStation2View(i)));
			if (pAnimate != NULL)
			{
				pAnimate->SetIsLoop(true);
				pAnimate->SetPlayState(true);
				pAnimate->SetControlVisible(true);
				
			}
		}

		SAFE_CTRL_OPERATE(IMoveAction,DZPK_CHIP_RECYCLE_MOVE + i,SetPlayState(false));
		SAFE_CTRL_OPERATE(IMoveAction,DZPK_MONEY_ISSUE_MOVE + i,SetPlayState(false));

		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DESKTOP_MONEY_LOGO + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetControlVisible(false));

		SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_MAP + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(INum,DZPK_MONEY_NUM + i,SetControlVisible(false));
	}

	// ������Ӯ����
	if (m_tagResult.nMoney[m_byMeStation] > 0)
	{	
		if (m_bSoundPlay)
		{
			// Ӯ
			PlayPublicSound(BT_WIN);
		}
	}
	else
	{
		if (m_bSoundPlay)
		{
			// ��
			PlayPublicSound(BT_LOSE);
		}
	}

	CString strMsg, strTemp, strTemp1;

	// ����Ϸ������������������
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		// ֻ��ʾӮ����Ϣ
		if (pResult->nbyUserID[i] > 0)
		{
			strTemp.Format("%d: ", pResult->nbyUserID[i]);
			//strTemp.Format("%s: ", pResult->szName[i]);
			strMsg = strTemp;

			for (int j = 0; j < 8; j++)
			{
				if (pResult->nUserBet[i][j] > 0)
				{
					if (pResult->bWinBetPools[i][j])
					{
						if (j == 0)
						{
							strTemp.Format("����:Ӯ%I64d", pResult->nWinPoolsMoney[i][j]);
						}
						else
						{
							strTemp.Format(" �߳�%d:Ӯ%I64d", j, pResult->nWinPoolsMoney[i][j]);
						}
					}
					else
					{   
						if (j == 0)
						{
							strTemp.Format("����:��%I64d", pResult->nUserBet[i][j]);
						}
						else
						{
							strTemp.Format(" �߳�%d:��%I64d", j, pResult->nUserBet[i][j]);
						}
					}

					strMsg += strTemp;
				}
			}

			// pResult->nMoney: Ϊʵ����Ӯ��Ǯ
			// pResult->nMoneyEx: Ϊʵ����Ӯ��Ǯ+���ص���ע��Ǯ
			// ӮǮʹ��pResult->nMoneyEx���������ʾ, ��Ӯʹ��pResult->nMoney���������ʾ
			strTemp.Format(" �ܼ���Ӯ:%I64d ", pResult->nMoney[i] > 0 ? pResult->nMoneyEx[i] : pResult->nMoney[i]);
			strMsg += strTemp;
			m_pGameFrame->InsertNormalMessage(strMsg.GetBuffer());
		}
	}

	// ���´����Ҽ���ҽ��
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pGameFrame->GetUserInfo(i,User))
		{
			m_iMoneys[i] = pResult->nSubMoney[i];

			CString str;
			str.Format("Client: ���:%d ���´�����:%I64d",  User.dwUserID, m_iMoneys[i]);
			OutputDebugString(str);
			if (m_iMoneys[i] > 0)
			{
				INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_USER_MONEY + m_pGameFrame->DeskStation2View(i)));
				if (pNum != NULL)
				{
					pNum->SetRankType(0);
					pNum->SetNum(m_iMoneys[i]);
					pNum->SetControlVisible(true);
				}
			}
		}
	}

	CString strMoney;
	IText * pText = NULL;

	// ���������ӵ�еĽ��
	if (m_byMeStation != 255 && pResult->nSelfMoney[m_byMeStation] > 0)
	{
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_REAL_MONEY));
		if (pText != NULL)
		{
			strMoney.Format("%I64d",pResult->nSelfMoney[m_byMeStation]);
			pText->SetText(CA2W(strMoney.GetBuffer()));
		}

		INum * pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_SHOW_REAL_DINAR));
		if (pNum != NULL)
		{
			pNum->SetNum(pResult->nSelfMoney[m_byMeStation]);
			pNum->SetRankType(0);
			pNum->SetControlVisible(true);
		}
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (pResult->nMoney[i] > 0)
		{
			for (int j = 0; j < 8; j++)
			{
				if (pResult->nUserBet[i][j] > 0)
				{
					CString str;
					str.Format("Client: ��%d��ע����ע: %I64d", j+1,pResult->nUserBet[i][j]);
					OutputDebugString(str);
				}
			}
		}
	}

	int nCount = 0;
	IImage * pBak = NULL;
	IBCInterFace * pClone = NULL;
	IContain * pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(DZPK_BALANCE_CONTAINER));
	GetControlNode(pImage, IImage, DZPK_BALANCE_ZHONG)->SetControlVisible(false); 
	int  iStartPosX = pImage->GetGameX(); 
	int  iStartPosY = pImage->GetGameY(); 
	
	CString strbOrder;

	BYTE bDeskStation = 255;
	for (int i = 0; i < PLAY_COUNT; i++)
	{	
		if (pResult->bOrder[i] != 255)
		{
			bDeskStation = pResult->bOrder[i];

			strbOrder.Format("Client:bOrder[%d] == %d",i,pResult->bOrder[i]);
			OutputDebugString(strbOrder);
		}
		else 
		{
			continue;
		}

		bool bShowControl  = (pResult->nMoney[bDeskStation] > 0 && bDeskStation != 255); 
	
		// ֻ��ʾӮ����Ϣ
		//if (pResult->nMoney[bDeskStation] > 0)
		{
			pClone = pImage->Clone(DZPK_BALANCE_ZHONG + 5000000 + i);
			pBak = dynamic_cast<IImage *>(pClone);
			pContain->IInsertControl(pBak);
			pBak->SetControlVisible(bShowControl);
			pBak->SetGameXY(pImage->GetGameX(), iStartPosY + nCount *pImage->GetGameH());

			// ��ʾӮ���ǳ�
			pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_WIN_USER_ID + nCount));
			if (pText != NULL)
			{
				//strTemp.Format("%d", pResult->nbyUserID[bDeskStation]);
				strTemp.Format("%s",pResult->szName[bDeskStation]);
				pText->SetFontShowStyle(AlignmentCenter);
				pText->SetText(CA2W(strTemp.GetBuffer()));
				pText->SetControlVisible(bShowControl);
			}

			// ��ʾӮ�Ľ��
			pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_GONG_WIN_MONEY + nCount));

			// 2012-10-12 duanxiaohui alter nMoney�ĳ�nMoneyEx, �����Ӯ������Ҫ������ע�ı�Ǯ
			if (pText != NULL && /*pResult->nMoney[bDeskStation] > 0*/pResult->nMoneyEx[bDeskStation] > 0)
			{
				strTemp.Format("%I64d", /*pResult->nMoney[bDeskStation]*/pResult->nMoneyEx[bDeskStation]);
				pText->SetText(CA2W(strTemp.GetBuffer()));
				pText->SetFontShowStyle(AlignmentCenter);
				pText->SetControlVisible(bShowControl);
			}

			// ��ʾ������ע����Ϣ
			pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_LOSE_WIN_INFO + nCount));
			if (pText != NULL)
			{
				strTemp = "";
				for (int j = 0; j < 8; j++)
				{
					if (pResult->bWinBetPools[bDeskStation][j])
					{
						// �������ע��Ӯ��
						if (j == 0)
						{
							strTemp1.Format("����:%I64d ", /*pResult->nWinPoolsMoney[bDeskStation][j]*/pResult->nBetPools[j]);
							strTemp = strTemp1;
						}
						else
						{
							strTemp1.Format("�߳�%d:%I64d ", j, /*pResult->nWinPoolsMoney[bDeskStation][j]*/pResult->nBetPools[j]);
							strTemp += strTemp1;
						}
					}
				}

				pText->SetText(CA2W(strTemp.GetBuffer()));
				pText->SetFontShowStyle(AlignmentCenter);
				pText->SetControlVisible(bShowControl);
			}	

			if(bShowControl == true)
			{
				nCount++; 
			}
		}

	}

	m_nCount = nCount;
	
	GetControlNode(pImage1, IImage, DZPK_BALANCE_BASE); 
	if (pImage1 != NULL)
	{
		pImage1->SetGameXY(pImage->GetGameX(), iStartPosY + nCount * pImage->GetGameH());
		pImage1->SetControlVisible(true);
	}
	
	// ʤ������������ʱ��
	m_pGameFrame->SetTimer(IDT_WIN_DONG_HUA_TIMER,2000);
	
	// ���������ʾ����
	if (m_iMoneys[m_byMeStation] <= m_tagDeskCfg.dz.iUntenMoney && !m_bWatch)
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_IN_MONEY_BU_ZU, SetControlVisible(true));
	} 
	else
	{
		SAFE_CTRL_OPERATE(IImage,DZPK_IN_MONEY_BU_ZU, SetControlVisible(false));
	}

	UserInfoForGame user;
	memset(&user,0,sizeof(user));
	// ������������ʹ��뽫����߳�
	if (m_pGameFrame->GetUserInfo(m_byMeStation,user) != NULL)
	{
		if (m_tagResult.nSelfMoney[m_byMeStation] < m_tagDeskCfg.dz.iSubMinMoney)
		{
			SAFE_CTRL_OPERATE(IImage,DZPK_SELF_MONEY_BU_ZU,SetControlVisible(true));
			m_pGameFrame->SetTimer(IDT_MONEY_LACK, 500);
		}
	}
}

// �������滷��
void CNewGameClient::OnShowDeskTop()
{
	SAFE_CTRL_OPERATE(IButton,DZPK_BET_BUT, SetControlVisible(false));									// ������ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_BUT, SetControlVisible(false));									// ���ظ�ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_RAISE_BUT, SetControlVisible(false));								// ���ؼ�ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_BUT, SetControlVisible(false));								// ���ع��ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_FOLD_BUT, SetControlVisible(false));									// �������ư�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_ALL_IN_BUT, SetControlVisible(false));								// ����ȫ�°�ť
	SAFE_CTRL_OPERATE(ISlider,DZPK_JSLIDER_BUT, SetControlVisible(false));								// ���ػ�����ע��
	SAFE_CTRL_OPERATE(IButton,DZPK_ADD_MONEY, SetControlVisible(false));								// ���ؼ�ע(+)��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_SUB_MONEY, SetControlVisible(false));								// ���ؼ�ע(-)��ť
	SAFE_CTRL_OPERATE(IImage,DZPK_JSLIDER_MAP, SetControlVisible(false));								// ���ػ�����ͼ

	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_TUO_GUAN_BUT, SetControlVisible(false));						// ���ظ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CALL_ANY_TUO_GUAN_BUT, SetControlVisible(false));					// ���ظ��κ�ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_TUO_GUAN_BUT, SetControlVisible(false));						// ���ع����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_CHECK_FOLD_TUO_GUAN_BUT, SetControlVisible(false));					// ���ع���/�����йܰ�ť

	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_TUO_GUAN_BUT, SetControlVisible(false));					// ���ظ�ע�йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CALL_ANY_TUO_GUAN_BUT, SetControlVisible(false));				// ���ظ��κ�ע��ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_TUO_GUAN_BUT, SetControlVisible(false));				// ���ع����йܰ�ť
	SAFE_CTRL_OPERATE(IButton,DZPK_GOU_QU_CHECK_FOLD_TUO_GUAN_BUT, SetControlVisible(false));			// ���ع���/�����йܰ�ť


	for (int i = 0; i < 5; i++)
	{
		SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT, SetControlVisible(false));		// ���������ť
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{		
		// ���׼��ͼƬ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + i,SetControlVisible(false));
		// С����ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_CARD + i,SetControlVisible(false));
		// V1�� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI + i,SetControlVisible(false));
		// V2�� * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI + i,SetControlVisible(false));
		// ������LOGO * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DESKTOP_MONEY_LOGO + i,SetControlVisible(false));
		// ������ * 8
		SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetNum(0));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BET_LOGO + i,SetControlVisible(false));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CALL_LOGO + i,SetControlVisible(false));
		// ��עLogo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_RAISE_LOGO + i,SetControlVisible(false));
		// ����Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_CHECK_LOGO + i,SetControlVisible(false));
		// ����Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_FOLD_LOGO + i,SetControlVisible(false));
		// ȫ��Logo��ʾ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_ALLIN_LOGO + i,SetControlVisible(false));
		// ����ͼƬ * 8
		SAFE_CTRL_OPERATE(IImage,DZPK_PAI_TYPE_MAP + i,SetControlVisible(false));
		// ����߳ؽ�����ʾ��� * 8
		SAFE_CTRL_OPERATE(IText,DZPK_SHOW_BALANCE_POT_MONEY + i,SetControlVisible(false));
		// ����߳ؽ�����ʾ
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_BALANCE_POT_LOGO + i,SetControlVisible(false));
		// ����
		SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD + i,SetControlVisible(false));
		// ׯ
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_NTUSER_IMAGE + i,SetControlVisible(false));
		// Сäע
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_XIAO_IMAGE + i,SetControlVisible(false));
		// ��äע
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_DA_IMAGE + i,SetControlVisible(false));
		// ��ҷ���ͷ��ͼƬ
		SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + i,SetControlVisible(false));
		// �������
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_TOKEN + i,SetControlVisible(false));

		// �������Ͷ���
		SAFE_CTRL_OPERATE(IExAnimate,DZPK_PAI_TYPE_ANIMATION + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(IExAnimate,DZPK_PAI_TYPE_ANIMATION + i,SetPlayState(false));

		// ���������ҵ��ƿؼ�
		SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD + i,SetCardList(0,0));
		SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD + i,SetControlVisible(false));
		SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_CARD + i,SetDarkCard(0,0));
		SAFE_CTRL_OPERATE(INum,DZPK_SHOW_DESKTOP_MONEY + i,SetNum(0));
	}

	// ������
	SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_BASE_CARD,SetCardList(0,0));
	SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_BASE_CARD,SetControlVisible(false));
	SAFE_CTRL_OPERATE(INoMoveCard,DZPK_SHOW_BASE_CARD,SetDarkCard(0,0));

	// ������ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_CHIP_TOTAL_MAP,SetControlVisible(false));
	// ����ռ���
	SAFE_CTRL_OPERATE(INum,DZPK_MONEY_COLLECT,SetControlVisible(false));
}

//������ж�ʱ��
void CNewGameClient::OnClearTimer()
{
	m_pGameFrame->KillTimer(IDT_MODIFY_MONEY);				// �ȴ�����޸Ĵ�����
	m_pGameFrame->KillTimer(IDT_COLLECT_JETONS);			// �ռ����붯����ʱ��
	m_pGameFrame->KillTimer(IDT_WAIT_DISPATCH_JETONS);		// �ȴ��䷢����
	m_pGameFrame->KillTimer(IDT_DISPATCH_JETONS);			// �䷢���붯����ʱ��
	m_pGameFrame->KillTimer(IDT_HIDE_BALANCE_TIMER);		// ���ؽ����ʱ��
	m_pGameFrame->KillTimer(IDT_IN_MONEY_BOX);				// �����������
	m_pGameFrame->KillTimer(IDT_MONEY_LACK);				// ��ұ�����㶨ʱ��
	m_pGameFrame->KillTimer(IDT_WIN_DONG_HUA_TIMER);		// ʤ��������ʱ��
	m_pGameFrame->KillTimer(IDT_OK_LOAD);					// ȷ�����붨ʱ��
	m_pGameFrame->KillTimer(IDT_AOUT_IN);					// �Զ�������
	m_pGameFrame->KillTimer(DZPK_SHOW_CLCKTIME);			

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetPLay(false));
		SAFE_CTRL_OPERATE(ITime,DZPK_SHOW_USER_TIMER + i,SetControlVisible(false));
	}
	
	m_pTimerSMusic->setAllSoundsPaused(true);
	m_pTimerSMusic->stopAllSounds();
}

// ���������Ϸ��Ϣ
int CNewGameClient::UserSetGameInfo(bool bSoundEnable,bool bWatchEnable,bool bShow)
{
	if(m_bWatch)
	{
		return 0; 
	}

	IRadioButton *pRadioBtnVoice = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(DZPK_MUSIC_SELECT));
	IRadioButton *pRadioBtnWatch = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(DZPK_KE_WATCH));
	if(NULL != pRadioBtnVoice && pRadioBtnWatch != NULL)
	{
		pRadioBtnVoice->SetControlVisible(true);
		pRadioBtnVoice->SetIsSelect(bSoundEnable);

		pRadioBtnWatch->SetControlVisible(true);
		pRadioBtnWatch->SetIsSelect(bWatchEnable) ; 
		
	}	
	// ��ʾ���ÿ�
	SAFE_CTRL_OPERATE(IContain,DZPK_SET_CONTAINER,SetControlVisible(bShow));
	return true;
}

// ���ű�������
void CNewGameClient::PlayBeiJingSound()
{
	if(!m_pSoundEngine)
	{
		return ;
	}

	TCHAR music_file_name[MAX_PATH];
	memset(music_file_name, 0, sizeof(TCHAR)*MAX_PATH);

	wsprintf(music_file_name, TEXT("Sound\\Base\\����BGM.ogg"));

	if (m_pSoundEngine)
	{
		m_pSoundEngine->play2D(music_file_name, true);
	}
}

// ������������
void CNewGameClient::PlayTimerSound()
{
	if(!m_pTimerSMusic)
	{
		return ;
	}

	TCHAR music_file_name[MAX_PATH];
	memset(music_file_name, 0, sizeof(TCHAR)*MAX_PATH);

	wsprintf(music_file_name, TEXT("Sound\\Base\\����.ogg"));

	if (m_pTimerSMusic)
	{
		m_pTimerSMusic->play2D(music_file_name, true);
	}
}


// ������Ϸ��ע����
void CNewGameClient::PlayBetSound(BYTE bDeskStation, int SoundID)
{
	if(!m_pBetSMusic)
	{
		return ;
	}

	CString folder,filename;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)); 
	srand((unsigned)time(NULL));
	
	folder.Format("Sound\\Speak\\");


	CString strSex ; 
	UserInfoForGame user;
	::memset(&user,0,sizeof(user));
	if (m_pGameFrame->GetUserInfo(bDeskStation,user))
	{
		strSex = user.bBoy  ? "male" : "female";
	}

	switch (SoundID)
	{
	case ST_CALL:	// ��ע����
		{
			filename.Format("%s\\Call\\%d.ogg",strSex,rand() % 4 + 1);
		}
		break;
	case ST_FOLD:	// ��������
		{
			filename.Format("%s\\Fold\\%d.ogg",strSex,rand() % 4 + 1);
		}
		break;
	case ST_CHECK:	// ��������
		{
			filename.Format("%s\\Check\\%d.ogg",strSex,rand() % 4 + 1);
		}
		break;
	case ST_RAISE:	// ��ע����
		{
			filename.Format("%s\\Raise\\%d.ogg",strSex,rand() % 4 + 1);
		}
		break;
	case ST_ALL_IN:	// ȫ������
		{
			filename.Format("%s\\All\\%d.ogg",strSex, rand() % 4 + 1);
		}
		break;
	default:{break;}
	}

	wsprintf(szFileName,"%s%s",folder,filename);
	m_pBetSMusic->play2D(szFileName,false);
}

// ������Ϸ��������
void CNewGameClient::PlayPublicSound(int SoundID)
{
	if(!m_pSEBkMusic)
	{
		return ;
	}
	if(false == m_bSoundPlay)
	{
		return ;
	}

	CString folder,filename;
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)); 
	folder.Format("Sound/Base/"); 

	switch (SoundID)
	{
	case BT_SEND_BACK_CARD:	  // ����������
		{
			filename = "����.ogg";
		}
		break;
	case BT_FLOP_ROUND:		  // ���ƹ�������
		{
			filename = "Flop����.ogg";
		}
		break;
	case BT_TURN_ROUND:		  // ת�ƹ�������
		{
			filename = "Turnת��.ogg";
		}
		break;
	case BT_RIVER_ROUND:	  // ���ƹ�������
		{
			filename = "River����.ogg";
		}
		break;
	case BT_FLOP_PUBLIC_CARD: // ������������
		{
			filename = "�м䷭��.ogg";
		}
		break;
	case BT_FLOP_BACK_CARD:	 // ����������
		{
			filename = "����.ogg";
		}
		break;
	case BT_WIN:			 // ʤ������
		{
			filename = "ʤ��.ogg";
		}
		break;
	case BT_LOSE:			 // ʧ������
		{
			filename = "ʧ��.ogg";
		}
		break;
	case BT_FOLD_MUSIC:		 // ��������
		{
			filename = "����.ogg";
		}
		break;
	case BT_CHIP_MUSIC:		 // ��������
		{
			filename = "����.ogg";
		}
		break;
	case BT_TRANSFER_TOKEN:	 // �ƽ���������
		{
			filename = "��ʾ��˭.ogg";
		}
		break;
	case BT_OK:				 // ȷ����ť����
		{
			filename = "ȷ����ť.ogg";
		}
		break;
	case BT_MONEY_BUTTON:	 // ���ѡ������
		{
			filename = "���ѡ��.ogg";
		}
		break;
	case BT_CHIP_MONEY_MUSIC:// ������Ч
		{
			filename = "������Ч.ogg";
		}
		break;
	default:{break;}
	}

	wsprintf(szFileName,"%s%s",folder,filename);
	m_pSEBkMusic->play2D(szFileName,false);
}

// ��Ϸ����
void CNewGameClient::FlashWindow()
{
	HWND hwnd = (m_pUI->GetWindowsHwnd());
	FLASHWINFO info;
	info.cbSize = sizeof(FLASHWINFO);
	info.dwFlags = FLASHW_TRAY|FLASHW_TIMERNOFG;
	info.hwnd = hwnd;
	info.uCount = 100;
	info.dwTimeout = 500;
	FlashWindowEx(&info);
}

//ϵͳ�����������ݺ��UI������Ҫ��ʾ��Ϸ����
void CNewGameClient::OnShowSysDeskCfg(const TDeskCfg* pDeskCfg)
{
	if (m_bWatch)
	{
		for (int i = 0; i < 5; i++)
		{
			SAFE_CTRL_OPERATE(IButton,DZPK_MONEY_BUT + i,SetControlVisible(false));	  // �����ť
		}
		return;
	}

	for (int i = 0; i < 5; ++i)
	{
		IButton* pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_MONEY_BUT + i));
		IText * pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_FIVE_MONEY + i));
		if (pButton != NULL && pText != NULL)
		{
			CString strMoney;
			strMoney.Format("%I64d",pDeskCfg->dz.iLowers[i]);
			pButton->SetText(CA2W(strMoney));
			pButton->SetControlVisible(false);
		}
	}
}

//���´�������Ϣ
void CNewGameClient::OnShowSubMoney(const TSubMoney * pSubMoney)
{
	UserInfoForGame User;
	memset(&User,0,sizeof(User));
	INum * pNum = NULL;

	memcpy(m_iMoneys, pSubMoney->nMoney, sizeof(m_iMoneys));	
	if (pSubMoney->bDeskStation == m_byMeStation && !m_bWatch)
	{
		m_pGameFrame->KillTimer(IDT_HIDE_BALANCE_TIMER);
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_READY_GO,SetControlVisible(true));		// ��ʾ��׼��ͼƬ
		SAFE_CTRL_OPERATE(IButton,DZPK_GAME_BEGIN_BUT,SetControlVisible(false));	// ���ؿ�ʼ��Ϸ��ť
		m_bAutoStartTimer = true;
	}
	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		bool bUser = m_pGameFrame->GetUserInfo(i,User);
		if (bUser)
		{
			CString str;
			str.Format("dxh: Client %d λ�ô�����: %d", i, pSubMoney->nMoney[i]);
			OutputDebugString(str);
		}

		if (bUser && /*m_iMoneys[i] > 0*/pSubMoney->bIn[i])
		{
			BYTE byUserView = m_pGameFrame->DeskStation2View(i);

			pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(DZPK_USER_MONEY + byUserView));
			if (pNum != NULL)
			{
				pNum->SetNum(m_iMoneys[i]);
				pNum->SetRankType(0);
				pNum->SetControlVisible(true);
			}
			else
			{
				pNum->SetControlVisible(false);
			}

			// ��ʾ���Logo��ʾ * 8
			SAFE_CTRL_OPERATE(IImage, DZPK_USER_MAP + byUserView, SetControlVisible(true));
			// ��ʾ׼��ͼƬ
			SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + byUserView, SetControlVisible(true));
			// ������ʾ�������
			SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_PROMPT, SetControlVisible(false));
		}
	}

	// ��ʾ��ǰ���ý��ͼƬ
	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_KE_YONG_MONEY,SetControlVisible(true));
}

//������Ϣ����
void CNewGameClient::OnShowGetToken(const TToken* pToken, BYTE byTime)
{
	//��������
	m_byTokenUser = pToken->byUser;

	//֪ͨ������л滭����
	if (m_byMeStation == pToken->byUser)
	{
		// ��ǰ��ע��
		m_nCurrentMoney = 0;

		// ��հ�ť�ϵ�����
		IText * pText = NULL;
		IButton *pButton  = NULL;

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_BET_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_BET_MONEY));
		if (pButton != NULL) 
		{
			pText->SetText(NULL);
			pText->SetFontShowStyle(AlignmentCenter);
		}

		pButton = dynamic_cast<IButton*>(m_pUI->GetIControlbyID(DZPK_RAISE_BUT));
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(DZPK_SHOW_RAISE_MONEY));
		if (pButton != NULL) 
		{
			pText->SetText(NULL);
			pText->SetFontShowStyle(AlignmentCenter);
		}

		// �Լ��õ������
		OnShowMeGetToken(pToken);

		// ������Ҷ�ʱ��
		if (byTime == 0)
		{
			ShowTimer(pToken->byUser,DZPK_SHOW_CLCKTIME,m_tagDeskCfg.Time.byOperate);
		}
		else
		{
			ShowTimer(pToken->byUser,DZPK_SHOW_CLCKTIME,byTime);
		}

		BYTE byView = m_pGameFrame->DeskStation2View(pToken->byUser);
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_TOKEN + byView,SetControlVisible(true));	 // �������
	}
	else
	{
		// û�õ���������
		OnShowOtherPlayerGetToken(pToken);

		// ������Ҷ�ʱ��
		if (byTime == 0)
		{
			ShowTimer(pToken->byUser,DZPK_SHOW_CLCKTIME,m_tagDeskCfg.Time.byOperate);
		}
		else
		{
			ShowTimer(pToken->byUser,DZPK_SHOW_CLCKTIME,byTime);
		}

		BYTE byView = m_pGameFrame->DeskStation2View(pToken->byUser);
		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_TOKEN + byView,SetControlVisible(true));	 // �������

		if (m_bGiveUp[m_byMeStation] || m_bAllBet[m_byMeStation])
		{
			// ȫ�»�������ֻ��ʾ���ֽ̳�
			OnAutoTrusteeship(SW_HIDE);
			OnShowPdg(SW_SHOW);
		}

		int nbyUser = GetNextUserStation(m_byTokenUser);
		int iUser = GetNextUserStation(nbyUser);

		if (nbyUser == m_byMeStation || iUser == m_byMeStation)
		{
			OnShowPdg(SW_HIDE);
			OnAutoTrusteeship(SW_SHOW);
		}
		else
		{
			OnAutoTrusteeship(SW_HIDE);
			OnShowPdg(SW_SHOW);
		}
	}
}

#ifdef CLOSE_CARD
// ����Լ��ĵ���
void CNewGameClient::OnHitMySelfBackCard()
{

	INoMoveCard * pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD+4));
	if (pCard != NULL)
	{
		BYTE byCardList[2] = {0};
		int iCardCount = 0;
		iCardCount = pCard->GetCardList(byCardList);

		if(byCardList[0] != 0)
		{
			return;
		}

		pCard->SetCardList(m_iUserDataCard,m_iCardCounts);

		m_pGameFrame->SetTimer(IDT_LOOK_CARD,1000);
	}

	if(GetGameStatus() < GS_WAIT_NEXT_ROUND)
	{
		m_pGameFrame->SendGameData(ASS_LOOK_CARD,NULL,0);	
	}
}

//�������ҵ���ʱ �ƶ���ҵ���
void CNewGameClient::OnHandleMovePlayerCard(BYTE * buffer,int nLen)
{
	if(sizeof(UserLookCardStruct) != nLen)
	{
		return;
	}

	UserLookCardStruct *pLookCard = (UserLookCardStruct*)buffer;

	BYTE byViewStation = m_pGameFrame->DeskStation2View(pLookCard->bDeskStation);	//��ͼλ��

	//�Լ�����ͼλ�ò����ſ��ƶ���
	if(byViewStation == m_pGameFrame->DeskStation2View(m_pGameFrame->GetMyDeskStation()))
	{
		return;
	}

	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_CARD+byViewStation,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI+byViewStation,SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI+byViewStation,SetControlVisible(false));


	///�ؼ����ƶ���
	IExAnimate *pAnima = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_LOOK_CARD_ANIMA_0 + byViewStation));
	if(NULL != pAnima)
	{
		pAnima->SetIsLoop(false);				//��ѭ��
		pAnima->SetPlayTotalTime(400);			//���ò���ʱ��
		pAnima->SetPlayState(true);				//���ò���
		pAnima->SetIsSymmetrical(false);		//�����Ƿ�Գ�
		pAnima->SetControlVisible(true);
	}
}

//���չʾ�ư�ť
void CNewGameClient::OnHitShowCard()
{
	//�Լ������� ֱ�ӷ���
	if(m_bGiveUp[m_pGameFrame->GetMyDeskStation()])
	{
		return;
	}

	SAFE_CTRL_OPERATE(IButton,DZPK_SHOW_CARD_BUT,SetControlVisible(false));	//����չʾ�ư�ť

	m_pGameFrame->SendGameData(ASS_SHOW_CARD,NULL,0);	//����չʾ����Ϣ
}
#endif
