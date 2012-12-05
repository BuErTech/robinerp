#include "stdafx.h"
#include "MJFrameLogic.h"

/** ���캯�� */
CMJFrameLogic::CMJFrameLogic(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_pSoundEngine = createIrrKlangDevice();

	memset(&m_tagSetting, 0, sizeof(m_tagSetting));
	memset(&m_bReady, 0, sizeof(m_bReady));
	memset(&m_bTuoGuan, 0, sizeof(m_bTuoGuan));
	memset(&m_byDice, 0, sizeof(m_byDice));
	memset(&m_nTempMoney, 0, sizeof(m_nTempMoney));

	m_nTimeOutCount = 0;
	m_byGangCount = 0;
	m_byDiceFinishCount = 0;
	m_byDealer = -1;
	m_byMyStation = -1;
	m_byTurnStation = -1;
	m_byGameState = MJ_STATE_NOSTART;
	m_byStateSelect = MJ_STATE_SELECT_NONE;
	m_enDiceState = En_Dice_None;
	m_TilesGod.ReleaseAll();
	m_TilesWall.ReleaseAll();
	
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		m_TilesHand[i].ReleaseAll();
		m_TilesGive[i].ReleaseAll();
		m_TilesSet[i].ReleaseAll();
		m_TilesFlower[i].ReleaseAll();
	}
}

/** �������� */
CMJFrameLogic::~CMJFrameLogic()
{
}

/** 
 * @brief ��ʼ��
 */
int CMJFrameLogic::Initial()
{
	// ����UI
	m_pUI->LoadData(L"UIGameComm.dat"); 

	// ��ȡ��������
	CString s = CBcfFile::GetAppPath (); 
	CBcfFile f(s + SKIN_FOLDER +".bcf");
	CString key = TEXT("Operate");
	m_bSound = f.GetKeyVal(key, "Sound", TRUE);

	SAFE_CTRL_OPERATE(IButton, CTN_BTN_OpenSound, SetControlVisible(m_bSound));   // �����ְ�ť
	SAFE_CTRL_OPERATE(IButton, CTN_BTN_CloseSound, SetControlVisible(!m_bSound)); // �ر����ְ�ť

	// ���ű�����Ч
	PlayBackSound();

	// ������Ϸ��Ч
	LoadSound();

	return 0;
}

/** 
 * @brief ��ʼ��UI
 */
int CMJFrameLogic::InitUI()
{
	memset(&m_bReady, 0, sizeof(m_bReady));

	// ��ʾ�����Ϣ����
	SAFE_CTRL_OPERATE(IContain, CTN_GameInfo, SetControlVisible(true));
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		DrawUserInfo(i, m_tmpUserInfo, false);
		DrawReady(i, false);
	}

	// ����׼����ť
	SAFE_CTRL_OPERATE(IButton, CTN_BTN_Start, SetControlVisible(false));

	// ����GM����ؼ�
	SAFE_CTRL_OPERATE(IEditorText, CTN_EDITORTEXT_GM, SetControlVisible(false));

	InitData();

	return 0;
}

/** 
 * @brief �Զ����ʹ������
 */
void CMJFrameLogic::InitData()
{
	memset(&m_bTuoGuan, 0, sizeof(m_bTuoGuan));
	memset(&m_byDice, 0, sizeof(m_byDice));
	memset(&m_nTempMoney, 0, sizeof(m_nTempMoney));
	m_nTimeOutCount = 0;
	m_byGangCount = 0;
	m_byDiceFinishCount = 0;
	m_byDealer = -1;
	m_byTurnStation = -1;
	m_byGameState = MJ_STATE_NOSTART;
	m_byStateSelect = MJ_STATE_SELECT_NONE;
	m_enDiceState = En_Dice_None;
	m_TilesGod.ReleaseAll();
	m_TilesWall.ReleaseAll();
	m_TilesWall.CurrentLength(m_tagSetting.nTilesNumber);

	SAFE_CTRL_OPERATE(IButton, CTN_BTN_TuoGuan, SetControlVisible(true));
	SAFE_CTRL_OPERATE(IButton, CTN_BTN_Cancel_TuoGuan, SetControlVisible(false));
	SAFE_CTRL_OPERATE(IImage, CTN_IMG_LANZHUANG, SetControlVisible(false))
	
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		m_TilesHand[i].ReleaseAll();
		m_TilesGive[i].ReleaseAll();
		m_TilesSet[i].ReleaseAll();
		m_TilesFlower[i].ReleaseAll();

		// ȡ�����õ��ƿؼ�
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_HandCtrl_0 + i, SetInvalidTile(NULL, 0));
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_HandCtrl_0 + i, SetGodTile(NULL, 0));

		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_GiveCtrl_0 + i, SetGodTile(NULL, 0));

		SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_MJCtrl_WallCtrl_0 + i, FillWallBack(MJ_WALL_NUMBER));
		SAFE_CTRL_OPERATE(IMjWallCtrl, CTN_MJCtrl_WallCtrl_0 + i, SetMaxLength(MJ_WALL_NUMBER));  // SetMaxLength���������������, ��Ȼ�����齫λ�û����߿�ʼ
	}


	// ��ʾ�齫����
	SAFE_CTRL_OPERATE(IContain, CTN_MJCtrl, SetControlVisible(true));
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		DrawHand(i, m_TilesHand[i]);
		DrawGive(i, m_TilesGive[i]);
		DrawSet(i, m_TilesSet[i]);

		DrawTuoGuan(i, false);
		DrawDealer(i, false);
	}

	// ��ʾ��ǽ
	DrawWall(true);

	// �������ƿ�
	DrawBlock(0);

	// ���ؽ����
	DrawFinish(NULL, false);

	// ����С��
	DrawSmallHand(0, false);

	// ���س��Ʊ��
	DrawGiveAnimate(0, false);

	// ����������Ϣ
	OnBlockMoveSelect(TILE_BEGIN);
}

/** 
 * @brief ������Ч��Դ
 */
void CMJFrameLogic::LoadSound()
{
	CString s = CINIFile::GetAppPath (); // ����·��   
	s = s + TEXT("/music/sound.ini");

	char m_pchar[MAX_PATH] = {0};
	wchar_t wszWho[MAX_PATH] = L"";

	int i, j, num = 0;
	CINIFile file(s);
	CString strSec = TEXT("Normal");
	CString value = "";

	
	// ��(1~9)
	for (i = 1; i < 10; i++)
	{
		SOUND_INFO info;
		int &nCount = info.nCount;
		nCount = 0;

		for (j = 0; j < MAX_SOUND_RAND; j++)
		{
			value.Format("%dwan%d", i, j);
			CString strVal = file.GetKeyVal(strSec, value, TEXT("NULL"));
			if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL"))) == 0)
			{
				break;
			}
			_tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
		}        
		m_SoundMap[i] = info;
	}

	// ��(11~19)
	for (i = 1; i < 10; i++)
	{
		SOUND_INFO info;
		int &nCount = info.nCount;
		nCount = 0;

		for (j = 0; j < MAX_SOUND_RAND; j++)
		{
			value.Format("%dtiao%d", i, j);
			CString strVal = file.GetKeyVal(strSec, value, TEXT("NULL"));
			if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL"))) == 0)
			{
				break;
			}
			_tcscpy(info.szFileName[nCount++], strVal.GetBuffer());  
		}        
		m_SoundMap[10 + i] = info;
	}

	// Ͳ(21~29)
	for (i = 1; i < 10; i++)
	{
		SOUND_INFO info;
		int &nCount = info.nCount;
		nCount = 0;
		for (j = 0; j < MAX_SOUND_RAND; j++)
		{
			value.Format("%dtong%d", i, j);
			CString strVal = file.GetKeyVal(strSec, value, TEXT("NULL"));
			if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL")))==0)
			{
				break;
			}
			_tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
		}        
		m_SoundMap[20 + i] = info;
	}

	// ���������з���(31~37)
	CString strKey[7] = {TEXT("dong"), TEXT("nan"), TEXT("xi"), TEXT("bei"), TEXT("zhong"), TEXT("fa"),TEXT("bai")};
	for (i = 0; i < 7; i++)
	{
		SOUND_INFO info;
		int &nCount = info.nCount;
		nCount = 0;
		for (j = 0; j < MAX_SOUND_RAND; j++)
		{
			CString st="";
			st.Format("%d", j);
			value = strKey[i] + st;

			CString strVal = file.GetKeyVal(strSec, value, TEXT("NULL"));
			if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL"))) == 0)
			{
				break;
			}
			_tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
		}        
		m_SoundMap[31 + i] = info;
	}

	// �ԡ������ܡ���������������(100~105)
	CString strKeyBlock[6] = {TEXT("chi"), TEXT("peng"), TEXT("gang"), TEXT("ting"), TEXT("Zimo"), TEXT("DianPao")};
	for (i = 0; i < 6; i++)
	{
		SOUND_INFO info;
		int &nCount = info.nCount;
		nCount = 0;
		for (j = 0; j < MAX_SOUND_RAND; j++)
		{
			CString st="";
			st.Format("%d", j);
			value = strKeyBlock[i] + st;
			CString strVal = file.GetKeyVal(strSec, value, TEXT("NULL"));
			if (_tcscmp(strVal.GetBuffer(), (TEXT("NULL"))) == 0)
			{
				break;
			}
			_tcscpy(info.szFileName[nCount++], strVal.GetBuffer());
		}        
		m_SoundMap[SOUND_CHI + i] = info;
	}
}

/** 
 * @brief ��Ϸ��Ϣ
 * @param [in] nMainID ��Ϸ��ID
 * @param [in] nSubId  ��Ϸ��ID
 * @param [in] buffer  ��Ϸ���ݰ�
 * @param [in] nLen    ��Ϸ���ݰ��ĳ���
 * @return 
 */
int CMJFrameLogic::HandleGameMessage(WORD nMainID, WORD nSubId, BYTE * buffer, int nLen)
{
	if (nSubId == ASS_GM_AGREE_GAME/*nSubId == ASS_SUB_USERAGREE*/)
	{
		if (nLen != sizeof(MSG_GR_S_Agree))
		{
			return 0;
		}

		// �����ͬ����Ϸ
		MSG_GR_S_Agree * pCmd = (MSG_GR_S_Agree *)buffer;
		if (pCmd == NULL)
		{
			return 0;
		}

		if (m_byGameState == MJ_STATE_NONE || m_byGameState == MJ_STATE_NOSTART || m_byGameState == MJ_STATE_END)
		{
			// ��ʾ׼����Ϣ
			DrawReady(pCmd->bDeskStation, true);

			// ����׼����Ч
			if (pCmd->bDeskStation == m_byMyStation)
			{
				PlaySound(SOUND_BEGIN);
			}
			else
			{
				PlaySound(SOUND_READY);
			}
		}

		return 0;
	}
	else if (nSubId == ASS_SUB_TRUSTEE)
	{
		MSG_GR_S_UserTruste * pCmd = (MSG_GR_S_UserTruste *)buffer;
		if (pCmd == NULL)
		{
			return 0;
		}

		m_bTuoGuan[pCmd->byChair] = pCmd->byFlag;

		// �����Ϲ�
		DrawTuoGuan(pCmd->byChair, pCmd->byFlag);

		if (pCmd->byChair == m_byMyStation)
		{
			if (m_bTuoGuan[m_byMyStation])
			{
				SAFE_CTRL_OPERATE(IButton, CTN_BTN_TuoGuan, SetControlVisible(false));
				SAFE_CTRL_OPERATE(IButton, CTN_BTN_Cancel_TuoGuan, SetControlVisible(true));

				// �Զ�����
				if (m_byGameState == MJ_STATE_GIVE || m_byGameState == MJ_STATE_BLOCK)
				{
					OnRequestGive(0, true);
				}
			}
			else
			{
				m_nTimeOutCount = 0;
				SAFE_CTRL_OPERATE(IButton, CTN_BTN_TuoGuan, SetControlVisible(true));
				SAFE_CTRL_OPERATE(IButton, CTN_BTN_Cancel_TuoGuan, SetControlVisible(false));
			}
		}

		return 0;
	}

	// ��Ϸ�ڲ���ϢID����
	if (buffer != NULL && nLen >= sizeof(TCMD_HEADER))
	{
		TCMD_HEADER *pData = (TCMD_HEADER *)buffer;
		if (MJFRAME_MOUDLE_MSG == pData->cCmdID)
		{
			switch(pData->cCmdPara)
			{
			case SC_MJ_NOTIFY_BASE_INFO:     // ��Ϸ������Ϣ(�������˵�������Ϣ)
				{
					OnNotifyBaseInfo(buffer, nLen);
				}
				break; 
			case SC_MJ_NOTIFY_VIEW_INFO:     // ��Ϸ������Ϣ(������ҽ��롢�����ػء��Թ۽���)
				{
					OnNotifyGameInfo(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_RUN_NBR:       // ���յ�֪ͨ��ׯ��Ϣ(��Ϸ��ʼ���͵ĵĵ�һ����Ϸ��Ϣ)
				{
					OnNotifyRunNbr(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_DEALER:        // ���յ�֪ͨׯ��λ����Ϣ
				{
					OnNotifyDealer(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_DEAL:          // ���յ�֪ͨ������Ϣ
				{
					OnNotifyDeal(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_FLOWER_OVER:   // ���յ�����������Ϣ(��һ����ҳ���)
				{
					OnNotifyFlowerOver(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_GIVE:          // ���յ����ƽ����Ϣ
				{
					OnNotifyGive(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_DRAW:          // ���յ�������Ϣ
				{
					OnNotifyDraw(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_BLOCK:         // ���յ������¼���Ϣ(��ԡ������ܡ�������)
				{
					OnNotifyBlock(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_BLOCK_RESULT:  // ���յ����ƽ����Ϣ
				{
					OnNotifyBlockResult(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_FINISH:        // ���յ���Ϸ������Ϣ
				{
					OnNotifyFinish(buffer, nLen);
				}
				break;
			case SC_MJ_UPDATEHANDTILES:      // ���յ���������������Ϣ
				{
					OnNotifyUpdateHandTiles(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_CHANGE_LOOKON: // ���յ��޸��Թۺ󷢸��Թ�����Ϣ(Ŀǰû��ʲô����, �����Թ�ʹ��)
				{
					OnNotifyChangeLookOn(buffer, nLen);
				}
				break;
			case SC_MJ_NOTIFY_GMCMD:         // ���յ�GM����ִ�н����Ϣ
				{
					OnNotifyGMCmd(buffer, nLen);
				}
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

/** 
 * @brief ������Ϣ
 * @param [in] pMessage ��Ӧ��������Ŀؼ���Ϣ
 */
int CMJFrameLogic::OnUIMessage(TUIMessage* pMessage)
{
	if (pMessage->_uMessageType == UI_KEYDOWN)
	{
		if (m_tagSetting.nOpenGM == 1)
		{
			if (pMessage->_ch == VK_F11)
			{
				IEditorText *pEdit = NULL;
				GETCTRL(IEditorText, pEdit, m_pUI, CTN_EDITORTEXT_GM);
				if (pEdit != NULL)
				{
					pEdit->SetControlVisible(!pEdit->GetVisible());
					pEdit->SetMaxTypeIn(100);
					pEdit->Clear();
					pEdit->SetText(L"");
				}
			}
			else if (pMessage->_ch == VK_RETURN)
			{
				//m_TilesWall.PopFrontTile();
				DrawWall(true);
				IEditorText *pEdit = NULL;
				GETCTRL(IEditorText, pEdit, m_pUI, CTN_EDITORTEXT_GM);
				if (pEdit != NULL)
				{
					if (pEdit->GetVisible())
					{
						OnRequestGMCmd();
					}
				}
			}
		}

		return 0;
	}

	if (pMessage->_uMessageType == UI_LBUTTONDOWN) 
	{
		switch (pMessage->_uControlID)
		{
		case CTN_JSK_BTN_Exit:       // ������˳�
			{
				OnGameQuiting();
				m_pGameFrame->CloseClient();
			}
			break;
		case CTN_BTN_Start:          // ��ʼ
		case CTN_JSK_BTN_Continue:   // ��������
			{
				//TCMD_NOTIFY_DEAL msg2;
				//msg2.ucDealer = 3;
				//for (int i = 0; i < 13; i++)
				//{
				//	msg2.tiles[i] = 2;
				//}
				//msg2.ucDice[0] = 1;
				//msg2.ucDice[1] = 1;
				//msg2.ucOther[0] = 4;
				//msg2.ucOther[1] = 4;
				//OnNotifyDeal((BYTE*)&msg2, sizeof(TCMD_NOTIFY_DEAL));

				InitData();

				SAFE_CTRL_OPERATE(IButton, CTN_BTN_Start, SetControlVisible(false));

				MSG_GR_S_Agree msg;
				m_pGameFrame->SendGameData(ASS_GM_AGREE_GAME, (void*)&msg, sizeof(msg));
			}
			break;
		case CTN_MJCtrl_HandCtrl_2:  // �Լ����ƿؼ�
			{
				if (m_byGameState == MJ_STATE_BLOCK && (m_byStateSelect == MJ_STATE_SELECT_COLLECT || m_byStateSelect == MJ_STATE_SELECT_QUADRUPLET))
				{
					// ���������ѡ��
					OnRequestBlockSelect(pMessage->_uControlID);
				}
				else
				{
					// ��������������ѡ��
					OnRequestGive(pMessage->_uControlID);
				}
			}
			break;
		case CTN_Block_BTN_Chi:      // �ԡ������ܡ���������ȡ��
		case CTN_Block_BTN_Peng:    
		case CTN_Block_BTN_Gang:    
		case CTN_Block_BTN_Ting:    
		case CTN_Block_BTN_Hu:      
		case CTN_Block_BTN_Cancel:  
			{
				OnRequestBlock(pMessage->_uControlID);
			}
			break;
		case CTN_BTN_TuoGuan:
		case CTN_BTN_Cancel_TuoGuan:
			{
				if (m_byGameState == MJ_STATE_END || m_byGameState == MJ_STATE_NOSTART)
				{
					break; // ����Ϸ״̬�в����Ϲܽ��д���
				}

				MSG_GR_S_UserTruste msg;
				msg.byFlag = !m_bTuoGuan[m_byMyStation];
				m_pGameFrame->SendGameData(ASS_SUB_TRUSTEE, (void*)&msg, sizeof(msg));
			}
			break;
		case CTN_BTN_CloseSound:  // �ر�����
			{
				m_bSound = true;
				CString s = CBcfFile::GetAppPath (); // ����·��
				DWORD dwCfgFile = ::cfgOpenFile(s + SKIN_FOLDER +".bcf");
				if (dwCfgFile >= 0x10) // �ļ��򿪳ɹ�
				{
					// ������������
					::cfgSetValue(dwCfgFile,"Operate","Sound",m_bSound);
					::cfgClose(dwCfgFile);						
				}

				if (m_pSoundEngine != NULL)
				{
					m_pSoundEngine->setAllSoundsPaused(false);
					PlayBackSound();//���ű�������
				}

				SAFE_CTRL_OPERATE(IButton, CTN_BTN_OpenSound, SetControlVisible(true));   // �����ְ�ť
				SAFE_CTRL_OPERATE(IButton, CTN_BTN_CloseSound, SetControlVisible(false)); // �ر����ְ�ť
			}
			break;
		case CTN_BTN_OpenSound:  // ������
			{
				m_bSound = false;
				CString s = CBcfFile::GetAppPath (); // ����·��
				DWORD dwCfgFile = ::cfgOpenFile(s + SKIN_FOLDER +".bcf");
				if (dwCfgFile >= 0x10)  // �ļ��򿪳ɹ�
				{
					// ������������
					::cfgSetValue(dwCfgFile, "Operate", "Sound", m_bSound);
					::cfgClose(dwCfgFile);						
				}

				if (m_pSoundEngine != NULL)
				{
					m_pSoundEngine->setAllSoundsPaused(true);
					m_pSoundEngine->stopAllSounds();
				}

				SAFE_CTRL_OPERATE(IButton, CTN_BTN_OpenSound, SetControlVisible(false));  // �����ְ�ť ����
				SAFE_CTRL_OPERATE(IButton, CTN_BTN_CloseSound, SetControlVisible(true));  // �ر����ְ�ť ��ʾ
				PlayBackSound(false);//���ű�������
			}
			break;
		default:
			break;
		}
	}
	else if (pMessage->_uMessageType == UI_CATOONFINISH) 
	{
		OnAnimalFinish(pMessage->_uControlID);  // ��������
	}
	else if (pMessage->_uMessageType == UI_TIME_RUN)
	{
		if (pMessage->_uControlID == CTN_Clock_Time)
		{
			// �������ӵ���5����ʾ��Ч
			PlaySound(SOUND_CLOCK);
		}
	}
	else if (pMessage->_uMessageType == UI_MJCTRL_MOVEONMJ)
	{
		if (pMessage->_uControlID == CTN_MJCtrl_HandCtrl_2)
		{
			// ������Ϣ�ƶ���ʾѡ��  
			OnBlockMoveSelect(pMessage->_uControlID, pMessage->_mouseX);

			// ��������ƶ��齫���ϵ���Ч
			PlaySound(SOUND_SEL);
		}
	}
	else if (pMessage->_uMessageType == UI_MOUSEENTER)
	{
		if (pMessage->_uControlID == CTN_MJCtrl_HandCtrl_2)
		{
			if (pMessage->_ch == 1)
			{
				// ����ƶ����ƿؼ�
				OnBlockMoveSelect(pMessage->_uControlID);
			}
		}
	}

	return 0;
}

/** 
 * @brief ��������
 * @param[in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnAnimalFinish(UINT32 uControlID)
{
	switch (uControlID) 
	{	
	case ExAnimate_0_Saizi_1:   // ��һ�����Ӳ������
	case ExAnimate_1_Saizi_1:
	case ExAnimate_2_Saizi_1:
	case ExAnimate_3_Saizi_1:
	case ExAnimate_0_Saizi_2:   // �ڶ������Ӳ������
	case ExAnimate_1_Saizi_2:
	case ExAnimate_2_Saizi_2:
	case ExAnimate_3_Saizi_2:
		{
			OnAnimalDiceFinish(uControlID);
		}
		break;
	case CTN_Clock_Time:        // ��ʱ������
		{
			OnAnimalClockFinish(uControlID);
		}
		break;
	default:
		break;
	}
}

/** 
 * @brief �������ӽ���
 * @param[in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnAnimalDiceFinish(UINT32 uControlID)
{
	IExAnimate * pExAnimate = NULL;
	GETCTRL(IExAnimate, pExAnimate, m_pUI, uControlID);	
	if (pExAnimate != NULL)
	{
		if (uControlID < ExAnimate_0_Saizi_2)
		{
			// ��1������
			pExAnimate->SetShowFrame(m_byCurDice[0] + 14);   // ���ӻ�ɫ����ʼֵ 1Ϊ 0 + 14, 2Ϊ 1 + 14
		}
		else
		{
			// ��2������
			pExAnimate->SetShowFrame(m_byCurDice[1] + 14);   // ���ӻ�ɫ����ʼֵ 1Ϊ 0 + 14, 2Ϊ 1 + 14
		}

		// ��������������ʾλ��
		pExAnimate->SetGameXY(pExAnimate->GetEnd().x, pExAnimate->GetEnd().y);

		m_byDiceFinishCount++;

		if (m_byDiceFinishCount == DICE_NBR)
		{
			m_byDiceFinishCount = 0;

			m_pGameFrame->SetTimer(TIME_DICE_FINISH, 2000);
		}
	}
}

/** 
 * @brief ������ʱ������
 * @param[in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnAnimalClockFinish(UINT32 uControlID)
{
	switch (m_byGameState)
	{
	case MJ_STATE_NOSTART: // ׼��ʱ�����
	case MJ_STATE_END:
		{
			m_pGameFrame->CloseClient();
		}
		break;
	case MJ_STATE_GIVE:    // ����ʱ�����
		{
			OnTimeOutGive(uControlID);
		}
		break;
	case MJ_STATE_BLOCK:   // ����ʱ�����
		{
			OnTimeOutBlock(uControlID);
		}
		break;
	default:
		break;
	}
}

/** 
 * @brief ���Ƴ�ʱ
 * @param[in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnTimeOutGive(UINT32 uControlID)
{
	if (m_byTurnStation == m_byMyStation)
	{
		OnRequestGive(TILE_BEGIN, true);

		m_nTimeOutCount++;  // ��¼��ʱ����, ��ʱ3�ν������й�״̬

		TCHAR szText[250];
		if (m_nTimeOutCount == 3)
		{
			wsprintf(szText, "���Ѿ���ʱ%d��, ϵͳ�Զ�Ϊ���й�!", m_nTimeOutCount);
			m_pGameFrame->InsertSystemMessage(szText);

			// ����������й�
			MSG_GR_S_UserTruste msg;
			msg.byFlag = 1;
			m_pGameFrame->SendGameData(ASS_SUB_TRUSTEE, (void*)&msg, sizeof(msg));

			m_nTimeOutCount = 0;
		}
		else
		{
			wsprintf(szText, "���Ѿ���ʱ%d��, ��ʱ3��ϵͳ�����Զ�Ϊ���й�!", m_nTimeOutCount);
			m_pGameFrame->InsertSystemMessage(szText);
		}
	}
}

/** 
 * @brief ���Ƴ�ʱ
 * @param[in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnTimeOutBlock(UINT32 uControlID)
{
	if (m_byTurnStation == m_byMyStation)
	{
		// ���س��������
		SAFE_CTRL_OPERATE(IContain, CTN_Block, SetControlVisible(false));

		// ȡ�����õ��ƿؼ�
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_HandCtrl_2, SetInvalidTile(NULL, 0));

		// ����������ʾ��Ϣ
		SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(false));

		// ȡ������, ������Ժ��ƣ���Ĭ�Ϻ���
		TCMD_REQUEST_BLOCK cmd;
		cmd.ucFlag = (m_stBlockInfo.byFlag & 0x10) ? ACTION_WIN : ACTION_EMPTY;
		m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));

		if (cmd.ucFlag == ACTION_EMPTY && m_TilesHand[m_byMyStation].CurrentLength() % 3 == 2)
		{
			m_byGameState = MJ_STATE_GIVE;
			SetGameTimer(m_byTurnStation, m_tagSetting.nTimerGive);
		}
	}
}


/** 
 * @brief ������Ϸ
 */
int CMJFrameLogic::ResetGameFrame(void)
{
	return 0;
}

/** 
 * @brief ��ҽ���
 */
int CMJFrameLogic::GameUserCome(void)
{	
	m_byMyStation = m_pGameFrame->GetMyDeskStation();
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		if (m_pGameFrame->GetUserInfo(i, m_tmpUserInfo))
		{
			DrawUserInfo(i, m_tmpUserInfo, true);
		}
	}
	return 0;
}

/** 
 * @brief ��ҽ���
 * @param [in] nIndex ��Ϸ����λ��
 */
int CMJFrameLogic::GameUserCome(int nIndex)
{
	return 0;
}

/** 
 * @brief ����뿪
 */
int CMJFrameLogic::GameUserLeft(int nIndex)
{
	m_bReady[nIndex] = false;

	DrawReady(nIndex, false);

	// �����뿪�����Ϣ
	DrawUserInfo(nIndex, m_tmpUserInfo, false);

	return 0;
}

/** 
 * @brief ��Ҷ���
 */
int CMJFrameLogic::GameUserOffLine(void)
{
	return 0;
}

/** 
 * @brief �Ƿ�������Ϸ������Ϸʵ��
 * @return �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
 */
bool CMJFrameLogic::IsPlayingGame()
{
	return m_byGameState ;
}

/** 
 * @brief ��ʱ����Ϣ
 * @param [in] nTimerID ��ʱ��ID
 */
void CMJFrameLogic::OnTimer(int nTimerID)
{
	switch (nTimerID)
	{
	case TIME_DEAL:          // ����
		{
			m_pGameFrame->KillTimer(TIME_DEAL);
			OnDeal();
		}
		break;
	case TIME_DICE_FINISH:   // �������
		{
			m_pGameFrame->KillTimer(TIME_DICE_FINISH);
			OnDice();
		}
		break;
	case TIME_FIND_POS:      // ����λ�ö�ʱ��
		{
			m_pGameFrame->KillTimer(TIME_FIND_POS);
			
			// ʹ��С��ָ���б���λ��ȷ��
			m_nSmallHandCount = 0;

			m_pGameFrame->SetTimer(TIME_HAND_MOVE, 300);
		}
		break;
	case TIME_HAND_MOVE:     // С��ָ�ƶ���ʱ��
		{
			m_pGameFrame->KillTimer(TIME_HAND_MOVE);
			OnSmallHandMove();
		}
		break;
	case TIME_SHOW_FINISH:   // ��ʾ�����ʱ��
		{
			m_pGameFrame->KillTimer(TIME_SHOW_FINISH);
			OnShowFinish();
		}
		break;
	default:
		break;
	}
}

/** 
 * @brief ���Ʋ�������
 */
void CMJFrameLogic::OnDeal()
{
	m_TilesWall.ReleaseAll();
	m_TilesWall.CurrentLength(m_tagSetting.nTilesNumber);

	// ����
	BYTE byStation = m_pGameFrame->DeskStation2View(m_byDealer);     // ץ�Ʒ���
	int nPos = (m_byDice[0] + m_byDice[1]) * 2;                      // �ӵڼ��տ�ʼץ
	int i, j, nStartPos, nCount = 0;

	for (i = 0; i < PLAYER_NBR; i++)
	{
		for (j = 0; j < m_tagSetting.nTilesHandNbr; j++)
		{
			nStartPos = byStation * MJ_WALL_NUMBER + nPos + nCount;
			if (nStartPos >= m_tagSetting.nTilesNumber)
			{
				nStartPos = nStartPos - m_tagSetting.nTilesNumber; 
			}

			m_TilesWall.SetTile(nStartPos,  -1);

			nCount++;
		}

		if (i == m_byDealer)
		{
			nStartPos = byStation * MJ_WALL_NUMBER + nPos + nCount;
			if (nStartPos >= m_tagSetting.nTilesNumber)
			{
				nStartPos = nStartPos - m_tagSetting.nTilesNumber; 
			}

			// ׯ�Ҷ�ץһ��
			m_TilesWall.SetTile(nStartPos,  -1);
			nCount++;
		}
	}

	// ������ǽ��ץ���벹��λ��
	m_TilesWall.SetPos(byStation * MJ_WALL_NUMBER + nPos + nCount, byStation * MJ_WALL_NUMBER + nPos - 1);

	// ��ʾ��ǽ
	DrawWall(true);

	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		DrawHand(i, m_TilesHand[i], m_TilesHand[i].CurrentLength() % 3 == 2);
	}

	// �������, ��Ҫ֪ͨ�����
	TCMD_REQUEST_DEAL_FINISH cmd; 
	m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));
}

/** 
 * @brief ���Ӳ�������
 */
void CMJFrameLogic::OnDice()
{
	switch (m_enDiceState)
	{
	case En_Dice_1:   // ��1������, ȷ����һλ�����ӵ����
		{
			//// ����ڶ�������
			//m_enDiceState = En_Dice_2;

			//// �������Ӷ���
			//DrawDice((m_byDealer + 1) % PLAYER_NBR, m_byDice, true);

			// ������ɺ�, ���뷢��
			m_pGameFrame->SetTimer(TIME_DEAL, 800);
		}
		break;
	case En_Dice_2:   // ��2������, ȷ��ץ��λ��
		{
			m_pGameFrame->SetTimer(TIME_DEAL, 800);
		}
		break;
	case En_Dice_3:   // ��3������, ȷ�����Ƶ�λ��
		{
			m_pGameFrame->SetTimer(TIME_FIND_POS, 800);
		}
		break;
	case En_Dice_4:   // ��4������, ��������ȷ������λ��
		{
			m_pGameFrame->SetTimer(TIME_FIND_POS, 800);
		}
		break;
	}
}

/** 
 * @brief С���ƶ���������
 */
void CMJFrameLogic::OnSmallHandMove()
{
	// ����С���ƶ�����
	PlaySound(SOUND_SEL);

	if (m_nSmallHandCount < m_byCurDice[0] + m_byCurDice[1])
	{
		// m_nSmallHandCount��Ҫ�ȼ�, ���ܴ�0��ʼ.
		DrawSmallHand(++m_nSmallHandCount, true);

		m_pGameFrame->SetTimer(TIME_HAND_MOVE, 200);
	}
	else
	{
		// ����С��
		m_nSmallHandCount = 0;
		DrawSmallHand(0, false);

		// ��������
		DrawDice(0, m_byCurDice, false);

		if (m_enDiceState == En_Dice_4)
		{
			// ����λ��ȷ��
			// ����ǽָ��λ�ñ�һ����
			int nPos = m_TilesWall.PopFrontTile(m_stDrawInfo.byDice[0] + m_stDrawInfo.byDice[1]);

			if (m_stDrawInfo.chair == m_byMyStation)
			{
				m_TilesHand[m_stDrawInfo.chair].AddTile(m_stDrawInfo.t);
			}
			else
			{
				m_TilesHand[m_stDrawInfo.chair].AddTile(TILE_BEGIN);
			}

			// ������ǽ
			DrawWall(true);

			// ������������
			DrawHand(m_stDrawInfo.chair, m_TilesHand[m_stDrawInfo.chair], true);

			// ����״̬
			m_byGameState = MJ_STATE_GIVE;

			m_byTurnStation = m_stDrawInfo.chair;

			// �ж��Ƿ������Ʋ���
			if (m_stDrawInfo.ucFlag == ACTION_EMPTY)
			{
				// ���ó��ƶ�ʱ��
				SetGameTimer(m_byTurnStation, m_tagSetting.nTimerGive);

				if (m_byTurnStation == m_byMyStation && m_bTuoGuan[m_byMyStation])
				{
					// �Զ�����
					OnRequestGive(TILE_BEGIN, true);
				}
			}
			else
			{
				OnNotifyBlock((BYTE*)&m_stDrawInfo, sizeof(m_stDrawInfo));
			}
		}
	}
}

/** 
 * @brief ��ʾ�����������
 */
void CMJFrameLogic::OnShowFinish()
{
	m_pGameFrame->InsertNormalMessage("------------------");
	m_pGameFrame->InsertNormalMessage("��Ϸ������Ϣ:");
	TCHAR szText[MAX_PATH];
	TCHAR szName[MAX_PATH];
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		memset(szName, 0, sizeof(szName));
		W2C(m_wNickName[i], szName);
		wsprintf(szText, "%s: ���ͷ�: %d, �ܷ�: %d �ܼ���Ӯ: %d", 
			szName, m_stFinishInfo.nFanPoint[i], m_stFinishInfo.nGangFen[i], m_stFinishInfo.nMoney[i]);
		m_pGameFrame->InsertNormalMessage(szText);
	}
	m_pGameFrame->InsertNormalMessage("------------------");

	// ��ʾ����
	DrawFinish(&m_stFinishInfo, true);

	// ˢ�������Ϣ
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		if (m_pGameFrame->GetUserInfo(i, m_tmpUserInfo))
		{
			DrawUserInfo(i, m_tmpUserInfo, true);
		}
	}

}


/** 
 * @brief ����״̬
 * @param [in] bGameStation ��Ϸ״̬
 */
void CMJFrameLogic::SetStationParameter(BYTE bGameStation)
{

}

/** 
 * @brief �˳��¼�
 */
bool CMJFrameLogic::OnGameQuiting()
{
	if(m_pGameFrame->GetIsWatching())
	{
		return true ; 
	}

	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength() - 1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);


	CString strKeyName  ; 
	strKeyName.Format("%d", NAME_ID) ; 
	CBcfFile File(szAppPathParent + "\\SpecialRule.bcf") ; 

	bool bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto", strKeyName, 1) ;		
	if (m_byGameState != MJ_STATE_NOSTART && m_byGameState != MJ_STATE_END)
	{
		if (bForceQuitAsAuto)
		{
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}
		}
		else
		{
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪���ᱻ�۷����Ƿ����Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}

			m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////
/** 
 * @brief ���������Ϣ
 * @param[in] byChair    ���λ��
 * @param[in] stUserInfo �����Ϣ
 * @param[in] bShow      �Ƿ���ʾ
 */
void CMJFrameLogic::DrawUserInfo(BYTE byChair, UserInfoForGame stUserInfo, bool bShow)
{
	BYTE byViewStation = m_pGameFrame->DeskStation2View(byChair);
	wchar_t wText[MAX_PATH];
	IText * pText = NULL;
	IImage * pImage = NULL;
	IExAnimate * pExAnimate = NULL;

	// ��ʾ������ͼ
	SAFE_CTRL_OPERATE(IImage, CTN_IMG_BK_0 + byViewStation, SetControlVisible(bShow));

	// ��ʾ�ǳ�
	GETCTRL(IText, pText, m_pUI, CTN_TEXT_NickName_0 + byViewStation);
	if (pText != NULL)
	{
		C2W(stUserInfo.szNickName, wText);
		pText->SetText(wText);
		pText->SetControlVisible(bShow);
	}

	// ��ʾ���
	GETCTRL(IText, pText, m_pUI, CTN_TEXT_Money_0 + byViewStation);
	if (pText != NULL)
	{
		// ��Ϸ��������;���۵Ľ����m_nTempMoney�����б��ֳ���
		swprintf(wText, MAX_PATH, L"%d", stUserInfo.i64Money + m_nTempMoney[byChair]);
		pText->SetText(wText);
		pText->SetControlVisible(bShow);
	}

	// ��ʾIP
	//GETCTRL(IText, pText, m_pUI, CTN_TEXT_IP_0 + byViewStation);
	//if (pText != NULL)
	//{
	//	C2W("������", wText);
	//	pText->SetText(wText);
	//	pText->SetControlVisible(bShow);
	//}

	// ��ʾ����Լ�����Ϣ
	if (byChair == m_byMyStation)
	{
		// �Լ��Ľ�ҵ�ͼ
		SAFE_CTRL_OPERATE(IImage, CTN_Self_Money_BG, SetControlVisible(true));

		// ��ʾ�û�ID
		GETCTRL(IText, pText, m_pUI,CTN_TEXT_UserID);
		if (pText != NULL)
		{
			swprintf(wText, MAX_PATH, L"%d", stUserInfo.dwUserID);
			pText->SetText(wText);
			pText->SetControlVisible(bShow);
		}
		
		// �Լ��ĵȼ���ͼ
		//SAFE_CTRL_OPERATE(IImage, CTN_Self_Level_BG, SetControlVisible(true));

		// �Լ���ѫ�µ�ͼ
		//SAFE_CTRL_OPERATE(IImage, CTN_Self_Medal_BG, SetControlVisible(true));
	}
	else
	{
		// ��ʾ���ͷ��(�Լ��Ĳ���Ҫ��ʾ)
		CString strUserPath = "GameGrand\\LogoAnimal\\" ; 
		CString strDir = (byViewStation > 1) ? "left" : "right";
		CString strSex = (stUserInfo.bBoy) ? "boy" : "girl";
		CString strName;
		strName.Format("%s_%d_%s.bani",strSex, stUserInfo.bLogoID % 4, strDir) ; 
		strUserPath += strName;	
		pExAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(CTN_ExAnimate_Logo_0 + byViewStation));
		if(pExAnimate != NULL)
		{
			pExAnimate->SetExtFilePath(CA2W(strUserPath)) ; 
			pExAnimate->LoadFromFile();
			pExAnimate->SetIsLoop(true) ;
			pExAnimate->SetIsSymmetrical(false);
			pExAnimate->SetControlVisible(bShow);
			pExAnimate->SetPlayState(bShow);
		}
	}
}

/** 
 * @brief ����׼��
 * @param[in] byChair ���λ��
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawReady(BYTE byChair, bool bShow)
{
	SAFE_CTRL_OPERATE(IImage, CTN_IMG_Ready_0 + m_pGameFrame->DeskStation2View(byChair), SetControlVisible(bShow));

	if (byChair == m_byMyStation)
	{
		IExAnimate * pExAnimate = NULL;
		GETCTRL(IExAnimate, pExAnimate, m_pUI, ExAnimate_Ready_Wait);
		if (pExAnimate != NULL)
		{
			pExAnimate->SetControlVisible(bShow);
			pExAnimate->SetIsLoop(true);
			pExAnimate->SetPlayState(bShow);
		}
	}
}

/** 
 * @brief ��������
 * @param[in] byChair ���λ��
 * @param[in] byDice  ����
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawDice(BYTE byChair, BYTE byDice[], bool bShow)
{
	KillAllTimer();

	IExAnimate * pExAnimate = NULL;

	// ����������
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		GETCTRL(IExAnimate, pExAnimate, m_pUI, ExAnimate_0_Saizi_1 + i);
		if (pExAnimate != NULL)
		{
			pExAnimate->SetControlVisible(false);
			pExAnimate->SetIsLoop(false);
			pExAnimate->SetPlayState(false);
		}

		GETCTRL(IExAnimate, pExAnimate, m_pUI, ExAnimate_0_Saizi_2 + i);
		if (pExAnimate != NULL)
		{
			pExAnimate->SetControlVisible(false);
			pExAnimate->SetIsLoop(false);
			pExAnimate->SetPlayState(false);
		}
	}

	m_byDiceFinishCount = 0;
	BYTE bViewStation = m_pGameFrame->DeskStation2View(byChair);
	
	memcpy(m_byCurDice, byDice, sizeof(m_byCurDice));

	GETCTRL(IExAnimate, pExAnimate, m_pUI, ExAnimate_0_Saizi_1 + bViewStation);
	if (pExAnimate != NULL)
	{
		pExAnimate->SetControlVisible(bShow);
		pExAnimate->SetIsLoop(false);
		pExAnimate->SetPlayState(bShow);
	}

	GETCTRL(IExAnimate, pExAnimate, m_pUI, ExAnimate_0_Saizi_2 + bViewStation);
	if (pExAnimate != NULL)
	{
		pExAnimate->SetControlVisible(bShow);
		pExAnimate->SetIsLoop(false);
		pExAnimate->SetPlayState(bShow);
	}

	if (bShow)
	{
		PlaySound(SOUND_SEZI);
	}
}

/** 
 * @brief �����й�
 * @param[in] byChair ���λ��
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawTuoGuan(BYTE byChair, bool bShow)
{
	SAFE_CTRL_OPERATE(IImage, CTN_IMG_TuoGuan_0 + m_pGameFrame->DeskStation2View(byChair), SetControlVisible(bShow));
}

/** 
 * @brief ����ׯ��
 * @param[in] byChair ���λ��
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawDealer(BYTE byChair, bool bShow)
{
	SAFE_CTRL_OPERATE(IImage, CTN_IMG_NT_0 + m_pGameFrame->DeskStation2View(byChair), SetControlVisible(bShow));
}

/** 
 * @brief �������ƿ�
 * @param[in] byFlag ���Ʊ�ʶ: ��(0x01), ��(0x02) ��(0x04), ��(0x08), ��(0x10)
 */
void CMJFrameLogic::DrawBlock(BYTE byFlag)
{
	SAFE_CTRL_OPERATE(IContain, CTN_Block, SetControlVisible(byFlag > 0));
	SAFE_CTRL_OPERATE(IButton, CTN_Block_BTN_Cancel, SetEnable(true));

	for (BYTE i = 0; i < 5; i++)
	{
		SAFE_CTRL_OPERATE(IButton, CTN_Block_BTN_Chi + i, SetEnable(byFlag & (1 << i)));
	}
}

/** 
 * @brief ��������
 * @param[in] byChair         ���λ��
 * @param[in] mjHand          ��������
 * @param[in] bIsLaydown      �ؼ��Ƿ�����(̯��)
 * @param[in] bIsGiveTileMode ���ұ�һ�����Ƿ�ֿ�
 */
void CMJFrameLogic::DrawHand(BYTE byChair, CMJHand mjHand, bool bIsGiveTileMode, bool bIsLaydown)
{
	IMjHandCtrl * pMJCtrl = NULL;
	GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_0 + m_pGameFrame->DeskStation2View(byChair));
	if (pMJCtrl != NULL)
	{
		int nTiles[20];
		int nTilesCount = mjHand.GetAllTile(nTiles);
		pMJCtrl->SetHandInfo(nTiles, nTilesCount);
		pMJCtrl->SetGiveTileMode(bIsGiveTileMode);
		pMJCtrl->SetIsLaydown(bIsLaydown);
		pMJCtrl->SetControlVisible(nTilesCount > 0);
	}
}

/** 
 * @brief ���Ƴ���
 * @param[in] byChair ���λ��
 * @param[in] mjGive  ��������
 */
void CMJFrameLogic::DrawGive(BYTE byChair, CMJGive mjGive)
{
	IMjGiveCtrl * pMJCtrl = NULL;
	GETCTRL(IMjGiveCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_GiveCtrl_0 + m_pGameFrame->DeskStation2View(byChair));
	if (pMJCtrl != NULL)
	{
		int nTiles[50];
		mjGive.GetAllTile(nTiles);
		pMJCtrl->SetGiveInfo(nTiles, mjGive.CurrentLength());
		pMJCtrl->SetControlVisible(mjGive.CurrentLength() > 0);
	}
}

/** 
 * @brief ��������
 * @param[in] byChair ���λ��
 * @param[in] mjSet   ��������
 */
void CMJFrameLogic::DrawSet(BYTE byChair, CMJSet mjSet)
{
	IMjHandCtrl * pMJCtrl = NULL;
	GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_0 + m_pGameFrame->DeskStation2View(byChair));
	if (pMJCtrl != NULL)
	{
		pMJCtrl->ClearAllBlock();

		TILE_SET tileSet;
		for (BYTE i = 0; i < mjSet.CurrentLength(); i++)
		{
			mjSet.GetSet(i, tileSet);
			switch (tileSet.m_ucFlag)
			{
			case ACTION_COLLECT:               // ��
				{
					pMJCtrl->AddOneBlock(LT_COLLECT, tileSet.m_ucTile, tileSet.m_ucChair);
				}
				break;
			case ACTION_TRIPLET:               // ��
				{
					pMJCtrl->AddOneBlock(LT_TRIPLET, tileSet.m_ucTile, tileSet.m_ucChair);
				}
				break;
			case ACTION_QUADRUPLET_PATCH:      // ����
			case ACTION_QUADRUPLET_REVEALED:   // ����
				{
					pMJCtrl->AddOneBlock(LT_QUADRUPLET_REVEALED, tileSet.m_ucTile, tileSet.m_ucChair);
				}
				break;
			case ACTION_QUADRUPLET_CONCEALED:  // ����
				{
					tileSet.m_ucTile[0] = TILE_BEGIN;
					tileSet.m_ucTile[1] = TILE_BEGIN;
					tileSet.m_ucTile[2] = TILE_BEGIN;
					pMJCtrl->AddOneBlock(LT_QUADRUPLET_CONCEALED, tileSet.m_ucTile, tileSet.m_ucChair);
				}
				break;
			case ACTION_QUADRUPLET_DNXB:       // ��������
			case ACTION_QUADRUPLET_ZFB:        // �з���X
				{
					pMJCtrl->AddOneBlock(LT_QUANDUPLET_SPECIAL, tileSet.m_ucTile, tileSet.m_ucChair);
				}
				break;
			default:
				break;
			}
		}
	}
}

/** 
 * @brief ������ǽ
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawWall(bool bShow)
{
	IMjWallCtrl * pMJCtrl = NULL;
	int i, j, byViewStation;
	int pMjStatus[MJ_WALL_NUMBER] = {0};
	TILE tile;
	for (i = 0; i < PLAYER_NBR; i++)
	{
		byViewStation = m_pGameFrame->DeskStation2View(i);
		GETCTRL(IMjWallCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_WallCtrl_0 + byViewStation);
		if (pMJCtrl != NULL)
		{
			// ��¼�齫��״��
			memset(&pMjStatus, 0, sizeof(pMjStatus));
			CString str;
			for (j = 0; j < MJ_WALL_NUMBER; j ++) 
			{
				tile = m_TilesWall.GetTile(byViewStation * MJ_WALL_NUMBER + j);

				if (tile == 255) 
				{		
					// ���ɼ�
					pMJCtrl->SetWallTile(-1, j);
					pMjStatus[j] = -1;
				}
				else
				{																						
					// �ɼ�
					pMJCtrl->SetWallTile(tile, j);
				}
			}

			// ����(��)����ʱ, �����齫��Żᵼ���齫��ץ����һ�ţ������͵�������һ�Ÿ��ڿ��У���Ҫ�Ѹ������������Ʒŵ�����һ��λ��
			for (j = 0; j < MJ_WALL_NUMBER; j += 2) 
			{
				if (pMjStatus[j] == 0 && pMjStatus[j + 1] == -1) 
				{
					tile = m_TilesWall.GetTile(byViewStation * MJ_WALL_NUMBER + j);
					pMJCtrl->SetWallTile(-1, j);
					pMJCtrl->SetWallTile(tile, j + 1);
				}
			}
		}
	}
}

/** 
 * @brief ����С��
 * @param[in] nOffsetPos С��ָ���ڼ��յ���
 * @param[in] bShow      �Ƿ���ʾ
 */
void CMJFrameLogic::DrawSmallHand(int nOffsetPos, bool bShow)
{
	int nIndex = 0;
	int nPos = m_TilesWall.GetEndPos(nOffsetPos);
	BYTE byViewStation = 0;
	if (nPos >= 0 && nPos < 34)
	{
		byViewStation = 0;
	}
	else if (nPos >= 34 && nPos < 68)
	{
		byViewStation = 1;
	}
	else if (nPos >= 68 && nPos < 102)
	{
		byViewStation = 2;
	}
	else if (nPos >= 102 && nPos < 136)
	{
		byViewStation = 3;
	}

	IMjWallCtrl * pMJCtrl = NULL;
	GETCTRL(IMjWallCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_WallCtrl_0 + byViewStation);
	if (pMJCtrl == NULL)
	{
		return;
	}

	// ȷ����ǰλ�����ĸ��ؼ��ڼ���
	nIndex = nPos - byViewStation * MJ_WALL_NUMBER;

	// С��ָ�Ǹ�����ǽ�ؼ�����һ�ŵ��齫����ƫ�Ƶ�, �������������һ�ŵ���������Ҫ+1ת��������һ�ŵ�����
	RECT rtLast;
	if (nIndex % 2 == 0)
	{
		rtLast = pMJCtrl->GetTileRectByIndex(MJ_WALL_NUMBER - (nIndex + 1));
	}
	else
	{
		rtLast = pMJCtrl->GetTileRectByIndex(MJ_WALL_NUMBER - nIndex);
	}

	// С��ָ���ÿ����λ����ǽ�ؼ�ƫ��λ��
	const POINT ptOffset[PLAY_COUNT] = { {-16, -65}, {-8, -62}, {-10, -55}, {-8, -60} };

	IImage * pImage = NULL;
	GETCTRL(IImage, pImage, m_pUI, CTN_IMG_Hand);
	if (pImage != NULL)
	{
		pImage->SetGameXY(rtLast.left + ptOffset[byViewStation].x, rtLast.top + ptOffset[byViewStation].y);
		pImage->SetControlVisible(bShow);
	}
}

/** 
 * @brief ���Ƴ��Ʊ��
 * @param[in] byChair ���λ��
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawGiveAnimate(BYTE byChair, bool bShow)
{
	IExAnimate *pExAnimate = NULL;
	GETCTRL(IExAnimate, pExAnimate, m_pUI, CTN_ExAnimate_GiveTag);

	if (pExAnimate == NULL)
	{
		return;
	}

	BYTE byViewStation = m_pGameFrame->DeskStation2View(byChair);

	IMjGiveCtrl * pMJCtrl = NULL;
	GETCTRL(IMjGiveCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_GiveCtrl_0 + byViewStation);
	if (pMJCtrl == NULL)
	{
		return;
	}

	const POINT ptOffset[PLAY_COUNT] = 
	{
		{-5, -25},
		{2, -30},
		{-5, -25},
		{2, -30}
	};

	RECT rtLast;
	rtLast = pMJCtrl->GetTileRectByIndex(pMJCtrl->GetTileCount() - 1);   
	pExAnimate->SetGameXY(rtLast.left + ptOffset[byViewStation].x, rtLast.top + ptOffset[byViewStation].y);
	pExAnimate->SetIsLoop(bShow);
	pExAnimate->SetPlayState(bShow);
	pExAnimate->SetControlVisible(bShow);
}

/** 
 * @brief  
 * @param[in] pCmd    ��������
 * @param[in] bShow   �Ƿ���ʾ
 */
void CMJFrameLogic::DrawFinish(TCMD_NOTIFY_FINISH * pCmd, bool bShow)
{
	if (pCmd == NULL)
	{
		SAFE_CTRL_OPERATE(IContain, CTN_JieSuanKuang, SetControlVisible(false));
		return ;
	}

	int i, j, nCount = 0;
	wchar_t wText[MAX_PATH];
	BYTE byWinStation = -1;
	BYTE byViewStation = -1;
	IExAnimate * pExAnimate = NULL;
	IMjHandCtrl * pHandCtrl = NULL;
	IContain * pContain = NULL;
	IText  * pText  = NULL;
	IImage * pImage = NULL;
	INum   * pNum   = NULL;

	SAFE_CTRL_OPERATE(IContain, CTN_JieSuanKuang, SetControlVisible(bShow));

	// ��ʾ���������
	if (pCmd->byFlag == MJ_END_SELF)
	{
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_ZiMo, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_DianPao, SetControlVisible(false));

		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_LiuJu, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_Line1, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_Line2, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_pxhj, SetControlVisible(true));
	}
	else if (pCmd->byFlag == MJ_END_GUN || pCmd->byFlag == MJ_END_QIANG)
	{
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_ZiMo, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_DianPao, SetControlVisible(true));

		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_LiuJu, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_Line1, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_Line2, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_pxhj, SetControlVisible(true));
	}
	else
	{
		// ����
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_ZiMo, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_DianPao, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_LiuJu, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_Line1, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_Line2, SetControlVisible(false));
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_pxhj, SetControlVisible(false));
	}

	// ����ʤ����ʧ����Ч
	PlaySound(pCmd->nMoney[m_byMyStation] >= 0 ? SOUND_WIN : SOUND_LOST);

	for (i = 0; i < PLAYER_NBR; i++)
	{
		if (pCmd->byWhoWin[i])
		{
			byWinStation = i;
			break;
		}
	}

	if (byWinStation != 255)
	{
		// ��ʾ��������ǳ�
		SAFE_CTRL_OPERATE(IText, CTN_JSK_TEXT_Win_NickName, SetText(m_wNickName[byWinStation]));
	}
	else
	{
		SAFE_CTRL_OPERATE(IText, CTN_JSK_TEXT_Win_NickName, SetText(L""));
	}

	// ��ʾ����������
	GETCTRL(IMjHandCtrl, pHandCtrl, m_pUI, CTN_JSK_MJHandCtrl);
	if (pHandCtrl != NULL)
	{
		if (byWinStation != 255)
		{
			int nTiles[20];
			int nTilesCount = m_TilesHand[byWinStation].GetAllTile(nTiles);
			pHandCtrl->SetHandInfo(nTiles, nTilesCount);
			pHandCtrl->SetGiveTileMode(true);
			pHandCtrl->SetIsUseMouse(false);
			pHandCtrl->SetControlVisible(true);

			// ���þ���
			m_TilesGod.GetAllTile(nTiles);
			pHandCtrl->SetGodTile(nTiles, m_TilesGod.CurrentLength());

			pHandCtrl->ClearAllBlock();

			TILE_SET tileSet;
			for (BYTE i = 0; i < m_TilesSet[byWinStation].CurrentLength(); i++)
			{
				m_TilesSet[byWinStation].GetSet(i, tileSet);
				switch (tileSet.m_ucFlag)
				{
				case ACTION_COLLECT:               // ��
					{
						pHandCtrl->AddOneBlock(LT_COLLECT, tileSet.m_ucTile, tileSet.m_ucChair);
					}
					break;
				case ACTION_TRIPLET:               // ��
					{
						pHandCtrl->AddOneBlock(LT_TRIPLET, tileSet.m_ucTile, tileSet.m_ucChair);
					}
					break;
				case ACTION_QUADRUPLET_PATCH:      // ����
				case ACTION_QUADRUPLET_REVEALED:   // ����
					{
						pHandCtrl->AddOneBlock(LT_QUADRUPLET_REVEALED, tileSet.m_ucTile, tileSet.m_ucChair);
					}
					break;
				case ACTION_QUADRUPLET_CONCEALED:  // ����
					{
						tileSet.m_ucTile[0] = TILE_BEGIN;
						tileSet.m_ucTile[1] = TILE_BEGIN;
						tileSet.m_ucTile[2] = TILE_BEGIN;
						pHandCtrl->AddOneBlock(LT_QUADRUPLET_CONCEALED, tileSet.m_ucTile, tileSet.m_ucChair);
					}
					break;
				default:
					break;
				}
			}
		}
		else
		{
			pHandCtrl->SetHandInfo(NULL, 0);
			pHandCtrl->SetControlVisible(false);
		}
	}

	// ��ʾ������ҵ���ؽ�����Ϣ
	for (i = 0; i < PLAYER_NBR; i++)
	{
		byViewStation = m_pGameFrame->DeskStation2View(i);

		// ��ʾ�ǳ�
		SAFE_CTRL_OPERATE(IText, CTN_JSK_TEXT_NickName_0 + byViewStation, SetText(m_wNickName[i]));
		
		// ��ʾ����
		swprintf(wText, MAX_PATH, L"%d", pCmd->nFanPoint[i]);

		SAFE_CTRL_OPERATE(IText, CTN_JSK_TEXT_HuFen_0 + byViewStation, SetText(wText));

		// �ܷ�
		swprintf(wText, MAX_PATH, L"%d", pCmd->nGangFen[i]);
		SAFE_CTRL_OPERATE(IText, CTN_JSK_TEXT_SJGangFen_0 + byViewStation, SetText(wText));

		// ��ʾ˰��
		swprintf(wText, MAX_PATH, L"%d", pCmd->nTax[i]);
		SAFE_CTRL_OPERATE(IText, CTN_JSK_TEXT_Tax_0 + byViewStation, SetText(wText));

		// ��ʾ���ܷ�
		GETCTRL(INum, pNum, m_pUI, CTN_JSK_NUM_AllFen_0 + byViewStation);
		if (pNum != NULL)
		{
			// ���ݷ�������������ͼƬ
			if (pCmd->nMoney[i] >= 0)
			{
				pNum->SetImagePath(L"GameGrand\\ying_num1.png");
			}
			else
			{
				pNum->SetImagePath(L"GameGrand\\shu_num.png");
			}
			pNum->SetHaveSymbol(true);
			pNum->SetNum(pCmd->nMoney[i]);
		}

		// ��ʾׯ
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_NT_0 + byViewStation, SetControlVisible(i == m_byDealer));

		// ��ʾ��
		SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_HU_0 + byViewStation, SetControlVisible(pCmd->byWhoWin[i]));

		// ��ʾ��
		if (pCmd->byFlag == MJ_END_GUN || pCmd->byFlag == MJ_END_QIANG)
		{
			SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_PAO_0 + byViewStation, SetControlVisible(pCmd->byWhoGun == i));
		}
		else
		{
			SAFE_CTRL_OPERATE(IImage, CTN_JSK_IMG_PAO_0 + byViewStation, SetControlVisible(false));
		}
	}

	nCount = 0;

	// ��ʾ����
	if (byWinStation != 255)
	{
		nCount = m_tagSetting.nBaseScore;

		for (i = 0, j = 0; i < FAN_XING_COUNT; i++)
		{
			if (pCmd->byFan[byWinStation][i])
			{
				SAFE_CTRL_OPERATE(IContain, CTN_JSK_CTN_FanXing + j * 10, SetControlVisible(true));

				// ��ʾ��������ǳ�
				GETCTRL(IText, pText, m_pUI, CTN_JSK_CTN_FanXing_Name + j * 10);
				if (pText != NULL)
				{
					pText->SetText(g_wFanXingName[i]);
					pText->SetControlVisible(true);
				}
				
				// ��ʾ������ֵ
				GETCTRL(INum, pNum, m_pUI, CTN_JSK_CTN_FanXing_Num + j * 10);
				if (pNum != NULL)
				{
					pNum->SetHaveSymbol(true);
					pNum->SetNum(pCmd->byFanScore[byWinStation][i]);
					pNum->SetControlVisible(true);
				}

				nCount *= pCmd->byFanScore[byWinStation][i];
				j++;
			}
		}

		// ���ز���Ҫ�ķ��Ϳؼ�
		for (i = j; i < FAN_XING_COUNT; i++)
		{
			SAFE_CTRL_OPERATE(IContain, CTN_JSK_CTN_FanXing + i * 10, SetControlVisible(false));
		}
	}

	// ��ʾ�����ܷ�
	GETCTRL(INum, pNum, m_pUI, CTN_JSK_NUM_AllFen);
	if (pNum != NULL)
	{
		pNum->SetHaveSymbol(false);
		pNum->SetNum(nCount);
		pNum->SetControlVisible(nCount > 0);
	}
}
//////////////////////////////////////////////////////////////////////////////
/** 
 * @brief ���ö�ʱ��
 * @param[in] byChair ���λ��
 * @param[in] uTime   ʱ������
 */
void CMJFrameLogic::SetGameTimer(BYTE byChair, UINT uTime)
{
	SAFE_CTRL_OPERATE(IContain, CTN_Clock, SetControlVisible(uTime > 0));

	if (uTime > 0)
	{
		ITime * pTime = NULL;
		GETCTRL(ITime, pTime, m_pUI, CTN_Clock_Time);
		if (pTime != NULL)
		{
			pTime->SetControlVisible(true);
			pTime->SetShowMaxNum(uTime);
			pTime->SetShowMinNum(0);
			pTime->SetPlaySoundNum(5);
			pTime->SetPLay(true);
		}

		// ���ü�ͷ��ʾ����
		for (BYTE i = 0; i < PLAYER_NBR; i++)
		{
			SAFE_CTRL_OPERATE(IImage, CTN_Clock_IMG_Arrows_0 + m_pGameFrame->DeskStation2View(i), SetControlVisible(i == byChair));
		}
	}
}

/** 
 * @brief ֹͣ���ж�ʱ��
 */
void CMJFrameLogic::KillAllTimer()
{
	SAFE_CTRL_OPERATE(IContain, CTN_Clock, SetControlVisible(false));

	ITime * pTime = NULL;
	GETCTRL(ITime, pTime, m_pUI, CTN_Clock_Time);
	if (pTime != NULL)
	{
		pTime->SetControlVisible(false);
		pTime->SetShowMaxNum(0);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(false);
	}
}

/** 
 * @brief ��������
 * @param[in] nSoundID ��������ID
 * @param[in] byChair  ���λ��
 */
void CMJFrameLogic::PlaySound(int nSoundID, BYTE byChair)
{
	if (!m_bSound || m_pSoundEngine == NULL)
	{
		return;
	}

	char chSoundName[MAX_PATH] = {0};
	char chSoundPath[MAX_PATH] = {0};
	char szPlay[MAX_PATH] = "";

	if (byChair >= 0 && byChair < PLAYER_NBR)
	{
		// ��Ů��Ч
		m_pGameFrame->GetUserInfo(byChair, m_tmpUserInfo);
		sprintf(chSoundPath, "music/putong/%s", m_tmpUserInfo.bBoy ? "Man" : "Wom");
	}
	else
	{
		// ������Ч
		sprintf(chSoundPath, "music");
	}

	if (m_SoundMap[nSoundID].nCount > 0)
	{
		sprintf(chSoundName, "%s", m_SoundMap[nSoundID].szFileName[(m_SoundMap[nSoundID].nCount - 1) % MAX_SOUND_RAND]);
		sprintf(szPlay, "%s/%s", chSoundPath, chSoundName);

		m_pSoundEngine->play2D(szPlay, false);
	}
}

/** 
 * @brief ��������
 * @param[in] pSoundName ��������
 */
void CMJFrameLogic::PlaySound(char * pSoundName)
{
	if (pSoundName == NULL)
	{
		return ;
	}

	if (!m_bSound || m_pSoundEngine == NULL)
	{
		return;
	}

	char szPlay[MAX_PATH] = "";

	// ������Ч
	sprintf(szPlay, "music/%s", pSoundName);

	m_pSoundEngine->play2D(szPlay, false);
}

/** 
 * @brief ���ű�������
 * @param[in] bPlay ���ſ���(true: ����,  false: ֹͣ)
 */
void CMJFrameLogic::PlayBackSound(bool bPlay)
{
	if (!m_bSound)
	{		
		return ;
	}

	if (m_pSoundEngine == NULL)
	{		
		return;
	}

	char szFileName[MAX_PATH];
	srand((unsigned)time(NULL));
	int iSoundType = rand() % 3 + 1;
	sprintf(szFileName, "music\\bg%d.ogg", iSoundType);

	if (bPlay)
	{
		m_pSoundEngine->play2D(szFileName, true);
	}
	else
	{
		m_pSoundEngine->stopAllSounds();
	}
}

///////////////////////////////////////////////////////////////////////////////
/**
 *  @brief ���������Ϣ
 */	
void CMJFrameLogic::OnNotifyBaseInfo(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_MJ_BASE_INFO * pCmd = (TCMD_NOTIFY_MJ_BASE_INFO*)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	m_tagSetting.nOpenGM      = pCmd->nOpenGM;                 // GM�����
	m_tagSetting.nTimerGive   = pCmd->ucTimerGive;			   // ����ʱ��
	m_tagSetting.nTimerFlower = pCmd->ucTimerFlower;		   // ����ʱ��
	m_tagSetting.nTimerBlock  = pCmd->ucTimerBlock;		       // ����ʱ��
	m_tagSetting.nTimerReady  = pCmd->ucTimerReady;		       // ׼������ʱ

	m_tagSetting.nTimerCutCard= pCmd->ucTimerCutCard;          // ���Ʋ���ʱ��
	m_tagSetting.nTimerDice   = pCmd->ucTimerDice;             // ���Ӳ���ʱ��
	m_tagSetting.nTimerDeal   = pCmd->ucTimerDeal;             // ���Ʋ���ʱ��
	m_tagSetting.nTimerJP     = pCmd->ucTimerJP;               // ���Ʋ���ʱ��

	m_tagSetting.nTilesHandNbr = pCmd->ucTilesHandNbr;         // ��������
	m_tagSetting.nTilesNumber = pCmd->ucTilesNumber;           // ��������

	m_tagSetting.nGangScore = pCmd->ucGangScore;               // ���Ƶĵ׷�
	m_tagSetting.nMinFan    = pCmd->ucMinFan;                  // ��С��
	m_tagSetting.nBaseScore = pCmd->ucBaseScore;               // ��Ϸ�׷�
	m_tagSetting.nMoneyType = pCmd->ucMoneyType;               // �۷�����
}

/**
 *  @brief ���ܵ����롢�ػء����ݻָ���Ϣ
 */	
void CMJFrameLogic::OnNotifyGameInfo(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_VIEW_INFO * pCmd = (TCMD_NOTIFY_VIEW_INFO *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	m_byMyStation = m_pGameFrame->GetMyDeskStation();

	m_byGameState = pCmd->ucGameState;

	if (m_byGameState == MJ_STATE_NOSTART || m_byGameState == MJ_STATE_END)
	{
		// ��ʾ׼����Ϣ
		for (BYTE i = 0; i < PLAYER_NBR; i++)
		{
			m_bReady[i] = pCmd->ucReady[i];
			DrawReady(i, pCmd->ucReady[i]);
		}

		// ׼����ť
		SAFE_CTRL_OPERATE(IButton, CTN_BTN_Start, SetControlVisible(pCmd->ucReady[m_byMyStation] == 0));

		if (!pCmd->ucReady[m_byMyStation])
		{
			SetGameTimer(m_byMyStation, m_tagSetting.nTimerReady);
		}

		return ;
	}

	int i, j;
	
	// ��ǰ�������
	m_byTurnStation = pCmd->ucCurTurn;

	// ׯ��
	m_byDealer = pCmd->ucDealer;
	DrawDealer(m_byDealer, true);

	for (i = 0; i < DICE_NBR; i++)
	{
		// ����
		m_byDice[i] = pCmd->ucDice[i];
	}

	// �йܱ�־��ԭ
	for (i = 0; i < PLAYER_NBR; i++)
	{
		m_bTuoGuan[i] = pCmd->ucTrusteeFlag[i];
		DrawTuoGuan(i, m_bTuoGuan[i]);
	}

	// �����ƻ�ԭ
	m_TilesGod.ReleaseAll();
	for (i = 0; i < pCmd->ucGodTilesLength; i++)
	{
		m_TilesGod.AddTile(pCmd->ucGodTiles[i]);
	}

	// �������ݻ�ԭ
	for (i = 0; i < PLAYER_NBR; i++)
	{
		m_TilesHand[i].ReleaseAll();

		if (i == m_byMyStation)
		{
			for (j = 0; j < pCmd->ucHandTilesLength[i]; j++)
			{
				m_TilesHand[i].AddTile(pCmd->ucHandTiles[j]);
			}

			m_TilesHand[i].Sort();
		}
		else
		{
			m_TilesHand[i].CurrentLength(pCmd->ucHandTilesLength[i]);
		}
		
		// ��������
		DrawHand(i, m_TilesHand[i], m_TilesHand[i].CurrentLength() % 3 == 2);
	}

	// �������ݻ�ԭ
	for (i = 0; i < PLAYER_NBR; i++)
	{
		m_TilesGive[i].ReleaseAll();

		for (j = 0; j < pCmd->ucGiveTilesLength[i]; j++)
		{
			m_TilesGive[i].AddTile(pCmd->ucGiveTiles[i][j]);
		}

		// ���Ƴ���
		DrawGive(i, m_TilesGive[i]);
	}

	// �������ݻ�ԭ
	TILE_SET tileSet;
	for (i = 0; i < PLAYER_NBR; i++)
	{
		m_TilesSet[i].ReleaseAll();

		for (j = 0; j < pCmd->ucSetTilesLength[i]; j++)
		{
			tileSet.m_ucFlag = pCmd->ucBlockTilesFlag[i][j];
			tileSet.m_ucChair = pCmd->ucBlockTilesWhich[i][j];
			::CopyMemory(&tileSet.m_ucTile, &pCmd->ucBlockTiles[i][j * 4], sizeof(tileSet.m_ucTile));
			m_TilesSet[i].AddSet(tileSet);
		}

		// ��������
		DrawSet(i, m_TilesSet[i]);
	}

	// ��ǽ��ԭ
	m_TilesWall.ReleaseAll();
	m_TilesWall.CurrentLength(m_tagSetting.nTilesNumber);
	BYTE byStation = m_pGameFrame->DeskStation2View(m_byDealer);   // ץ�Ʒ���
	int nPos = (m_byDice[0] + m_byDice[1]) * 2;                    // �ӵڼ��տ�ʼץ
	int nStartPos, nCount = 0;

	for (i = 0; i < PLAYER_NBR; i++)
	{
		for (j = 0; j < m_tagSetting.nTilesHandNbr; j++)
		{
			nStartPos = byStation * MJ_WALL_NUMBER + nPos + nCount;
			if (nStartPos >= m_tagSetting.nTilesNumber)
			{
				nStartPos = nStartPos - m_tagSetting.nTilesNumber; 
			}

			m_TilesWall.SetTile(nStartPos,  -1);

			nCount++;
		}

		if (i == m_byDealer)
		{
			nStartPos = byStation * MJ_WALL_NUMBER + nPos + nCount;
			if (nStartPos >= m_tagSetting.nTilesNumber)
			{
				nStartPos = nStartPos - m_tagSetting.nTilesNumber; 
			}

			// ׯ�Ҷ�ץһ��
			m_TilesWall.SetTile(nStartPos,  -1);
			nCount++;
		}
	}

	// ������ǽ��ץ���벹��λ��
	m_TilesWall.SetPos(byStation * MJ_WALL_NUMBER + nPos + nCount, byStation * MJ_WALL_NUMBER + nPos - 1);

	
	//  �ж�״̬, ����״̬ʱ���ƻ�δ����, �Ƿ���״̬ʱ��Ҫ���þ�������ǽ�е�λ��
	if (pCmd->ucGameState != MJ_STATE_DEAL)
	{
		////////////////////////////////////////////////////////////////////////////////////////
		// ����������(����������)
		for (i = 0; i < pCmd->ucDarwTileLength; i++)
		{
			m_TilesWall.PopTile();
		}

		// ����������
		for (i = 0; i < pCmd->ucGangTileLength; i++)
		{
			m_TilesWall.PopFrontTile();
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		// ������������ƿؼ�����
		int nGodTiles[2];
		m_TilesGod.GetAllTile(nGodTiles);

		IMjHandCtrl * pHandCtrl = NULL;
		IMjGiveCtrl * pGiveCtrl = NULL;
		for (BYTE i = 0; i < PLAY_COUNT; i++)
		{
			GETCTRL(IMjHandCtrl, pHandCtrl, m_pUI, CTN_MJCtrl_HandCtrl_0 + i);
			if (pHandCtrl != NULL)
			{
				pHandCtrl->SetGodTile(nGodTiles, m_TilesGod.CurrentLength());
			}

			GETCTRL(IMjHandCtrl, pHandCtrl, m_pUI, CTN_MJCtrl_GiveCtrl_0 + i);
			if (pGiveCtrl != NULL)
			{
				pGiveCtrl->SetGodTile(nGodTiles, m_TilesGod.CurrentLength());
			}
		}
	}
	
	// ������ǽ
	DrawWall(true);

	if (pCmd->ucGameState == MJ_STATE_DEAL)
	{
		// �������, ��Ҫ֪ͨ�����
		TCMD_REQUEST_DEAL_FINISH cmd; 
		m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));
	}
	else
	{
		SetGameTimer(pCmd->ucCurTurn, pCmd->ucNowTime);
	}
}

/**
 *  @brief ��������������Ϣ(��Ϸ��ʼ���͵ĵĵ�һ����Ϸ��Ϣ)
 */	
void CMJFrameLogic::OnNotifyRunNbr(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_RUN_NBR * pCmd = (TCMD_NOTIFY_RUN_NBR *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	m_byGameState = MJ_STATE_DEAL;

	// ��������ǳ�
	memset(&m_wNickName, 0, sizeof(m_wNickName));
	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		C2W(m_pGameFrame->GetUserNickName(i), m_wNickName[i]);
	}
}

/**
 *  @brief ���ܵ�֪ͨׯ��λ����Ϣ
 */	
void CMJFrameLogic::OnNotifyDealer(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_DEALER * pCmd = (TCMD_NOTIFY_DEALER *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	m_byDealer = pCmd->byDealer;

	DrawDealer(m_byDealer, true);
}

/**
 *  @brief ���ܵ����ַ�����Ϣ��
 */	
void CMJFrameLogic::OnNotifyDeal(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_DEAL * pCmd = (TCMD_NOTIFY_DEAL*)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	CopyMemory(&m_stDealInfo, pCmd, sizeof(TCMD_NOTIFY_DEAL));

	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		DrawReady(i, false);
	}

	// ����״̬
	m_byGameState = MJ_STATE_DEAL;

	CMJHand mjHand;
	for (BYTE i = 0; i < 17; i++)
	{
		if (pCmd->tiles[i] != TILE_BEGIN)
		{
			mjHand.AddTile(pCmd->tiles[i]);
		}
	}

	for (BYTE i = 0; i < PLAYER_NBR; i++)
	{
		m_TilesHand[i].ReleaseAll();
		if (i == m_byMyStation)
		{
			m_TilesHand[i].AddTiles(mjHand);
			m_TilesHand[i].Sort();
		}
		else
		{
			if (i == m_byDealer)
			{
				m_TilesHand[i].AddTile(TILE_BEGIN, m_tagSetting.nTilesHandNbr + 1);
			}
			else
			{
				m_TilesHand[i].AddTile(TILE_BEGIN, m_tagSetting.nTilesHandNbr);
			}
		}

		//DrawHand(i, m_TilesHand[i]);
	}

	m_byDealer = pCmd->ucDealer;

	// ����
	m_byDice[0] = pCmd->ucDice[0];
	m_byDice[1] = pCmd->ucDice[1];

	// ��1�δ�����
	m_enDiceState = En_Dice_1;

	// ��ʾׯ��
	DrawDealer(m_byDealer, true);

	// �������Ӷ���
	DrawDice(pCmd->ucDealer, m_byDice, true);
}


/**
 *  @brief ������������Ϣ
 */	
void CMJFrameLogic::OnNotifyFlowerOver(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_FLOWER_OVER * pCmd = (TCMD_NOTIFY_FLOWER_OVER *)buffer;
	if (buffer == NULL)
	{
		return ;
	}

	// ��������
	DrawDice(m_byDealer, m_byDice, false);

	// �ж��Ƿ������Ʋ���
	if (pCmd->ucFlag == ACTION_EMPTY)
	{
		// ����״̬
		m_byGameState = MJ_STATE_GIVE;

		m_byTurnStation = m_byDealer;

		// ���ó��ƶ�ʱ��
		SetGameTimer(m_byTurnStation, m_tagSetting.nTimerGive);

		if (m_byTurnStation == m_byMyStation && m_bTuoGuan[m_byMyStation])
		{
			// �Զ�����
			OnRequestGive(TILE_BEGIN, true);
		}
	}
	else
	{
		OnNotifyBlock(buffer, nLen);
	}
}

/**
 *  @brief ���ܵ�������Ϣ
 */	
void CMJFrameLogic::OnNotifyDraw(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_DRAW * pCmd = (TCMD_NOTIFY_DRAW *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	m_byStateSelect = MJ_STATE_SELECT_NONE;

	if (pCmd->chair == m_byMyStation)
	{
		m_TilesHand[pCmd->chair].AddTile(pCmd->t);
	}
	else
	{
		m_TilesHand[pCmd->chair].AddTile(TILE_BEGIN);
	}

	// ��ǽ����
	if (pCmd->t_target == MJ_GANG_STATE_DRAW)
	{
		// ���ƻ򲹻�����
		m_TilesWall.PopFrontTile();
	}
	else
	{
		// ��������
		m_TilesWall.PopTile();
	}
	
	// ������ǽ
	DrawWall(true);

	// ������������
	DrawHand(pCmd->chair, m_TilesHand[pCmd->chair], true);

	// ��ǰ������
	m_byTurnStation = pCmd->chair;

	// ����״̬
	m_byGameState = MJ_STATE_GIVE;

	// �ж��Ƿ������Ʋ���
	if (pCmd->ucFlag == ACTION_EMPTY)
	{
		// ���ó��ƶ�ʱ��
		SetGameTimer(m_byTurnStation, m_tagSetting.nTimerGive);

		if (m_byTurnStation == m_byMyStation && m_bTuoGuan[m_byMyStation])
		{
			// �Զ�����
			OnRequestGive(TILE_BEGIN, true);
		}

		// ����ץ����Ч
		PlaySound(SOUND_ZHUAPAI);
	}
	else
	{
		OnNotifyBlock(buffer, nLen);
	}
}

/**
 *  @brief ���ܵ�������ƽ����Ϣ
 */	
void CMJFrameLogic::OnNotifyGive(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_GIVE * pCmd = (TCMD_NOTIFY_GIVE *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	// ���س��������
	SAFE_CTRL_OPERATE(IContain, CTN_Block, SetControlVisible(false));

	// ȡ�����õ��ƿؼ�
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_HandCtrl_2, SetInvalidTile(NULL, 0));

	// ����������ʾ��Ϣ
	SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(false));

	m_TilesGive[pCmd->chair].AddTile(pCmd->t);

	m_LastTile = pCmd->t;

	m_byGameState = MJ_STATE_WAIT;

	m_byStateSelect = MJ_STATE_SELECT_NONE;

	// ������ֵ��Ч
	PlaySound(SOUND_OUTCARDRAND[rand() % 3]);
	PlaySound(pCmd->t, pCmd->chair);

	// ���Ƴ�������
	DrawGive(pCmd->chair, m_TilesGive[pCmd->chair]);

	// ���Ƴ��Ʊ��
	DrawGiveAnimate(pCmd->chair, true);

	if (pCmd->chair == m_byMyStation)
	{
		m_TilesHand[pCmd->chair].DelTile(pCmd->t);
		m_TilesHand[pCmd->chair].Sort();
	}
	else
	{
		m_TilesHand[pCmd->chair].DelTile(TILE_BEGIN);
	}

	// ������������
	DrawHand(pCmd->chair, m_TilesHand[pCmd->chair]);
}

/**
 *  @brief ���ܵ�������Ϣ
 */	
void CMJFrameLogic::OnNotifyBlock(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_BLOCK * pCmd = (TCMD_NOTIFY_BLOCK *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	if (pCmd->ucFlag == ACTION_EMPTY)
	{
		// ����û��������Ҷ�ʱ������ʱ��
		SetGameTimer(m_byTurnStation, m_tagSetting.nTimerBlock);
		m_byTurnStation = -1;
		return ;
	}

	// ����״̬
	m_byGameState = MJ_STATE_BLOCK;

	m_byTurnStation = pCmd->ucChair;

	// �����������
	m_stBlockInfo.ReleaseAll();

	m_stBlockInfo.byFlag = pCmd->ucFlag;

	TILE_SET tileSet;

	// ��
	if (pCmd->ucFlag & 0x01)
	{ 
		tileSet.m_ucFlag = ACTION_COLLECT;

		for (int i = 0; i < 4; i++)
		{
			if (pCmd->ucTile[0][i * 3] != TILE_BEGIN)
			{
				memset(&tileSet.m_ucTile, 0, sizeof(tileSet.m_ucTile));
				CopyMemory(tileSet.m_ucTile, &pCmd->ucTile[0][i * 3], sizeof(BYTE) * 3);
				m_stBlockInfo.tilesSet.AddSet(tileSet);
			}
		}
	}

	// ��
	if (pCmd->ucFlag & 0x02)
	{
		tileSet.m_ucFlag = ACTION_TRIPLET;
		CopyMemory(tileSet.m_ucTile, &pCmd->ucTile[1][0], sizeof(BYTE) * 3);
		m_stBlockInfo.tilesSet.AddSet(tileSet);
	}

	// ��
	if (pCmd->ucFlag & 0x04)
	{
		// pCmd->ucTile ���������6����
		for (int i = 0; i < 6; i++)
		{
			if (pCmd->ucTile[2][i * 4] != 0)
			{
				memset(&tileSet.m_ucTile, 0, sizeof(tileSet.m_ucTile));
				CopyMemory(tileSet.m_ucTile, &pCmd->ucTile[2][i * 4], sizeof(BYTE) * 4);
				
				// �����ÿ�ָ�������
				CMJHand mjHand;
				mjHand.AddTile(tileSet.m_ucTile[0]);
				mjHand.AddTile(tileSet.m_ucTile[1]);
				mjHand.AddTile(tileSet.m_ucTile[2]);
				mjHand.AddTile(tileSet.m_ucTile[3]);
				if (mjHand.IsHave(TILE_EAST) && mjHand.IsHave(TILE_SOUTH) && mjHand.IsHave(TILE_WEST) && mjHand.IsHave(TILE_NORTH))
				{
					tileSet.m_ucFlag = ACTION_QUADRUPLET_DNXB;      // ����������
				}
				else if (mjHand.IsHave(TILE_ZHONG) && mjHand.IsHave(TILE_FA) && mjHand.IsHave(TILE_BAI)
					 && (mjHand.IsHave(TILE_ZHONG, 2) || mjHand.IsHave(TILE_FA, 2) || mjHand.IsHave(TILE_BAI, 2)))
				{
					tileSet.m_ucFlag = ACTION_QUADRUPLET_ZFB;       // �з���X��
				}
				else if (m_TilesHand[m_byMyStation].GetTileCount(tileSet.m_ucTile[0]) == 4)
				{
					tileSet.m_ucFlag = ACTION_QUADRUPLET_CONCEALED; // ����
				}
				else if (m_TilesHand[m_byMyStation].GetTileCount(tileSet.m_ucTile[0]) == 3)
				{
					tileSet.m_ucFlag = ACTION_QUADRUPLET_REVEALED;  // ����
				}
				else
				{
					tileSet.m_ucFlag = ACTION_QUADRUPLET_PATCH;     // ����
				}

				m_stBlockInfo.tilesSet.AddSet(tileSet);
			}
		}
	}

	// ��
	if (pCmd->ucFlag & 0x08)
	{
		for (int i = 0; i < 24; i++)
		{
			if (pCmd->ucTile[3][i] != TILE_BEGIN)
			{
				m_stBlockInfo.tingGive.AddTile(pCmd->ucTile[3][i]);

				for (int j = 0; j < 15; j++)
				{
					if (pCmd->ucTingTile[i][j] != TILE_BEGIN)
					{
						m_stBlockInfo.tingWin[i].AddTile(pCmd->ucTingTile[i][j]);
					}
				}
			}
		}
	}

	// �������ƿ�
	DrawBlock(pCmd->ucFlag);

	// �������ƶ�ʱ��
	SetGameTimer(pCmd->ucChair, m_tagSetting.nTimerBlock);

	if (m_byTurnStation == m_byMyStation && m_bTuoGuan[m_byMyStation])
	{
		// �Զ�����
		OnRequestGive(TILE_BEGIN, true);
	}
	else
	{
		// ����������ʾ��Ч
		PlaySound(SOUND_BLOCK);
	}
}

/**
 *  @brief ���ܵ��������ƽ����Ϣ
 */	
void CMJFrameLogic::OnNotifyBlockResult(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_BLOCK_RESULT * pCmd = (TCMD_NOTIFY_BLOCK_RESULT *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	SAFE_CTRL_OPERATE(IContain, CTN_Block, SetControlVisible(false));

	switch (pCmd->ucFlag)
	{
	case ACTION_COLLECT:  // ��
		{
			PlaySound(SOUND_CHI, pCmd->ucChair);

			// ɾ������
			if (pCmd->ucChair == m_byMyStation)
			{
				for (int i = 0; i < 3; i++)
				{
					if (pCmd->ucTile[i] != m_LastTile)
					{
						m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[i]);
					}
				}
			}
			else
			{
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
			}

			// ɾ�������������
			m_TilesGive[pCmd->ucWhich].CurrentLength(m_TilesGive[pCmd->ucWhich].CurrentLength() - 1);
			DrawGive(pCmd->ucWhich, m_TilesGive[pCmd->ucWhich]);

			// ���س��Ʊ��
			DrawGiveAnimate(pCmd->ucWhich, false);
		}
		break;
	case ACTION_TRIPLET:  // ��
		{
			PlaySound(SOUND_PENG, pCmd->ucChair);

			// ɾ������
			if (pCmd->ucChair == m_byMyStation)
			{
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[0]);
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[0]);
			}
			else
			{
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
			}

			// ɾ�������������
			m_TilesGive[pCmd->ucWhich].CurrentLength(m_TilesGive[pCmd->ucWhich].CurrentLength() - 1);
			DrawGive(pCmd->ucWhich, m_TilesGive[pCmd->ucWhich]);

			// ���س��Ʊ��
			DrawGiveAnimate(pCmd->ucWhich, false);
		}
		break;
	case ACTION_QUADRUPLET_CONCEALED: // ����
	case ACTION_QUADRUPLET_DNXB:      // ����������
	case ACTION_QUADRUPLET_ZFB:       // �з���X��
		{
			PlaySound(SOUND_GANG, pCmd->ucChair);

			// ɾ������
			if (pCmd->ucChair == m_byMyStation)
			{
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[0]);
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[1]);
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[2]);
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[3]);
			}
			else
			{
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
			}

			m_byGangCount++;
		}
		break;
	case ACTION_QUADRUPLET_REVEALED:  // ����
		{
			PlaySound(SOUND_GANG, pCmd->ucChair);

			// ɾ������
			if (pCmd->ucChair == m_byMyStation)
			{
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[0]);
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[1]);
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[2]);
			}
			else
			{
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
			}

			// ɾ�������������
			m_TilesGive[pCmd->ucWhich].CurrentLength(m_TilesGive[pCmd->ucWhich].CurrentLength() - 1);
			DrawGive(pCmd->ucWhich, m_TilesGive[pCmd->ucWhich]);

			// ���س��Ʊ��
			DrawGiveAnimate(pCmd->ucWhich, false);

			m_byGangCount++;
		}
		break;
	case ACTION_QUADRUPLET_PATCH:     // ����
		{
			PlaySound(SOUND_GANG, pCmd->ucChair);

			// ɾ������
			if (pCmd->ucChair == m_byMyStation)
			{
				m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile[0]);
			}
			else
			{
				m_TilesHand[pCmd->ucChair].DelTile(TILE_BEGIN);
			}

			TILE_SET tileSet;
			for (int i = 0; i < 5; i++)
			{
				m_TilesSet[pCmd->ucChair].GetSet(i, tileSet);
				if (tileSet.m_ucFlag == ACTION_TRIPLET && tileSet.m_ucTile[0] == pCmd->ucTile[0])
				{
					tileSet.m_ucFlag = ACTION_QUADRUPLET_PATCH;
					tileSet.m_ucTile[3] = pCmd->ucTile[0];
					m_TilesSet[pCmd->ucChair].SetSet(i, tileSet);
				}
			}

			m_byGangCount++;
		}
		break;
	case ACTION_WIN:  // ��
		{
			return ;
		}
		break;
	default:
		break;
	}

	// ��������������������, ǰ���Ѿ�����, �������ӷǲ�������
	if (pCmd->ucFlag != ACTION_QUADRUPLET_PATCH)
	{
		m_TilesSet[pCmd->ucChair].AddSet(pCmd->ucTile, pCmd->ucFlag, pCmd->ucWhich);
	}

	// ���Ŷ�����Ч
	PlaySound(SOUND_BLOCK);

	// ����״̬
	m_byGameState = MJ_STATE_GIVE;

	m_byTurnStation = pCmd->ucChair;

	// ������������
	DrawHand(pCmd->ucChair, m_TilesHand[pCmd->ucChair]);

	// ������������
	DrawSet(pCmd->ucChair, m_TilesSet[pCmd->ucChair]);

	// ���ó���ʱ��
	SetGameTimer(m_byTurnStation, m_tagSetting.nTimerGive);
}

/**
 *  @brief ���ܵ�������Ϣ
 */	
void CMJFrameLogic::OnNotifyFinish(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_FINISH * pCmd = (TCMD_NOTIFY_FINISH *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	// ����״̬
	m_byGameState = MJ_STATE_END;

	// ֹͣ��ʱ��
	KillAllTimer();

	CopyMemory(&m_stFinishInfo, pCmd, sizeof(TCMD_NOTIFY_FINISH));

	
	if (pCmd->byFlag == MJ_END_SELF || pCmd->byFlag == MJ_END_GUN || pCmd->byFlag == MJ_END_QIANG)
	{
		// ���������, ���ź��ƶ���
		IExAnimate * pExAnimate = NULL;
		GETCTRL(IExAnimate, pExAnimate, m_pUI, ExAnimate_HuPai);
		if (pExAnimate != NULL)
		{
			pExAnimate->SetControlVisible(true);
			pExAnimate->SetIsSymmetrical(false);
			pExAnimate->SetIsLoop(false);
			pExAnimate->SetPlayState(true);
		}
	
		for (BYTE i = 0; i < PLAYER_NBR; i++)
		{
			if (pCmd->byWhoWin[i])
			{
				// �����������Ч
				PlaySound(pCmd->byFlag == MJ_END_SELF ? SOUND_ZIMO : SOUND_DIAN_PAO, i);
				break;
			}
		}
	}
	else
	{
		// ����
		PlaySound(SOUND_LIUJU);
	}

	int i, j;
	for (i = 0; i < PLAYER_NBR; i++)
	{
		// ���������������
		m_TilesHand[i].ReleaseAll();

		for (j = 0; j < 17; j++)
		{
			if (pCmd->tile[i][j] != TILE_BEGIN)
			{
				m_TilesHand[i].AddTile(pCmd->tile[i][j]);
			}
		}

		if (pCmd->byWhoWin[i] && m_TilesHand[i].CurrentLength() % 3 == 2)
		{
			// �Ѻ��������Ƶ������ұ���ʾ
			m_TilesHand[i].DelTile(pCmd->tLast);
			m_TilesHand[i].Sort();
			m_TilesHand[i].AddTile(pCmd->tLast);
		}

		DrawHand(i, m_TilesHand[i], m_TilesHand[i].CurrentLength() % 3 == 2, true);


		// ���������������
		m_TilesSet[i].ReleaseAll();
		for (j = 0; j < 5; j++)
		{
			if (pCmd->byBlockTilesFlag[i][j] != ACTION_EMPTY)
			{
				TILE_SET tileSet;
				tileSet.m_ucFlag = pCmd->byBlockTilesFlag[i][j];
				tileSet.m_ucChair = pCmd->byBlockTilesWhich[i][j];
				CopyMemory(tileSet.m_ucTile, &pCmd->byBlockTiles[i][j * 4], sizeof(tileSet.m_ucTile));
				m_TilesSet[i].AddSet(tileSet);
			}
		}

		DrawSet(i, m_TilesSet[i]);
	}

	// 2�����ʾ�����
	m_pGameFrame->SetTimer(TIME_SHOW_FINISH, 2000);
}

/**
 *  @brief ���ܵ��޸��Թ�״̬��Ϣ
 */	
void CMJFrameLogic::OnNotifyChangeLookOn(BYTE * buffer, int nLen)
{
}

/**
 *  @brief ���ܵ�ˢ��������Ϣ��
 */	
void CMJFrameLogic::OnNotifyUpdateHandTiles(BYTE * buffer, int nLen)
{
	TCMD_NOTIFY_UPDATEHANDTILES * pCmd = (TCMD_NOTIFY_UPDATEHANDTILES *)buffer;
	if (pCmd == NULL)
	{
		return ;
	}

	if (pCmd->ucChair != m_byMyStation)
	{
		return;
	}

	m_TilesHand[pCmd->ucChair].CurrentLength(pCmd->ucHandCount);

	for (int i = 0; i < pCmd->ucHandCount; ++i )
	{
		m_TilesHand[pCmd->ucChair].SetTile(i, pCmd->ucHandTiles[i]);
	}

	m_TilesHand[pCmd->ucChair].Sort();

	// ������������
	DrawHand(pCmd->ucChair, m_TilesHand[pCmd->ucChair], m_TilesHand[pCmd->ucChair].CurrentLength() % 3 == 2);
}

/**
 *  @brief ���ܵ�GM������Ϣ��ִ�е��¼�
 */
void CMJFrameLogic::OnNotifyGMCmd(BYTE * buffer, int nLen)
{
    TCMD_NOTIFY_GMCMD * pCmd = (TCMD_NOTIFY_GMCMD *)buffer;
    if (pCmd == NULL)
    {
        return ;
    }

    //m_pGameFrame->InsertNormalMessage(pCmd->strGMResult);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/** 
 * @brief ����GM����
 */
void CMJFrameLogic::OnRequestGMCmd()
{
	if (m_tagSetting.nOpenGM == 1)
	{
		IEditorText *pEdit = NULL;
		GETCTRL(IEditorText, pEdit, m_pUI, CTN_EDITORTEXT_GM);
		if (pEdit != NULL)
		{
			TCMD_REQUEST_GMCMD cmd;
			swprintf(cmd.strGMCmd, MAX_PATH, L"%s", pEdit->GetText());
			m_pGameFrame->SendGameData(0, (void *)&cmd, sizeof(cmd));
		}
	}
}

/** 
 * @brief �������                
 * @param [in] uControlID �ؼ�ID
 * @param [in] bAutoGive  �Ƿ��Զ�����
 */
void CMJFrameLogic::OnRequestGive(UINT32 uControlID, bool bAutoGive)
{
	if (m_byTurnStation != m_byMyStation)
	{
		return ;  // �ǵ�ǰ������Ҳ��ܳ���
	}

	if (m_byGameState == MJ_STATE_BLOCK && m_byStateSelect == MJ_STATE_SELECT_TING)
	{
		// ����ѡ��
		TCMD_REQUEST_BLOCK cmd;
		cmd.ucFlag = ACTION_TING;
		m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));

		// ȡ�����õ��ƿؼ�
		SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_HandCtrl_2, SetInvalidTile(NULL, 0));

		// ����������ʾ��Ϣ
		SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(false));

		m_byStateSelect = MJ_STATE_SELECT_NONE;
		m_byGameState = MJ_STATE_GIVE;
	}

	if (m_byGameState != MJ_STATE_GIVE && m_byGameState != MJ_STATE_BLOCK)
	{
		return ;  // ����״̬����
	}

	if (m_byGameState == MJ_STATE_BLOCK)
	{
		// �������ʱ, ��������״̬
		// ���س��������
		SAFE_CTRL_OPERATE(IContain, CTN_Block, SetControlVisible(false));

		// ȡ������, ������Ժ��ƣ���Ĭ�Ϻ���
		TCMD_REQUEST_BLOCK cmd;
		cmd.ucFlag = ((m_stBlockInfo.byFlag & 0x10) && bAutoGive) ? ACTION_WIN : ACTION_EMPTY;
		m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));
	}

	TILE tile = TILE_BEGIN;
	IMjHandCtrl * pMJCtrl = NULL;
	GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_2);
	if (pMJCtrl != NULL)
	{
		tile = pMJCtrl->GetUpTile();
	}

	TCMD_REQUEST_GIVE cmd;
	cmd.t = tile;
	cmd.ucTileLength = m_TilesHand[m_byMyStation].CurrentLength() - 1;  

    if (bAutoGive || tile == TILE_BEGIN)
	{
		cmd.t = m_TilesHand[m_byMyStation].GetTile(cmd.ucTileLength);
	}
	else
	{
		m_nTimeOutCount = 0;   // ���Ƴ��ƽ���ճ�ʱ��¼
	}

	m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));
}

/** 
 * @brief ��������              
 * @param [in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnRequestBlock(UINT32 uControlID)
{
	SAFE_CTRL_OPERATE(IContain, CTN_Block, SetControlVisible(false));

	if (m_byTurnStation != m_byMyStation)
	{
		return ;  // �ǵ�ǰ������Ҳ��ܳ���
	}

	if (m_byGameState != MJ_STATE_BLOCK)
	{
		return ;  // ����״̬����
	}

	CMJHand mjHand;
	int nTiels[20];
	int i, j, nCount = 0;
	TILE tile;
	TILE_SET tileSet;

	TCMD_REQUEST_BLOCK cmd;
	cmd.ucFlag = ACTION_EMPTY;

	// �ԡ������ܡ���������ȡ��
	switch (uControlID)
	{
	case CTN_Block_BTN_Chi:   // ��
		{
			cmd.ucFlag = ACTION_COLLECT;

			for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
			{
				m_stBlockInfo.tilesSet.GetSet(i, tileSet);
				if (tileSet.m_ucFlag == ACTION_COLLECT)
				{
					for (j = 0; j < 3; j++)
					{
						mjHand.AddTile(tileSet.m_ucTile[j]);
						cmd.ucTile[j] = tileSet.m_ucTile[j];
					}

					nCount++;
				}
			}

			if (nCount > 1)
			{
				// ���ڶ��ֳԷ�, �������ѡ��״̬
				m_byStateSelect = MJ_STATE_SELECT_COLLECT;

				nCount = 0;
				for (i = 0; i < m_TilesHand[m_byMyStation].CurrentLength(); i++)
				{
					tile = m_TilesHand[m_byMyStation].GetTile(i);
					if (!mjHand.IsHave(tile))
					{
						nTiels[nCount++] = tile;   // ��Ҫ���õ���
					}
				}

				IMjHandCtrl * pMJCtrl = NULL;
				GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_2);
				if (pMJCtrl != NULL)
				{
					// ���ý���
					pMJCtrl->SetInvalidTile(nTiels, nCount);
				}

				return ;
			}
		}
		break;
	case CTN_Block_BTN_Peng:  // ��
		{
			cmd.ucFlag = ACTION_TRIPLET;
			for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
			{
				m_stBlockInfo.tilesSet.GetSet(i, tileSet);
				if (tileSet.m_ucFlag == ACTION_TRIPLET)
				{
					cmd.ucTile[0] = tileSet.m_ucTile[0];
					cmd.ucTile[1] = tileSet.m_ucTile[1];
					cmd.ucTile[2] = tileSet.m_ucTile[2];
					break;
				}
			}
		}
		break;
	case CTN_Block_BTN_Gang:  // ��
		{
			for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
			{
				m_stBlockInfo.tilesSet.GetSet(i, tileSet);
				if (tileSet.m_ucFlag == ACTION_QUADRUPLET_CONCEALED
				 || tileSet.m_ucFlag == ACTION_QUADRUPLET_REVEALED
				 || tileSet.m_ucFlag == ACTION_QUADRUPLET_PATCH
				 || tileSet.m_ucFlag == ACTION_QUADRUPLET_DNXB
				 || tileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
				{
					for (j = 0; j < 4; j++)
					{
						mjHand.AddTile(tileSet.m_ucTile[j]);
						cmd.ucTile[j] = tileSet.m_ucTile[j];
					}

					nCount++;
				}
			}

			if (nCount > 1)
			{
				// ���ڶ��ָ�, �������ѡ��״̬
				m_byStateSelect = MJ_STATE_SELECT_QUADRUPLET;

				nCount = 0;
				for (i = 0; i < m_TilesHand[m_byMyStation].CurrentLength(); i++)
				{
					tile = m_TilesHand[m_byMyStation].GetTile(i);
					if (!mjHand.IsHave(tile))
					{
						nTiels[nCount++] = tile;   // ��Ҫ���õ���
					}
				}

				IMjHandCtrl * pMJCtrl = NULL;
				GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_2);
				if (pMJCtrl != NULL)
				{
					// ���ý���
					pMJCtrl->SetInvalidTile(nTiels, nCount);
				}

				return ;
			}
			else
			{
				
				if (m_TilesHand[m_byMyStation].IsHave(cmd.ucTile[0], 4))
				{
					cmd.ucFlag = ACTION_QUADRUPLET_CONCEALED; // ����
				}
				else if (m_TilesHand[m_byMyStation].IsHave(cmd.ucTile[0], 3))
				{
					cmd.ucFlag = ACTION_QUADRUPLET_REVEALED;  // ����
				}
				else
				{
					CMJHand mjHand;
					mjHand.AddTile(cmd.ucTile[0]);
					mjHand.AddTile(cmd.ucTile[1]);
					mjHand.AddTile(cmd.ucTile[2]);
					mjHand.AddTile(cmd.ucTile[3]);

					if (mjHand.IsHave(TILE_EAST) && mjHand.IsHave(TILE_SOUTH) && mjHand.IsHave(TILE_WEST) && mjHand.IsHave(TILE_NORTH))
					{
						cmd.ucFlag = ACTION_QUADRUPLET_DNXB;      // ����������
					}
					else if (mjHand.IsHave(TILE_ZHONG) && mjHand.IsHave(TILE_FA) && mjHand.IsHave(TILE_BAI)
					     && (mjHand.IsHave(TILE_ZHONG, 2) || mjHand.IsHave(TILE_FA, 2) || mjHand.IsHave(TILE_BAI, 2)))
					{
						cmd.ucFlag = ACTION_QUADRUPLET_ZFB;       // �з���X��
					}
					else
					{
						cmd.ucFlag = ACTION_QUADRUPLET_PATCH;     // ����
					}
				}
			}
		}
		break;
	case CTN_Block_BTN_Ting:  // ��
		{
			// ��������ѡ��״̬
			m_byStateSelect = MJ_STATE_SELECT_TING;

			for (i = 0; i < m_TilesHand[m_byMyStation].CurrentLength(); i++)
			{
				tile = m_TilesHand[m_byMyStation].GetTile(i);
				if (!m_stBlockInfo.tingGive.IsHave(tile))
				{
					nTiels[nCount++] = tile;   // ���ƺ󣬲����Գ����ƶ��������
				}
			}

			IMjHandCtrl * pMJCtrl = NULL;
			GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_2);
			if (pMJCtrl != NULL)
			{
				// ���ý���
				pMJCtrl->SetInvalidTile(nTiels, nCount);
			}
			
			return ;
		}
		break;
	case CTN_Block_BTN_Hu:    // ��
		{
			cmd.ucFlag = ACTION_WIN;
		}
		break;
	case CTN_Block_BTN_Cancel:// ȡ��  
		{
			if (m_TilesHand[m_byMyStation].CurrentLength() % 3 == 2)
			{
				SetGameTimer(m_byTurnStation, m_tagSetting.nTimerGive);
			}
		}
		break;
	}

	m_byStateSelect = MJ_STATE_SELECT_NONE;
	m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));

	if (m_TilesHand[m_byMyStation].CurrentLength() % 3 == 2)
	{
		m_byGameState = MJ_STATE_GIVE;
	}
	else
	{
		// ����״̬
		m_byGameState = MJ_STATE_NONE;
	}
}

/** 
 * @brief ��������              
 * @param [in] uControlID �ؼ�ID
 */
void CMJFrameLogic::OnRequestBlockSelect(UINT32 uControlID)
{
	TILE tile = TILE_BEGIN;
	int  nIndex = -1;
	IMjHandCtrl * pMJCtrl = NULL;
	GETCTRL(IMjHandCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_HandCtrl_2);
	if (pMJCtrl != NULL)
	{
		// ��ȡ�������
		tile = pMJCtrl->GetUpTile();

		// ��ȡ�����λ��
		MJ_TILE_UIINFO tileInfo[20];
		pMJCtrl->GetTilesInfo(tileInfo, 20);
		for (int i = 0; i < m_TilesHand[m_byMyStation].CurrentLength(); i++)
		{
			if (tileInfo[i].bSelect && (tileInfo[i].dwReserve1 & 0x00000001) != 0x00000001 /*����ѡ*/ ) 
			{
				nIndex = i;
				break;
			}
		}
	}

	if (tile == TILE_BEGIN || nIndex < 0 && nIndex >= m_TilesHand[m_byMyStation].CurrentLength())
	{
		return ;  // �����ݻ�����λ�ô���
	}

	int i, j;
	TILE_SET tileSet;
	CMJHand mjHand;
	TCMD_REQUEST_BLOCK cmd;

	if (m_byStateSelect == MJ_STATE_SELECT_COLLECT)
	{
		// ����
		for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
		{
			m_stBlockInfo.tilesSet.GetSet(i, tileSet);
			if (tileSet.m_ucFlag == ACTION_COLLECT)
			{
				mjHand.ReleaseAll();
				for (j = 0; j < 3; j++)
				{
					cmd.ucTile[j] = tileSet.m_ucTile[j];
					mjHand.AddTile(tileSet.m_ucTile[j]);
				}

				// �ж��Ƿ�������������
				if (mjHand.IsHave(tile))
				{
					cmd.ucFlag = ACTION_COLLECT;
					break;
				}
			}
		}
	}
	else if (m_byStateSelect == MJ_STATE_SELECT_QUADRUPLET)
	{
		// ����
		for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
		{
			m_stBlockInfo.tilesSet.GetSet(i, tileSet);
			if (tileSet.m_ucFlag == ACTION_QUADRUPLET_CONCEALED
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_REVEALED
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_PATCH
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_DNXB
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
			{
				mjHand.ReleaseAll();
				for (j = 0; j < 4; j++)
				{
					mjHand.AddTile(tileSet.m_ucTile[j]);
					cmd.ucTile[j] = tileSet.m_ucTile[j];
				}

				// �ж��Ƿ�����������
				if (mjHand.IsHave(tile))
				{
					/////////////////////////////////////////////////////////////////////////////////
					if (tile >= TILE_EAST && tile <= TILE_BAI)
					{
						// �������ʾѡ��
						if (m_TilesHand[m_byMyStation].IsHave(tile, 4))
						{
							// ӵ��4����ͬ�Ƶĸ��ж�
							// ����: ���з����װװװ�, �ᵼ���з��װ����ָ���ϲ���ʾ, �ᱻ�װװװ׸�������ʾ, ���԰�4���Ʒֿ���ʾ, ǰ�������ʾ�з��װ�, ���������ʾ�װװװ�
							int nFirstPos = 0;
							for (j = 0; j < m_TilesHand[m_byMyStation].CurrentLength(); j++)
							{
								if (m_TilesHand[m_byMyStation].GetTile(j) == tile)
								{
									nFirstPos = j;   // ��¼��ʼλ��, ������ͬ�Ƶ�λ��ѡ����ʾ��ͬ��ʾ
									break;
								}
							}

							if (tile >= TILE_EAST && tile <= TILE_NORTH)
							{
								// ���ѡ�е��Ƕ�������, ��Ҫ�ж��Ƿ���4��һ���ĸܣ����Ƕ��������ָܷ�
								// �ж϶����������Ƿ����
								BOOL bGang = FALSE;
								TILE_SET tmpTileSet;
								for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
								{
									m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

									if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_DNXB)
									{
										bGang = TRUE;
										break;
									}
								}

								mjHand.ReleaseAll();

								if (bGang && nFirstPos + 1 < nIndex)
								{
									// ǰ��2��λ����ʾ����ܶ�������
									for (j = 0; j < 4; j++)
									{
										mjHand.AddTile(TILE_EAST + j);
									}
								}
								else
								{
									// ����2��λ����ʾ��ͨ��4����ͬ��
									for (j = 0; j < 4; j++)
									{
										mjHand.AddTile(tile);
									}
								}
							}
							else if (tile >= TILE_ZHONG && tile <= TILE_BAI)
							{
								// ���ѡ�е����з���X, ��Ҫ�ж��Ƿ���4��һ���ĸܣ������з���X��
								// �ж��з���X���Ƿ����
								BOOL bGang = FALSE;
								TILE_SET tmpTileSet;
								for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
								{
									m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

									if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
									{
										bGang = TRUE;
										break;
									}
								}

								mjHand.ReleaseAll();

								if (bGang && nFirstPos + 1 < nIndex)
								{
									// ǰ��2��λ����ʾ������з���X
									for (j = 0; j < 3; j++)
									{
										mjHand.AddTile(TILE_ZHONG + j);
									}

									mjHand.AddTile(tile);
								}
								else
								{
									// ����2��λ����ʾ��ͨ��4����ͬ��
									for (j = 0; j < 4; j++)
									{
										mjHand.AddTile(tile);
									}
								}
							}
						}
						else if (tile >= TILE_ZHONG)
						{
							// ����4����ͬ�Ƶĸ��ж�, ����������ʾ
							BOOL bBuGang = FALSE;
							TILE_SET tmpTileSet;
							for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
							{
								m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

								if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_PATCH)
								{
									if (tmpTileSet.m_ucTile[0] == tile)
									{
										bBuGang = TRUE;

										mjHand.ReleaseAll();
										for (j = 0; j < 4; j++)
										{
											mjHand.AddTile(tmpTileSet.m_ucTile[j]);
										}
										break;
									}
								}
							}

							if (!bBuGang)
							{
								// �з���X�ж�, ����ѡ��ӵ��tile 2���Ƶ����ݸ�
								TILE_SET tmpTileSet;
								for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
								{
									m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

									if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
									{
										// ����ȡ2���Ƶ����ݸ�
										if (tmpTileSet.m_ucTile[3] == tile)
										{
											mjHand.ReleaseAll();
											for (j = 0; j < 4; j++)
											{
												mjHand.AddTile(tmpTileSet.m_ucTile[j]);
											}

											break;
										}
									}
								}
							}
						}

						for (j = 0; j < 4; j++)
						{
							cmd.ucTile[j] = mjHand.GetTile(j);
						}
					}
					////////////////////////////////////////////////////////////////////////

					if (mjHand.IsHave(TILE_EAST) && mjHand.IsHave(TILE_SOUTH) && mjHand.IsHave(TILE_WEST) && mjHand.IsHave(TILE_NORTH))
					{
						cmd.ucFlag = ACTION_QUADRUPLET_DNXB;      // ����������
					}
					else if (mjHand.IsHave(TILE_ZHONG) && mjHand.IsHave(TILE_FA) && mjHand.IsHave(TILE_BAI) 
						 && (mjHand.IsHave(TILE_ZHONG, 2) || mjHand.IsHave(TILE_FA, 2) || mjHand.IsHave(TILE_BAI, 2)))
					{
						cmd.ucFlag = ACTION_QUADRUPLET_ZFB;       // �з���X��
					}
					else if (m_TilesHand[m_byMyStation].IsHave(cmd.ucTile[0], 4))
					{
						cmd.ucFlag = ACTION_QUADRUPLET_CONCEALED; // ����
					}
					else if (m_TilesHand[m_byMyStation].IsHave(cmd.ucTile[0], 3))
					{
						cmd.ucFlag = ACTION_QUADRUPLET_REVEALED;  // ����
					}
					else
					{
						cmd.ucFlag = ACTION_QUADRUPLET_PATCH;     // ����
					}

					break;
				}
			}
		}
	}

	m_pGameFrame->SendGameData(0, (void*)&cmd, sizeof(cmd));

	// ȡ�����õ��ƿؼ�
	SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_MJCtrl_HandCtrl_2, SetInvalidTile(NULL, 0));

	// ����������ʾ��Ϣ
	SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(false));

	// �������״̬
	m_byGameState = MJ_STATE_GIVE;

	// �������Ʋ���״̬
	m_byStateSelect = MJ_STATE_SELECT_NONE;

	return ;
}

/** 
 * @brief ������Ϣ�ƶ���ʾѡ��             
 * @param [in] uControlID �ؼ�ID
 * @param [in] nIndex     ������λ��
 */
void CMJFrameLogic::OnBlockMoveSelect(UINT32 uControlID, int nIndex)
{
	TILE tile = TILE_BEGIN;
	IMjHandCtrl * pMJHandCtrl = NULL;
	GETCTRL(IMjHandCtrl, pMJHandCtrl, m_pUI, CTN_MJCtrl_HandCtrl_2);
	if (pMJHandCtrl != NULL)
	{
		tile = pMJHandCtrl->GetUpTile();
	}

	if ((nIndex < 0 || nIndex > 14) || tile == TILE_BEGIN || m_byGameState == MJ_STATE_GIVE || (m_byStateSelect != MJ_STATE_SELECT_COLLECT && m_byStateSelect != MJ_STATE_SELECT_QUADRUPLET && m_byStateSelect != MJ_STATE_SELECT_TING))
	{
		// ��ǰ״̬�����ڳԡ��ܡ�����ѡ��״̬
		SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(false));
		return ;
	}

	int i, j, k;
	TILE_SET tileSet;
	CMJHand mjHand;
	MJ_TILE_UIINFO tileUserInfo[20];
	IMjCtrl * pMJCtrl = NULL;
	SAFE_CTRL_OPERATE(IMjCtrl, CTN_MJCtrl_Block_3, SetControlVisible(false));
	SAFE_CTRL_OPERATE(IMjCtrl, CTN_MJCtrl_Block_4, SetControlVisible(false));

	IContain * pContain = NULL;
	GETCTRL(IContain, pContain, m_pUI, CTN_Block_Panel);
	if (pContain != NULL)
	{
		// �Ը����ƿ���ŵ�ǰѡ����齫���ƶ�
		RECT rtTile = pMJHandCtrl->GetTileRectByIndex(m_TilesHand[m_byMyStation].CurrentLength() % 3 == 2 ? nIndex - 1: nIndex);
		pContain->SetGameXY(rtTile.left, pContain->GetGameY());
	}

	if (m_byStateSelect == MJ_STATE_SELECT_COLLECT)
	{
		GETCTRL(IMjCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_Block_3);
		if (pMJCtrl == NULL)
		{
			return ;
		}

		pMJCtrl->SetIsUseMouse(false);
		pMJCtrl->GetTilesInfo(tileUserInfo, 20);

		// ����״̬ѡ��
		for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
		{
			m_stBlockInfo.tilesSet.GetSet(i, tileSet);
			if (tileSet.m_ucFlag == ACTION_COLLECT)
			{
				mjHand.ReleaseAll();
				for (j = 0; j < 3; j++)
				{
					mjHand.AddTile(tileSet.m_ucTile[j]);
				}

				if (mjHand.IsHave(tile))
				{
					tileUserInfo[0].ucTiles[0] = mjHand.GetTile(0);
					tileUserInfo[1].ucTiles[0] = mjHand.GetTile(1);
					tileUserInfo[2].ucTiles[0] = mjHand.GetTile(2);
					pMJCtrl->SetTilesInfo(tileUserInfo, 3);
					pMJCtrl->SetControlVisible(true);
					SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(true));
					return ;
				}
			}
		}
	}
	else if (m_byStateSelect == MJ_STATE_SELECT_QUADRUPLET)
	{
		GETCTRL(IMjCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_Block_4);
		if (pMJCtrl == NULL)
		{
			return ;
		}

		pMJCtrl->SetIsUseMouse(false);
		pMJCtrl->GetTilesInfo(tileUserInfo, 20);

		// ����״̬ѡ��
		for (i = 0; i < m_stBlockInfo.tilesSet.CurrentLength(); i++)
		{
			m_stBlockInfo.tilesSet.GetSet(i, tileSet);
			if (tileSet.m_ucFlag == ACTION_QUADRUPLET_CONCEALED
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_REVEALED
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_PATCH
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_DNXB
			 || tileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
			{
				mjHand.ReleaseAll();
				for (j = 0; j < 4; j++)
				{
					mjHand.AddTile(tileSet.m_ucTile[j]);
				}

				if (mjHand.IsHave(tile))
				{
					if (tile >= TILE_EAST && tile <= TILE_BAI)
					{
						// �������ʾѡ��
						if (m_TilesHand[m_byMyStation].IsHave(tile, 4))
						{
							// ӵ��4����ͬ�Ƶĸ��ж�
							// ����: ���з����װװװ�, �ᵼ���з��װ����ָ���ϲ���ʾ, �ᱻ�װװװ׸�������ʾ, ���԰�4���Ʒֿ���ʾ, ǰ�������ʾ�з��װ�, ���������ʾ�װװװ�
							int nFirstPos = 0;
							for (j = 0; j < m_TilesHand[m_byMyStation].CurrentLength(); j++)
							{
								if (m_TilesHand[m_byMyStation].GetTile(j) == tile)
								{
									nFirstPos = j;   // ��¼��ʼλ��, ������ͬ�Ƶ�λ��ѡ����ʾ��ͬ��ʾ
									break;
								}
							}

							if (tile >= TILE_EAST && tile <= TILE_NORTH)
							{
								// ���ѡ�е��Ƕ�������, ��Ҫ�ж��Ƿ���4��һ���ĸܣ����Ƕ��������ָܷ�
								// �ж϶����������Ƿ����
								BOOL bGang = FALSE;
								TILE_SET tmpTileSet;
								for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
								{
									m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

									if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_DNXB)
									{
										bGang = TRUE;
										break;
									}
								}

								mjHand.ReleaseAll();

								if (bGang && nFirstPos + 1 < nIndex)
								{
									// ǰ��2��λ����ʾ����ܶ�������
									for (j = 0; j < 4; j++)
									{
										mjHand.AddTile(TILE_EAST + j);
									}
								}
								else
								{
									// ����2��λ����ʾ��ͨ��4����ͬ��
									for (j = 0; j < 4; j++)
									{
										mjHand.AddTile(tile);
									}
								}
							}
							else if (tile >= TILE_ZHONG && tile <= TILE_BAI)
							{
								// ���ѡ�е����з���X, ��Ҫ�ж��Ƿ���4��һ���ĸܣ������з���X��
								// �ж��з���X���Ƿ����
								BOOL bGang = FALSE;
								TILE_SET tmpTileSet;
								for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
								{
									m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

									if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
									{
										bGang = TRUE;
										break;
									}
								}

								mjHand.ReleaseAll();

								if (bGang && nFirstPos + 1 < nIndex)
								{
									// ǰ��2��λ����ʾ������з���X
									for (j = 0; j < 3; j++)
									{
										mjHand.AddTile(TILE_ZHONG + j);
									}

									mjHand.AddTile(tile);
								}
								else
								{
									// ����2��λ����ʾ��ͨ��4����ͬ��
									for (j = 0; j < 4; j++)
									{
										mjHand.AddTile(tile);
									}
								}
							}
						}
						else if (tile >= TILE_ZHONG)
						{
							// ����4����ͬ�Ƶĸ��ж�, ����������ʾ
							BOOL bBuGang = FALSE;
							TILE_SET tmpTileSet;
							for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
							{
								m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

								if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_PATCH)
								{
									if (tmpTileSet.m_ucTile[0] == tile)
									{
										bBuGang = TRUE;

										mjHand.ReleaseAll();
										for (j = 0; j < 4; j++)
										{
											mjHand.AddTile(tmpTileSet.m_ucTile[j]);
										}
										break;
									}
								}
							}

							if (!bBuGang)
							{
								// �з���X�ж�, ����ѡ��ӵ��tile 2���Ƶ����ݸ�
								for (j = 0; j < m_stBlockInfo.tilesSet.CurrentLength(); j++)
								{
									m_stBlockInfo.tilesSet.GetSet(j, tmpTileSet);

									if (tmpTileSet.m_ucFlag == ACTION_QUADRUPLET_ZFB)
									{
										// ����ȡ2���Ƶ����ݸ�
										if (tmpTileSet.m_ucTile[3] == tile)
										{
											mjHand.ReleaseAll();
											for (j = 0; j < 4; j++)
											{
												mjHand.AddTile(tmpTileSet.m_ucTile[j]);
											}

											break;
										}
									}
								}
							}
						}
					}

					tileUserInfo[0].ucTiles[0] = mjHand.GetTile(0);
					tileUserInfo[1].ucTiles[0] = mjHand.GetTile(1);
					tileUserInfo[2].ucTiles[0] = mjHand.GetTile(2);
					tileUserInfo[3].ucTiles[0] = mjHand.GetTile(3);

					pMJCtrl->SetTilesInfo(tileUserInfo, 4);
					pMJCtrl->SetControlVisible(true);
					SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(true));
					return ;
				}
			}
		}
	}
	else if (m_byStateSelect == MJ_STATE_SELECT_TING)
	{
		GETCTRL(IMjCtrl, pMJCtrl, m_pUI, CTN_MJCtrl_Block_4);
		if (pMJCtrl == NULL)
		{
			return ;
		}

		pMJCtrl->SetIsUseMouse(false);
		pMJCtrl->GetTilesInfo(tileUserInfo, 20);

		// ����״̬ѡ��
		for (i = 0; i < m_stBlockInfo.tingGive.CurrentLength(); i++)
		{
			tile = m_stBlockInfo.tingGive.GetTile(i);

			for (j = 0; j < m_TilesHand[m_byMyStation].CurrentLength(); j++)
			{
				if (tile == m_TilesHand[m_byMyStation].GetTile(j))
				{
					mjHand.ReleaseAll();
					for (k = 0; k < m_stBlockInfo.tingWin[j].CurrentLength(); k++)
					{
						tileUserInfo[k].ucTiles[0] = m_stBlockInfo.tingWin[j].GetTile(k);
					}

					pMJCtrl->SetTilesInfo(tileUserInfo, m_stBlockInfo.tingWin[j].CurrentLength());
					pMJCtrl->SetControlVisible(true);
					SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(true));
					return ;
				}
			}
		}
	}

	SAFE_CTRL_OPERATE(IContain, CTN_Block_Panel, SetControlVisible(false));
}