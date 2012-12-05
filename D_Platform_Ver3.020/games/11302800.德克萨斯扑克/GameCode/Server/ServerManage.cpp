#include "StdAfx.h"
#include "ServerManage.h"
#include "../GameMessage/GameAlgorithm.h"
#include "../../../../sdk/include/Common/writelog.h"
#include <math.h>

TCHAR szNumber[][10] = {"","2","3","4","5","6","7","8","9","10","J","Q","K","A"};
TCHAR szShape[][10] = {"����", "����", "÷��" ,"����"};
TCHAR szName[][20] = {"", "ɢ��", "����", "����", "����", "��С˳��", "˳��", "ͬ��", "��«", "����", "��Сͬ��˳", "ͬ��˳", "�ʼ�ͬ��˳"};


/*
* ��ӡ��־�ļ�
*/
void DebugPrintf(int uRoomID,const char *p, ...)
{
	//return;	// ����ʱ�����κ���	By Zxd 20090727
	char szFilename[1024];
	CString strPath = CINIFile::GetAppPath() +"\\GameLog\\";

	SHCreateDirectoryEx(NULL, strPath, NULL);

	CTime time = CTime::GetCurrentTime();

	wsprintf( szFilename, "%s%d_%d_%d%d%d_Server.txt",strPath ,  NAME_ID,uRoomID ,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}

//��Ϸ����

//��Ϸ��չ����

//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	//for (int i = 0; i < PLAY_COUNT; i++)
	//{
	//	m_emUserAgree[i] = US_UNKNOWN;
	//}
	m_bGameFinished = true;

    m_bGameStation = GS_WAIT_SETGAME;
	m_bFaiPaiFinish = false;
}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}


/*-----------------------------------------------------------------------------------------------------------
һϵ��ƽ̨�ṩ����Ҫ������������Ϸ����ĺ���ʵ�֣������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
----------------------------------------------------------------------------------------------------------*/

//��ȡ�����ļ�
bool CServerGameDesk::InitDeskGameStation()
{
	//LoadDeskCfg();
	LoadDeskCfg(m_pDataManage->m_InitData.uRoomID);
	//LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (GetPlayerCount() < 2)
	{
		OutputDebugString("dxh: GameBegin, ��������");
		return false;
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			if (m_GameData.m_UserData[i].m_bIn == false)
			{
				OutputDebugString("dxh: GameBegin, �����δ������");
				return false;
			}
		}	
	}

	OutputDebugString("dxh: GameBegin");
	if (__super::GameBegin(bBeginFlag) == false) 
	{
		GameFinish(0, GF_SALE);
		return false;
	}

	m_bGameStation = GS_PLAY_GAME;

	m_bGameFinished = false;


	//����MyTimer����ζ��Ϸ��״̬������֮����
	SetTimer(IDT_MYTIMER,MY_ELAPSE);

	//�ر��¼���Ϸ����
	CloseEventProtection();

	//��ʼ�����
	InitGroupTable();

	//��ʼ�������Ϸ����
	m_GameData.Init();

	// ��Ϸ���ݶ���İ��ʼ��
	m_GameData.HalfInit();

	for (int i = 0; i < PLAY_COUNT; i++) 
	{
		m_iUserStation[i] = 255;
		if (m_pUserInfo[i] != NULL)
		{
			m_iUserStation[i] = TYPE_PLAY_GAME;
			m_GameData.m_bGiveUp[i] = false;
		}
	}

	//�·���Ϸ��ʼ�źţ�֪ͨ�ͻ��˿�����Ϸ״̬��
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);
		}
	}
	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);

	//����ϵͳ���������������̣���Ҫ��ɣ�ʱ�����ã��������õ�����
	ActionRedirect(IDEV_SYS_CFG_DESK, 1);

	OutputDebugString("DZPKServer:OnUserSubMoney2");
	return true;
}

// ��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	CString str;
	str.Format("Server: GameFinish() ��Ϸ������־: %d",bCloseFlag);
	OutputDebugString(str);

	KillMyTimer(IDT_MYTIMER);

	switch (bCloseFlag)
	{
	case GF_NORMAL:
		{
			TResult tagResult;

			memset(&tagResult,0,sizeof(tagResult));

			int i, j, k;

			CString str;

			CByteArray arrayResult;

			for (i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] != NULL)
				{
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
					tagResult.nbyUserID[i] = m_pUserInfo[i]->m_UserData.dwUserID;
					memcpy(tagResult.szName[i],m_pUserInfo[i]->m_UserData.nickName,sizeof(m_pUserInfo[i]->m_UserData.nickName));
					tagResult.bOrder[i] = i;
				}
			}

			int nWinCount = 0;
			__int64 nScore[PLAY_COUNT] = {0};
			__int64 nRecoverMoney[PLAY_COUNT][8] = {0}; // ������ע�Ľ��

			if (!m_GameData.m_bGiveUpOver)
			{
				OutputDebugString("Server: ���ƽ���");
				
				// ������Ϸ8����ע��, ��Ҫ���ÿ����ע��ʤ�����
				for (i = 0; i <= m_GameData.m_nBetPool && i < 8; i++)
				{
					// ����ÿһ����ע��Ӯ�� // Ӯ�������������
					CalculateWin(i, arrayResult);
					
					// ��ȡ�����С
					nWinCount = arrayResult.GetSize();

					str.Format("Server: ��ǰӮ������: %d", nWinCount);
					OutputDebugString(str);

					if (nWinCount > 0)
					{
						bool bIsEqual = true;
						for (j = 0; j < PLAY_COUNT; j++)
						{
							if (m_pUserInfo[j] == NULL || m_GameData.m_nBetMoney[j][i] <= 0)
							{
								continue;
							}

							bool bIsWin = false;

							for (k = 0; k < nWinCount; k++)
							{
								if (j == arrayResult.GetAt(k))
								{
									bIsWin = true;
									break;
								}
							}

							if (bIsWin)
							{
								// ʤ���������
								continue;
							}

							str.Format("Server: ��%d����ע��, ���ID: %d, λ��:%d, ��: %I64d", i, m_pUserInfo[j]->m_UserData.dwUserID, j, m_GameData.m_nBetMoney[j][i]);
							OutputDebugString(str);

							// ��ҿ۽��
							nScore[j] -= m_GameData.m_nBetMoney[j][i];

							bIsEqual = false;

							for (k = 0; k < nWinCount; k++)
							{
								int nStation = arrayResult.GetAt(k);  // Ӯ�����

								str.Format("Server: Ӯ������:%d, Ӯ��λ��: %d", nWinCount, nStation);
								OutputDebugString(str);

								// ʤ����Ҽӽ��
								nScore[nStation] += m_GameData.m_nBetMoney[j][i] / nWinCount;

								// ��¼���Ӯ����ע�ؽ��
								tagResult.nWinPoolsMoney[nStation][i] += m_GameData.m_nBetMoney[j][i] / nWinCount;

								// ��¼���Ӯ����ע��
								tagResult.bWinBetPools[nStation][i] = true;

								// �����Լ���ע�Ľ��
								if (nRecoverMoney[nStation][i] <= 0)
								{
									nRecoverMoney[nStation][i] = m_GameData.m_nBetMoney[nStation][i];
									CString str;
									str.Format("Server: ���ƽ���1---���:%d �����Լ���ע���:%I64d, ��%d����ע��", m_pUserInfo[nStation]->m_UserData.dwUserID,nRecoverMoney[nStation][i], i);
									OutputDebugString(str);
								}

								str.Format("Server: ��%d����ע��, ���ID: %d, λ��:%d, Ӯ: %I64d", i, m_pUserInfo[nStation]->m_UserData.dwUserID, nStation, m_GameData.m_nBetMoney[j][i]);
								OutputDebugString(str);
							}
						}

						if (bIsEqual)
						{
							// ���������ȣ�û����ң���Ҫ���Լ���ע�Ľ�һ���
							for (k = 0; k < nWinCount; k++)
							{
								int nStation = arrayResult.GetAt(k);  // Ӯ�����

								// �����Լ���ע�Ľ��
								if (nRecoverMoney[nStation][i] <= 0)
								{
									tagResult.bWinBetPools[nStation][i] = true;
									nRecoverMoney[nStation][i] = m_GameData.m_nBetMoney[nStation][i];
									CString str;
									str.Format("Server: ���ƽ���2---���:%d �����Լ���ע���:%I64d, ��%d����ע��", m_pUserInfo[nStation]->m_UserData.dwUserID,nRecoverMoney[nStation][i], i);
									OutputDebugString(str);
								}
							}
						}
					}
					else
					{
						str.Format("Server: ��%d����ע��û���ҵ�ʤ�����", i);
						OutputDebugString(str);
					}
				}
			}
			else
			{
				// ���ƽ���, ֻ��һ�����Ӯ
				OutputDebugString("Server: ���ƽ���");
				BYTE bWinStation = -1;
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					if (m_pUserInfo[i] == NULL)
					{
						continue;
					}

					// �Ȳ��ҵ����һ��δ���Ƶ����
					if (!m_GameData.m_bGiveUp[i])
					{
						bWinStation = i;

						str.Format("Server: bWinStation=%dδ����", bWinStation);
						OutputDebugString(str);

						for (int j = 0; j <= m_GameData.m_nBetPool && j < 8; j++)
						{
							// ��¼���Ӯ����ע��
							tagResult.bWinBetPools[bWinStation][j] = true;

							// �����Լ���ע�Ľ��
							nRecoverMoney[bWinStation][j] = m_GameData.m_nBetMoney[bWinStation][j];

							CString str;
							str.Format("Server: ���ƽ���---���:%d �����Լ���ע���:%I64d, ��%d����ע��", m_pUserInfo[bWinStation]->m_UserData.dwUserID,nRecoverMoney[bWinStation][j], j);
							OutputDebugString(str);
						}

						break;
					}
				}

				for (int i = 0; i < PLAY_COUNT; i++)
				{
					if (m_pUserInfo[i] == NULL || bWinStation == i)
					{
						continue;
					}

					// ��ҿ۽��
					nScore[i] -= m_GameData.GetBetMoney(i);

					str.Format("Server: ���: %d ����: %I64d", i, nScore[i]);
					OutputDebugString(str);

					// Ӯ�ҽ��
					nScore[bWinStation] += _abs64(nScore[i]);

					for (int j = 0; j <= m_GameData.m_nBetPool && j < 8; j++)
					{
						// ��¼���Ӯ����ע�ؽ��
						tagResult.nWinPoolsMoney[bWinStation][j] = _abs64(nScore[i]);
					}


					str.Format("Server: ���: %d Ӯ���: %I64d", bWinStation, nScore[bWinStation]);
					OutputDebugString(str);
				}
			}

			OutputDebugString("Server: ������һ���");

			for (i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					continue;
				}

				// �����Ӯ����
				tagResult.nScore[i] = nScore[i];

				if (nScore[i] > 0)
				{
					tagResult.bWin[i] = true;
				}

				for (j = 0; j <= m_GameData.m_nBetPool && j < 8; j++)
				{
					// ���ÿ����ע������
					tagResult.nUserBet[i][j] = m_GameData.m_nBetMoney[i][j];
				}

				str.Format("Server: λ��: %d����Ӯ����: %I64d", i, tagResult.nScore[i]);
				OutputDebugString(str);
			}

			for (i = 0; i <= m_GameData.m_nBetPool && j < 8; i++)
			{
				// ��ÿ����ע�صĽ��
				tagResult.nBetPools[i] = m_GameData.GetBetMoney(i, false);
			}

			// д�����ݿ�				
			bool bNetCutUser[PLAY_COUNT];
			memset(bNetCutUser, 0, sizeof(bNetCutUser));

			// ���н�ҿ۳�
			__super::ChangeUserPointint64(tagResult.nScore,bNetCutUser);

			memset(tagResult.nMoney,0,sizeof(tagResult.nMoney));

			// ��ȡ��˰����
			__super::RecoderGameInfo(tagResult.nMoney);

			// �Լ���ע��һ��մ���
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					continue;
				}

				// �Ӽ���Ӧ�Ĵ�����, ��Ĳ���Ҫ�ٴ���Ӯ��ʱ����Ҫ��Ӯ�Ľ�Ҽ��뵽������������
				if (tagResult.nMoney[i] > 0)
				{
					m_GameData.m_UserMoney[i] += tagResult.nMoney[i];
				}
				else
				{
					// ��ע��Ǯ + ���˵�Ǯ = ������Ϸ������Ǯ 2012-10-11 duanxiaohui alter 
					m_GameData.m_UserMoney[i] += (m_GameData.GetBetMoney(i) + tagResult.nMoney[i]);

					if (m_GameData.m_UserMoney[i] < 0)
					{
						m_GameData.m_UserMoney[i] = 0;
					}
				}

                // �ͻ��������Ҹ���
                tagResult.nSelfMoney[i] = m_pUserInfo[i]->m_UserData.i64Money;

				tagResult.nMoneyEx[i] = tagResult.nMoney[i];

				str.Format("Server: nScore[%d]=%I64d,  nMoneyEx[%d]=%I64d,  nMoney[%d]=%I64d ", i, tagResult.nScore[i], i, tagResult.nMoneyEx[i], i, tagResult.nMoney[i]);
				OutputDebugString(str);

				for (int j = 0; j < 8; j++)
				{
					if (tagResult.bWinBetPools[i][j])
					{
						CString str;
						str.Format("Server: ���:%d Ӯ���:%I64d, �����Լ���ע���:%I64d, ��%d����ע��", m_pUserInfo[i]->m_UserData.dwUserID,tagResult.nMoney[i], nRecoverMoney[i][j], j);
						OutputDebugString(str);

						// ���Լ���ע�Ľ�һ���
						//m_GameData.m_UserMoney[i] += nRecoverMoney[i][j];

						// Ӯ�Ľ������Ҳ�����Լ���ע�Ľ��
						tagResult.nWinPoolsMoney[i][j] += nRecoverMoney[i][j];

						if (tagResult.nMoneyEx[i] < 0)
						{
							// Ӯ�Ľ������Ҳ�����Լ���ע�Ľ��
							//tagResult.nMoneyEx[i] = nRecoverMoney[i][j];
							// 2012-10-11 duanxiaohui alter
							tagResult.nMoneyEx[i] = (m_GameData.GetBetMoney(i) + tagResult.nMoney[i]);
						}
						else
						{
							// ���Լ���ע�Ľ�һ��� 2012-10-11 duanxiaohui alter
							m_GameData.m_UserMoney[i] += nRecoverMoney[i][j];

							// Ӯ�Ľ������Ҳ�����Լ���ע�Ľ��
							tagResult.nMoneyEx[i] += nRecoverMoney[i][j];
						}
					}
				}
			}

            // ���¿ͻ��˴�����
            memcpy(tagResult.nSubMoney, m_GameData.m_UserMoney, sizeof(tagResult.nSubMoney));

			BYTE temp = 0;
			// �������(Ӯ���������ǰ��, ��������)
			for (i = 0; i < PLAY_COUNT; i++)
			{
				for (j = 0  ; j < PLAY_COUNT - i - 1; j++)
				{
					if (m_pUserInfo[i] == NULL)
					{
						continue;
					}

					if (tagResult.nMoney[j] > tagResult.nMoney[j + 1])
					{
						temp = tagResult.bOrder[j];
						tagResult.bOrder[j] = tagResult.bOrder[j+1];
						tagResult.bOrder[j + 1] = temp;
					}
				}
			}

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					tagResult.bOrder[i] = 255;
					tagResult.nMoney[i] = 0;
				}
			}

			CString strbOrder;

			// ֪ͨ�ͻ���
			for (i = 0; i < PLAY_COUNT; i++)
			{
 				m_GameData.m_UserData[i].m_bIn = false;

				if (m_pUserInfo[i] != NULL)
 				{
					strbOrder.Format("Server:bOrder[%d] == %d",i,tagResult.bOrder[i]);
					OutputDebugString(strbOrder);

 					SendGameData(i, &tagResult, sizeof(TResult), MDM_GM_GAME_NOTIFY, ASS_RESULT, 0);

					str.Format("Server:��Ϸ��������-Server:���ID:%d,����ǳ�:%s,��Ҵ�����:%I64d,�����ʵ���1:%I64d,�����ʵ���2:%I64d,���ʵ����Ӯ���:%I64d,�����Ӯ���:%I64d",
						m_pUserInfo[i]->m_UserData.dwUserID,m_pUserInfo[i]->m_UserData.szName,tagResult.nSubMoney[i],
						m_pUserInfo[i]->m_UserData.i64Money,tagResult.nSelfMoney[i],tagResult.nMoney[i],tagResult.nMoneyEx[i]);

				   	OutputDebugString(str);
 				}
			}

			SendWatchData(m_bMaxPeople, &tagResult, sizeof(TResult), MDM_GM_GAME_NOTIFY, ASS_RESULT, 0);

            m_bGameStation = GS_WAIT_NEXT_ROUND;

			// ������������Ϣ duanxiaohui
			for (i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] != NULL)
				{
					if (m_GameData.m_UserData[i].m_bNetCut)
					{
						CString str;
						str.Format("dxh: ��ն������:%d", i);
						OutputDebugString(str);

						//MakeUserOffLine(i);   // ǿ����ո������Ϣ
						OnUserMoneyLack(i);   // ֪ͨ�ͻ���ɾ���������Ϣ
					}
				}
			}
			// �������ȴ�ÿ�����ȷ�ϼ�����Ϸ��Ϣ(ASS_NEXT_ROUND_REQ)���ڴ���Ҫ���ƶ������չ������			
			//OpenEventProtection(IDEV_NEXT_ROUND_BEGIN, /*m_GameData.m_tagDeskCfg.Time.byNextRound +*/ 30 * 1000);)

			// ��Ϸ����
			//GameFinish(bDeskStation,bCloseFlag);
			//return true;
			
		}
		break;
	case GFF_SAFE_FINISH:
	case GF_SALE:			//��Ϸ��ȫ����
		{
			//��������
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			/*GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));

			for (int i = 0; i < PLAY_COUNT; i ++) 
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);*/

			bCloseFlag = GFF_SAFE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
		break;
	default:
		break;
	}

	// ��������
	ReSetGameState(bCloseFlag);

	return __super::GameFinish(bDeskStation,bCloseFlag);
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if (m_iUserStation[bDeskStation] != TYPE_PLAY_GAME ||m_GameData.m_bGiveUp[bDeskStation] == true)
	{
		return false;
	}
	else
	{
		return m_bGameStation >= GS_PLAY_GAME && m_bGameStation < GS_WAIT_NEXT_ROUND;
	}
	
	//return __super::IsPlayGame(bDeskStation);//m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT;
}

//������Ϸ�ж��Ƿ�����Ϸ��
BOOL CServerGameDesk::IsPlayingByGameStation() 
{
	return m_bGameStation >= GS_PLAY_GAME && m_bGameStation < GS_WAIT_NEXT_ROUND;
	//return 	__super::IsPlayingByGameStation();//m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT;
}

bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			bool bForceQuitAsAuto = true; 
			///�����ϲ�·��
			CString szAppPath = CINIFile::GetAppPath();

			CString strBCFFile; 
			CString strKeyName; 
			strKeyName.Format("%d", NAME_ID); 
			strBCFFile.Format("%s\\SpecialRule.bcf", szAppPath); 

			CBcfFile File(strBCFFile);

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto", strKeyName, 0); 
			}

			if (bForceQuitAsAuto == true)
			{
				return true; 
			}
			else
			{
				if (m_GameData.m_byTokenUser != bDeskStation 
					&& m_pUserInfo[bDeskStation] != NULL
					&&!m_GameData.m_bGiveUp[bDeskStation] 
				&&m_iUserStation[bDeskStation] == TYPE_PLAY_GAME)
				{
					m_GameData.m_bGiveUp[bDeskStation] = true;

					if (m_GameData.m_byNTUser == bDeskStation)
					{
						m_GameData.m_byNTUser = 255;
					}
					else if (m_GameData.m_bSmallBlind == bDeskStation)
					{
						// Сäע�������, ��Сäע��Ϊ255
						m_GameData.m_bSmallBlind = 255;
					}
					else if (m_GameData.m_bBigBlind == bDeskStation)
					{
						// ��äע�������, �Ѵ�äע��Ϊ255
						m_GameData.m_bBigBlind = 255;
					}
				}
				else
				{
					TBet tagVerb;
					tagVerb.nType = ET_FOLD;
					OnUserBet(bDeskStation, &tagVerb, true);
				}
			}

			int iCount = 0; 
			for(int  i = 0; i < PLAY_COUNT; i++)
			{
				if(NULL != m_pUserInfo[i] && m_iUserStation[i] == TYPE_PLAY_GAME && m_GameData.m_bGiveUp[i] == false)
				{
					iCount ++ ;
				}
			}

			if(iCount < 2)
			{
				m_GameData.m_bGiveUpOver = true;
				GameFinish(0, GF_NORMAL);
			}

			return true; 
		}
	default:
		{
			break; 
		}
	}

	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{

		// 	case ASS_GM_AGREE_GAME:		
		// 		{
		// 			//�û�ͬ����Ϸ
		// 			return OnUserAgreeGame(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		// 		}

	case ASS_CFG_DESK:
		{
			//����������
			if (bWatchUser)
			{
				return false;
			}
			//������������õ�ע
			return OnUserCfgDesk(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
		break;

	case ASS_CFG_DESK_TIMEOUT:
		{
			//����������
			if (bWatchUser)
			{
				return true;
			}
			//���õ�ע��ʱ
			return OnUserCfgDeskTimeout(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
		break;

	case ASS_AGREE_DESK_CFG:
		{
			//����������
			if (bWatchUser)
			{
				return false;
			}
			//���������ͬ���ע
			return OnUserAgreeDeskCfg(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
		break;

	case ASS_SUB_MENOY:
		{
			//����������
			if (bWatchUser)
			{
				return false;
			}

			if (uSize != sizeof(TSubMoney))
			{
				return false;
			}

			if (m_bGameStation == GS_PLAY_GAME)
			{
				return true;
			}

			// ��Ҵ���������
			return OnUserSubMoney(bDeskStation, pData);
		}
		break;
		
	case ASS_BET:
		{
			if (bWatchUser)
				return false;
			if( uSize != sizeof(TBet))
				return false;

			//�����������ͬ���ע
			return OnUserBet(bDeskStation, pData);
		}
		break;

	case ASS_SEND_CARD_FINISH:
		{
			//����������
			if (bWatchUser)
			{
				return true;
			}
			//������ҷ������
			return OnUserSendCardFinish(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
		break;

	case ASS_NEXT_ROUND_REQ:
		{
			//����������
			if (bWatchUser)
			{
				return false;
			}
			//������Ҷ���һ�غ�׼�����ź�
			return OnUserNextRoundReq(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
		break;
	case ASS_MONEY_LACK:
		{
			if (bWatchUser)
			{
				return true;
			}

			// ������ұ����Ҳ��������ٴ�����
			return OnUserMoneyLack(bDeskStation);
		}
		break;

#ifdef CLOSE_CARD
	case ASS_LOOK_CARD:		//��ҿ���
		{
			if(bWatchUser)
			{
				return true;
			}

			UserLookCardStruct s_userlookcard;
			s_userlookcard.bDeskStation = bDeskStation;

			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(m_pUserInfo[i] == NULL || i == bDeskStation)
					continue;

				SendGameData(i,&s_userlookcard,sizeof(UserLookCardStruct),MDM_GM_GAME_NOTIFY,ASS_LOOK_CARD,0);
			}

			return true;
		}
	case ASS_SHOW_CARD:	//չʾ����Ϣ
		{
			if(bWatchUser)
			{
				return true;
			}

			//չʾ������
			ShowCardStruct s_showcard;
			s_showcard.byDeskStation = bDeskStation;
			s_showcard.iCardCount = m_GameData.m_UserData[bDeskStation].m_iCardsNum;
			::CopyMemory(s_showcard.byCardList,m_GameData.m_UserData[bDeskStation].m_byMyCards,sizeof(BYTE)*s_showcard.iCardCount);

			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				if(m_pUserInfo[i] == NULL)
					continue;

				SendGameData(i,&s_showcard,sizeof(ShowCardStruct),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD,0);
			}
			return true;
		}
#endif
		//��Ϸ���Ե���Ϣ����
#ifdef CD_CAN_SETPAI

	case ASS_FINISH_ROUND_REQ:
		{
			//����������Ϸ
			return OnTestFinishRoundRequest(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
		break;
#endif

	}

	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);

}

//�������
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	//����������������û�������ˣ����ȡ���������
	int iSeatCount = 0;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pUserInfo[i])
		{
			iSeatCount++;
		}
	}

	if (iSeatCount == 0)
	{
		//���ԭ����ע����Ϣ
		m_GameData.m_tagDeskCfg.Clear();

		LoadDeskCfg(m_pDataManage->m_InitData.uRoomID);
	}

	return __super::UserSitDesk(pUserSit,pUserInfo);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	/*CString str;
	str.Format("Server:-Server: ���%d, �뿪��Ϸ", bDeskStation);
	OutputDebugString(str);*/

	if (!IsPlayingByGameStation())
	{
		// ������
		m_GameData.m_UserMoney[bDeskStation] = 0;
		m_GameData.m_UserData[bDeskStation].m_bIn = false;
	}
	
	bool bShow = false;
	int nCount = 0;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		// �жϴ��ڵ���Ҳ��Ҳ��ǵ�ǰ�˳������
		if (m_pUserInfo[i] != NULL && i != bDeskStation)
		{
			nCount++;
		}
		else
		{
			bShow = true;
		}
	}

	if (nCount == 0 && IsPlayingByGameStation())
	{
		m_GameData.m_iDealPeople = 255;
		// ��ǰ����˳���, û�������, ��ɢ����
		OutputDebugString("Server: ��ǰ����˳���, û�������, ��ɢ����");
		GameFinish(bDeskStation,GF_SALE);
	}


	UserLeftDeskStruct  UserLeft;
	UserLeft.bDeskStation = bDeskStation ; 

	UserLeft.bClearLogo = (m_iUserStation[bDeskStation] == TYPE_PLAY_GAME /*&& m_bGameStation == GS_PLAY_GAME || m_bGameStation == GS_WAIT_ARGEE*/ && bShow) ; 

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL && i != bDeskStation)
		{
			SendGameData(i,&UserLeft,sizeof(UserLeft),MDM_GM_GAME_NOTIFY ,ASS_USER_LEFT_DESK,0);
		}
	}

	 return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

///���ƣ�UserReCome
///������������ȡ��Ҷ��߻�����Ϣ
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	CString str;
	str.Format("Server: ���%d, ��Ҷ����ػ�", bDeskStation);
	OutputDebugString(str);

	m_GameData.m_UserData[bDeskStation].m_bNetCut = false;

	return __super::UserReCome(bDeskStation, pNewUserInfo);
}

//���ƣ�UserNetCut
///������������ȡ��Ҷ�����Ϣ
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	m_GameData.m_UserData[bDeskStation].m_bNetCut = true;

	bool bRet = __super::UserNetCut(bDeskStation, pLostUserInfo);


	CString str;
	str.Format("Server: ���%d, ��Ҷ���", bDeskStation);
	OutputDebugString(str);

	int iPlayerCount = 0;
	int iNetCutCount = 0;
	
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pUserInfo[i])
		{
			iPlayerCount++;
		}
	}

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pUserInfo[i])
		{
			if (m_GameData.m_UserData[i].m_bNetCut)
			{
				iNetCutCount++;
			}
		}
	}

	if (iNetCutCount >= iPlayerCount)
	{
		m_GameData.m_iDealPeople = 255;
		OutputDebugString("Server:��������");
		// ��������
		ReSetGameState(GF_SALE);
		__super::GameFinish(bDeskStation,GF_SALE);
	}
	
	//switch(m_bGameStation)
	//{
	//case GS_PLAY_GAME:
	//	{
	//		
	//		;
	//	}
	//	break;
	//case GS_WAIT_NEXT_ROUND:
	//	{
	//		TNextRoundReq NextRoundReq;
	//		NextRoundReq.nType = TNextRoundReq::EXIT;
	//		OnUserNextRoundReq(bDeskStation,NULL,&NextRoundReq,sizeof(TNextRoundReq),0,false);
	//	}
	//	break;
	//default:
	//	break;
	//}

	return bRet;
}

///���ƣ�CanNetCut
///�����������ı��û����������ĺ���
///@param bDeskStation Ҫ������ҵ�λ��
///@return 
bool CServerGameDesk::CanNetCut(BYTE bDeskStation)
{
	return true;
}


//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	//����״̬
	TGSBase tagGSBase;

	//�����Ϸ�Ļ�������
	GetGSBase(&tagGSBase,bDeskStation);

	CString str;
	str.Format("dxh: ���%d, ��ȡ��Ϸ״̬��Ϣ", m_bGameStation);
	OutputDebugString(str);

	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT_ROUND:
		{
			m_GameData.m_UserMoney[bDeskStation] = 0;
			m_GameData.m_UserData[bDeskStation].m_bIn = false;
			//�ȴ���ҿ�ʼ״̬
			return OnGetGSWaitAgree(bDeskStation,uSocketID,bWatchUser,&tagGSBase);
		}
		break;
	case GS_PLAY_GAME:
		{
			//��Ϸ���е�״̬��
			return OnGetGSPlaying(bDeskStation,uSocketID,bWatchUser,&tagGSBase);
		}
		break;
	//case GS_WAIT_NEXT_ROUND:
	//	{
	//		m_GameData.m_UserMoney[bDeskStation] = 0;
	//		m_GameData.m_UserData[bDeskStation].m_bIn = false;

	//		//�ȴ���һ����Ϸ�Ŀ�ʼ
	//		return OnGetGSWaitNextRound(bDeskStation,uSocketID,bWatchUser,&tagGSBase);
	//	}
		break;
	}

	return false;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	//KillAllTimer();

	//�ǵðѶ�ʱ����MyTimer����ȫ����������������Ӱ��������һ����Ϸ��
	KillTimer(IDT_MYTIMER);
	memset(m_arTimer,0,sizeof(m_arTimer));

	//�¼�������Ҳ��Ҫ�ص�
	CloseEventProtection();

	m_GameData.Init();
	m_bGameStation = GS_WAIT_SETGAME;

	return TRUE;
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	//��MyTimer�й�MFC��Timer

	if (uTimerID == IDT_MYTIMER)
	{
		for (int i = 0; i < TIMER_COUNT; ++i)
		{
			if (m_arTimer[i].uiID == 0)
			{
				continue;
			}

			m_arTimer[i].nTickCount++;

			if (m_arTimer[i].nTickCount >= m_arTimer[i].nTriggerCount)
			{
				m_arTimer[i].nTickCount = 0;
				OnMyTimer(m_arTimer[i].uiID);
			}
		}
	}

	return __super::OnTimer(uTimerID);
}


/*-----------------------------------------------------------------------------------------------------------
һϵ���������л��ƺ���ʵ�֣������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
�������û��ơ���ʱ���ơ����������ơ���Ϸ״̬�Ĺ���
----------------------------------------------------------------------------------------------------------*/

//������ע
BOOL CServerGameDesk::LoadDeskCfg()
{
	TCHAR szIniName[MAX_PATH];
	GET_SERVER_INI_NAME(szIniName);

	CINIFile f(szIniName);

	CString strSeg = "game";
	CString strKey;

	TDeskCfg* pDeskCfg = &m_GameData.m_tagDeskCfg;

	//ʱ�����////////////////////////////////////////////////////////////////////////////

	//���������Ҳ���ʱ��(��)
	strKey = _TEXT("OperateTime");
	pDeskCfg->Time.byOperate = f.GetKeyVal(strSeg,strKey,10);


	//�������////////////////////////////////////////////////////////////////////////////

	//��������////////////////////////////////////////////////////////////////////////////

	strKey = _TEXT("ShowIdOrPlay");
	pDeskCfg->bShowUserInfo = f.GetKeyVal(strSeg, strKey, pDeskCfg->bShowUserInfo); 

	return TRUE;
}

//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadDeskCfg(int iRoomID)
{
	// ����ͨ������
	LoadDeskCfg();

	TCHAR szIniName[MAX_PATH];
	GET_SERVER_INI_NAME(szIniName);

	CINIFile f(szIniName);

	CString strSeg;
	strSeg.Format("%d_%d",NAME_ID,iRoomID);

	CString strKey;

	TDeskCfg* pDeskCfg = &m_GameData.m_tagDeskCfg;

	//��ע���////////////////////////////////////////////////////////////////////////////
	__int64 iLowers[5] = {2, 5, 10, 20, 50};
	::memcpy(pDeskCfg->dz.iLowers,iLowers,sizeof(iLowers));

	pDeskCfg->dz.iRoomMultiple = f.GetKeyVal(strSeg, "RoomMultiple", 1); // ���䱶��
	pDeskCfg->dz.iLower = f.GetKeyVal(strSeg, "MinLimit", 100);			 // ��Ϸ��Ͷ�
	pDeskCfg->dz.iUpper = f.GetKeyVal(strSeg, "MaxLimit", 200000000);	 // ��Ϸ��߶�
	pDeskCfg->dz.iBigBlindNote = pDeskCfg->dz.iLower;		             // ��äע��ע
	pDeskCfg->dz.iSmallBlindNote = pDeskCfg->dz.iLower / 2;		         // Сäע��ע

	pDeskCfg->dz.bIsRoomSubMoney = f.GetKeyVal(strSeg, "RoomSubMoney", 0);  // ���������  0: �˷��䲻��Ҫ������  1: �˷�����Ҫ������
	pDeskCfg->dz.iSubMinMoney = f.GetKeyVal(strSeg, "SubMinMoney",  1000);  // ������ʹ�����
	pDeskCfg->dz.iSubMaxMoney = f.GetKeyVal(strSeg, "SubMaxMoney", 10000);  // ������ߴ�����
	pDeskCfg->dz.iSubPlanMoney = f.GetKeyVal(strSeg, "SubPlanMoney",3000);  // ��ǰ׼��������
	pDeskCfg->dz.iMinusMoney  = f.GetKeyVal(strSeg, "MinusMoney",100);		// �޸�ע��Ӽ����
	pDeskCfg->dz.iMinMoney   = f.GetKeyVal(strSeg, "MinMoney",100);			// ��С��ע���
	pDeskCfg->dz.iUntenMoney   = f.GetKeyVal(strSeg, "UntenMoney ",0);		// ���ڶ��ٽ��ʱ����

	pDeskCfg->dz.iUserClose = f.GetKeyVal(strSeg, "UserClose", true);		// һ�ֺ�10���ڽ����ǿ��
	pDeskCfg->dz.iUserCloseTime = f.GetKeyVal(strSeg, "UserCloseTime", 10); // �رտͻ���ʱ��
	pDeskCfg->dz.iUserInMoney = f.GetKeyVal(strSeg, "iUserInMoney", 10);	// ��Ϸ������ʱ��

	for (int i = 0; i < 5; ++i)
	{
		pDeskCfg->dz.iLowers[i] *= pDeskCfg->dz.iLower * pDeskCfg->dz.iRoomMultiple;
	}

	//ʱ�����////////////////////////////////////////////////////////////////////////////

	//�����������ʱ��(��)
	strKey = _TEXT("OperateTime");
	pDeskCfg->Time.byOperate = f.GetKeyVal(strSeg, strKey, pDeskCfg->Time.byOperate);
	
	//�������////////////////////////////////////////////////////////////////////////////

	//��������////////////////////////////////////////////////////////////////////////////
	strKey = _TEXT("ShowIdOrPlay");
	pDeskCfg->bShowUserInfo = f.GetKeyVal(strSeg,strKey,pDeskCfg->bShowUserInfo); 

	return TRUE;
}

BOOL CServerGameDesk::LoadChangCard(void)
{
	return true;
}

//��ʼ�����
void CServerGameDesk::InitGroupTable(void)
{
	return;
}

//������Ϸ�ڲ���ʱ��
int CServerGameDesk::SetMyTimer(UINT nTimeID, int uElapse)
{
	int nElapse = uElapse / MY_ELAPSE;
	if (nElapse < 1)
	{
		nElapse = 1;
	}
	int nIdx = nTimeID - IDMYT_BASE;
	if (nIdx < 0 || nIdx > TIMER_COUNT - 1)
	{
		CString strCNOutput;
		strCNOutput.Format("SJServer����ʱ��[%d]�������ɹ�.",nTimeID);
		OutputDebugString(strCNOutput);
		return -1;
	}
	m_arTimer[nIdx].uiID = nTimeID;
	m_arTimer[nIdx].nTriggerCount = nElapse;
	return 0;
}


//ɾ����Ϸ�ڲ���ʱ��
void CServerGameDesk::KillMyTimer(UINT nTimeID)
{
	int nIdx = nTimeID - IDMYT_BASE;
	if (nIdx < 0 || nIdx > TIMER_COUNT-1)
	{
		CString strCNOutput;
		strCNOutput.Format("SJServer��ɾ����ʱ��[%d]�������ɹ�.",nTimeID);
		OutputDebugString(strCNOutput);
		return;
	}
	memset(&m_arTimer[nIdx],0,sizeof(TMyTimerNode));
}

//��ʱ����Ϣ
bool CServerGameDesk::OnMyTimer(UINT uTimerID)
{
	switch(uTimerID)
	{

		//�����¼�������/////////////////////////////////////////////
	case IDEV_SYS_CFG_DESK: //ϵͳ�趨��������
		{
			KillMyTimer(IDEV_SYS_CFG_DESK);
			return OnEventSysCfgDesk();
		}
		break;

	case IDEV_USER_CFG_DESK: //����趨��������
		{
			KillMyTimer(IDEV_USER_CFG_DESK);
			return OnEventUserCfgDesk();
		}
		break;

	case IDEV_RAND_CARD:	//ϴ��
		{
			KillMyTimer(IDEV_RAND_CARD);
			return OnEventRandCard();
		}
		break;

	case IDEV_RANDOM:	//���һ��ׯ
		{
			KillMyTimer(IDEV_RANDOM);
			return OnEventRandom();
		}
		break;

	case IDEV_SENDACARD:	 //�·�2����
		{
			KillMyTimer(IDEV_SENDACARD);
			return OnEventSendACard();
		}
		break;

	case IDEV_SEND_3_CARD: //�·�3�Ź�������
		{
			KillMyTimer(IDEV_SEND_3_CARD);
			return OnEventSend3Card();
		}
		break;

	case IDEV_SEND_4_5_CARD: //�·�3�Ź�������
		{
			KillMyTimer(IDEV_SEND_4_5_CARD);
			return OnEventSend1Card();
		}
		break;

	case IDEV_BET_START:	//�״���ע�¼�
		{
			KillMyTimer(IDEV_BET_START);
			return OnEventBetStart();
		}
		break;

	case IDEV_BET_BLINDS_AUTO:	//��Сäעͬʱ��ע�¼�
		{
			KillMyTimer(IDEV_BET_BLINDS_AUTO);
			return OnEventBetBlindsAuto();
		}
		break;

	case IDEV_BET_NEXT:	//��λ�����ע�¼�
		{
			KillMyTimer(IDEV_BET_NEXT);
			return OnEventBetNext();
		}
		break;

	case IDEV_SMALL_BLIND:   //С��ע��ע
		{
			KillMyTimer(IDEV_SMALL_BLIND);
		}
		break;

	case IDEV_BIGBLIND:   //����ע��ע
		{
			KillMyTimer(IDEV_BIGBLIND);

		}
		break;

	case IDEV_ROUND_FINISH: //�غϽ���
		{
			KillMyTimer(IDEV_ROUND_FINISH);
			//return OnEventRoundFinish();
		}
		break;
	case IDEV_COMPARE_CARD: //��Ϸ����
		{
			KillMyTimer(IDEV_COMPARE_CARD);
			return OnCompareCard();
		}
		break;
	case IDEV_RESULT: //��Ϸ����
		{
			KillMyTimer(IDEV_RESULT);
			//return OnEventResult();
			return GameFinish(0, GF_NORMAL);
		}
		break;

	case IDEV_NEXT_ROUND_BEGIN: //��һ�غϿ�ʼ
		{
			KillMyTimer(IDEV_NEXT_ROUND_BEGIN);
			return OnEventNextRoundBegin();
		}
		break;

    case IDEV_AUTO_OPERATE:   // �Զ�����
        {
            KillMyTimer(IDEV_AUTO_OPERATE);
            return OnEventAutoOperate();
        }
        break;

		//�¼����������/////////////////////////////////////////////

	case IDMYT_EVENT_PROTECT:  //�¼���������ʱ��
		{
			return EventGoing();
		}
		break;
	}

	return __super::OnTimer(uTimerID);
}


//����������
bool CServerGameDesk::ActionRedirect(UINT uiActID, int iDelay)
{
	switch(uiActID)
	{
	case IDEV_SYS_CFG_DESK:
		{
			//ϵͳ�趨��������
			OutputDebugString("DZPKServer������ת��[IDEV_SYS_CFG_DESK.ϵͳ�趨��������].");
		}
		break;

	case IDEV_USER_CFG_DESK:
		{
			//����趨��������
			OutputDebugString("DZPKServer������ת��[IDEV_USER_CFG_DESK.����趨��ע��].");
		}
		break;
		
	case IDEV_RANDOM:
		{
			//���һ��ׯ
			OutputDebugString("DZPKServer������ת��[IDEV_RANDOM.���һ��ׯ].");
		}
		break;

	case IDEV_RAND_CARD:
		{
			//ϴ��
			OutputDebugString("DZPKServer������ת��[IDEV_RAND_CARD.ϴ��].");
		}
		break;

	case IDEV_SENDACARD:
		{
			//����
			OutputDebugString("DZPKServer������ת��[IDEV_SEND_CARD.����].");
		}
		break;

	case IDEV_SEND_3_CARD: 
		{
			//��3�Ź�����
			OutputDebugString("DZPKServer������ת��[IDEV_SEND_3_CARD.��3�Ź�����].");
		}
		break;

	case IDEV_BET_START:
		{
			//�״���ע
			OutputDebugString("DZPKServer������ת��[IDEV_BET_START.�µ�һ����ע].");
		}
		break;

	case IDEV_BET_BLINDS_AUTO:
		{
			//��Сäעͬʱ��ע�¼�
			OutputDebugString("DZPKServer������ת��[IDEV_BET_BLINDS_AUTO.��Сäעͬʱ��ע�¼�].");
		}
		break;

	case IDEV_BET_NEXT:
		{
			//��λ�����ע�¼�
			OutputDebugString("DZPKServer������ת��[IDEV_BET_NEXT.��λ�����ע�¼�].");
		}
		break;

	case IDEV_ROUND_FINISH:
		{
			//һ�غϽ���
			OutputDebugString("DZPKServer������ת��[IDEV_ROUND_FINISH.�غϽ���].");
		}
		break;

	case IDEV_RESULT:
		{
			//�غϽ���
			OutputDebugString("DZPKServer������ת��[IDEV_RESULT.�غϽ���].");
		}
		break;

	case IDEV_NEXT_ROUND_BEGIN:
		{
			//��ʼ��һ�غ���Ϸ
			OutputDebugString("DZPKServer������ת��[IDEV_NEXT_ROUND_BEGIN.��ʼ��һ�غ���Ϸ].");
		}
	}

	SetMyTimer(uiActID,iDelay);

	return true;
}

//�򿪶�������������
bool CServerGameDesk::OpenEventProtection(UINT uiEvID,int iLifeTime)
{
	if (m_EventProtecter.uiEvID > 0)
	{
		return false;
	}

	m_EventProtecter.uiEvID = uiEvID;
	m_EventProtecter.iLiftTime = iLifeTime;
	m_EventProtecter.iPassTime = 0;

	SetMyTimer(IDMYT_EVENT_PROTECT,1000);

	return true;
}

//����Ԥ���ѷ��������Ķ�ʱ��
bool CServerGameDesk::EventGoing()
{
	if (m_EventProtecter.uiEvID == 0)
	{
		return true;
	}

	m_EventProtecter.iPassTime++;

	if (m_EventProtecter.iPassTime < m_EventProtecter.iLiftTime) 
	{
		return true;
	}

	//�¼���Ҫ���𣬸���m_epDescr.uiEvID�ṩ��Ҫ�ı���

	switch(m_EventProtecter.uiEvID)
	{
	case IDEV_SENDACARD:
		{
			//������ҷ������
			OnAgentSendCardsFinish();
		}
		break;
	case IDEV_NEXT_ROUND_BEGIN:
		{
			//��һ�ֿ�ʼ�¼�������
			OnAgentNextRoundBegin();
		}
		break;
	case IDEV_BET_NEXT:
		{
			// �Զ�����
			OnEventAutoOperate();
		}
		break;
	}

	//��Ȼ�����¼�
	CloseEventProtection();

	return false;
}

//�����¼�
bool CServerGameDesk::CloseEventProtection()
{
	//uiEvID�¼�����������ϣ������¼�������ʱ��
	KillMyTimer(IDMYT_EVENT_PROTECT);

	//����¼����������ģ��Ա�Ϊһ�¸��¼������ṩ��Դ
	::memset(&m_EventProtecter,0,sizeof(TEvProtectDescr));

	return false;
}

//�����Ϸ�Ļ�����״̬
void CServerGameDesk::GetGSBase(TGSBase* pGSBase,BYTE byDeskstation)
{
	//GSBase��������
	//��ǰ�¼������ĵ�ʱ��
	pGSBase->iEvPassTime = m_EventProtecter.iPassTime + 2; //����������ʱ����2��

	//�����������
	::memcpy(&pGSBase->tagDeskCfg,&m_GameData.m_tagDeskCfg,sizeof(TDeskCfg));

	// ����Ϸ״̬�����ͻ���
	pGSBase->bGameStation = m_bGameStation;

	CString strUserInfo;
	//��ȡ����λ�Ƿ�������
	for (int i = 0; i < PLAY_COUNT; ++i)
	{	
		pGSBase->bHaveUser[i] = (m_pUserInfo[i] != NULL);
	}

	return;
}

//�����Ϸ�Ļ�����״̬
void CServerGameDesk::CopyGSBase(TGSBase* pTarget,const TGSBase* pSource)
{
	if (!pTarget || !pSource)
	{
		return;
	}

	::memcpy(pTarget,pSource,sizeof(TGSBase));

	return;
}

/*-----------------------------------------------------------------------------------------------------------
��Ϸ�¼���Ӧ����ʵ�֣����ܺ���ʵ�֣���ͬ��Ϸ����ϴ�
----------------------------------------------------------------------------------------------------------*/

//ϵͳ�趨������¼�������Ҫ�趨ʱ�䡢��Ϸ����ȣ�
bool CServerGameDesk::OnEventSysCfgDesk()
{
	//���ԭ����ע����Ϣ
	m_GameData.m_tagDeskCfg.Clear();

	LoadDeskCfg(m_pDataManage->m_InitData.uRoomID);

	// �㲥�趨��ע����Ϣ
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SendGameData(i,&m_GameData.m_tagDeskCfg,sizeof(TDeskCfg),MDM_GM_GAME_NOTIFY,ASS_SYS_DESK_CFG,0);
	}

	SendWatchData(m_bMaxPeople,&m_GameData.m_tagDeskCfg,sizeof(TDeskCfg),MDM_GM_GAME_NOTIFY,ASS_SYS_DESK_CFG,0);

	//�����û����������������̣���Ҫ��ɣ�ʱ�����ã��������õ�����

	ActionRedirect(IDEV_RAND_CARD, 1);

	return true;
}

//ע���趨
bool CServerGameDesk::OnEventUserCfgDesk()
{

	//���ѡ��һ����ң���TA���õ�ע��������ҵȴ�����

	int iPlayerNums = 0;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			iPlayerNums++;
		}
	}

	if (iPlayerNums >= 2)
	{
		::srand((unsigned)::time(NULL)); 
		int iUserSel = rand()%iPlayerNums;

		iPlayerNums = 0;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (m_pUserInfo[i] != NULL)
			{
				if (iPlayerNums++ == iUserSel)
				{
					iUserSel = i;
					break;
				}
			}
		}

		//��¼����һ�������Ҫ���õ�ע
		//m_GameData.m_tagDeskCfg.dz.byUser = iUserSel;


		//��ѡ���Ե��жϣ������ĸ�������п��ܻ�����ҵĽ����С�ڶ�ע����ô��Щ��ע��Ӧ�ĵ�ע���ܱ�ѡ��
		bool bBreak = false;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < PLAY_COUNT; ++j)
			{
				if (m_pUserInfo[j] != NULL)
				{
					if (m_pUserInfo[j]->m_UserData.i64Money < m_GameData.m_tagDeskCfg.dz.iUppers[i])
					{
						bBreak = true;
						break;
					}
				}
			}

			if (bBreak)
			{
				break;
			}

			//m_GameData.m_tagDeskCfg.dz.bSelects[i] = true;
		}


		//�㲥��ע��ʾ
		TCfgDesk tagCfgDesk;
		//���ƿ�ѡ��עֵ
		::memcpy(tagCfgDesk.dz.iLowers,m_GameData.m_tagDeskCfg.dz.iLowers,sizeof(tagCfgDesk.dz.iLowers));
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (m_pUserInfo[i] != NULL)
			{
				tagCfgDesk.dz.bCanSet = (i == iUserSel);
				SendGameData(i,&tagCfgDesk,sizeof(TCfgDesk),MDM_GM_GAME_NOTIFY,ASS_CFG_DESK,0);
			}
		}

		tagCfgDesk.dz.bCanSet = false;
		SendWatchData(m_bMaxPeople,&tagCfgDesk,sizeof(TCfgDesk),MDM_GM_GAME_NOTIFY,ASS_CFG_DESK,0);

		//�㲥��ע�󣬱�־����Ϸ����GS_CONFIG_NOTE״̬�����õ�ע״̬��
		//m_bGameStation = GS_CONFIG_NOTE;
	}

	return true;
}

//���һ��ׯ�¼�
bool CServerGameDesk::OnEventRandom()
{	
	::srand((unsigned)::time(NULL)); 

	if (m_GameData.m_iDealPeople == 255)
	{
		// ��һ�����ׯ
		m_GameData.m_iDealPeople = rand() % PLAY_COUNT;
		
		if (m_pUserInfo[m_GameData.m_iDealPeople] == NULL)
		{
			m_GameData.m_iDealPeople = GetNextUserStation(m_GameData.m_iDealPeople);
		}
	}
	else
	{
		// ������ׯ
		m_GameData.m_iDealPeople = GetNextUserStation(m_GameData.m_iDealPeople);
	}

	m_GameData.m_byNTUser = m_GameData.m_iDealPeople;

	////////////////////////////////////////////////////

	if (GetPlayerCount() == 2)
	{
		m_GameData.m_bSmallBlind = m_GameData.m_byNTUser;
		m_GameData.m_bBigBlind = GetNextUserStation(m_GameData.m_byNTUser);		// ��ȡ��äעλ��
	}
	else
	{
		m_GameData.m_bSmallBlind = GetNextUserStation(m_GameData.m_byNTUser);	// ��ȡСäעλ��
		m_GameData.m_bBigBlind = GetNextUserStation(m_GameData.m_bSmallBlind);	// ��ȡ��äעλ��
	}

	// �㲥�»غϿ�ʼ��
	TNextRoundBeginInfo tagNextRoundBeginInfo;
	tagNextRoundBeginInfo.byNTUser = m_GameData.m_byNTUser;
	tagNextRoundBeginInfo.bSmallBlind = m_GameData.m_bSmallBlind;
	tagNextRoundBeginInfo.bBigBlind = m_GameData.m_bBigBlind;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		// ֪ͨ��Ϸ��ʼ
		SendGameData(i,&tagNextRoundBeginInfo,sizeof(TNextRoundBeginInfo),MDM_GM_GAME_NOTIFY,ASS_NEW_ROUND_BEGIN,0);
	}

	SendWatchData(m_bMaxPeople,&tagNextRoundBeginInfo,sizeof(TNextRoundBeginInfo),MDM_GM_GAME_NOTIFY,ASS_NEW_ROUND_BEGIN,0);

	//����ϴ������
	ActionRedirect(IDEV_SENDACARD, 1);

	return true;
}

//ϴ���¼�
bool CServerGameDesk::OnEventRandCard()
{
	int iCardsNum = sizeof(m_GameData.m_byCards);
	GAlg::RandCard(m_GameData.m_byCards,m_GameData.m_tagDeskCfg.Rule.nPokeNum,m_bDeskIndex);

	//BYTE bCard[] = {0x08, 0x36, 0x03, 0x0A, 0x01, 0x05, 0x0C, 0x32, 0x17, 0x34, 0x14, 0x1B, 0x07, 0x21, 0x22};
	//memcpy(m_GameData.m_byCards, bCard, sizeof(bCard));

	//������������
	ActionRedirect(IDEV_RANDOM, 1);
	
	return true;
}

//Ϊÿ������·�2�ŵ��Ƶ��¼�
bool CServerGameDesk::OnEventSendACard()
{
	TCards tagCards;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL && TYPE_PLAY_GAME == m_iUserStation[i]) //�Ϸ����
		{
			//Ϊ���iȡ2����
			m_GameData.DealCardsTo(i, MAX_DEAL_CARDS);
		}
	}

#ifdef SUPER
	//���ƺ���
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(CanGetGoodCard(i))
		{
			m_GameData.GetGoodCard(i);
			break;
		}
	}
#endif
	
	tagCards.iCardsNum = MAX_DEAL_CARDS;
	tagCards.nTypeCard = SEND_A_CAND;

	//�㲥��������
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pUserInfo[i] != NULL && TYPE_PLAY_GAME == m_iUserStation[i]) //�Ϸ����
		{
			tagCards.byUser = i;

			//�õ����i������
			m_GameData.m_UserData[i].GetHandCards(tagCards.byCards);
			SendGameData(i, &tagCards,sizeof(TCards), MDM_GM_GAME_NOTIFY, ASS_SEND_A_CARD, 0);
		}
	}

	memset(tagCards.byCards,0,sizeof(tagCards.byCards));
	SendWatchData(m_bMaxPeople, &tagCards,sizeof(TCards),MDM_GM_GAME_NOTIFY, ASS_SEND_A_CARD, 0);

	OutputDebugString("dxh: Ϊÿ������·�2�ŵ��Ƶ��¼�");
	//ActionRedirect(IDEV_BET_START, 1000);
	m_bFaiPaiFinish = false;

	OpenEventProtection(IDEV_SENDACARD, GetPlayerCount() * 2);

	return true;

}

//Ϊ�����Ƶ��¼�
bool CServerGameDesk::OnEventSend3Card()
{
	TCards tagCards;

	CString str;
	str.Format("dxh : ��ǰ����λ��: %d", m_GameData.m_iGetCardPos);
	OutputDebugString(str);
	memcpy(m_GameData.m_iCards, &m_GameData.m_byCards[m_GameData.m_iGetCardPos], MAX_THREE_CARDS);
	m_GameData.m_iGetCardPos += MAX_THREE_CARDS;
	m_GameData.m_iCardsCount += MAX_THREE_CARDS;

	memcpy(tagCards.byCards, m_GameData.m_iCards, sizeof(BYTE) * m_GameData.m_iCardsCount);
	tagCards.iCardsNum += m_GameData.m_iCardsCount;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SendGameData(i, &tagCards, sizeof(tagCards), MDM_GM_GAME_NOTIFY, ASS_SEND_3_CARD, 0);
	}	
	
	SendWatchData(m_bMaxPeople,&tagCards, sizeof(tagCards), MDM_GM_GAME_NOTIFY, ASS_SEND_3_CARD, 0);

	OutputDebugString("dxh: Ϊ�����Ƶ��¼�");


	if (m_GameData.m_bAutoSendCard)
	{
		m_GameData.m_nTurnNums++;

		// ��һ�Ź�����
		ActionRedirect(IDEV_SEND_4_5_CARD, 1200);
	}
	else
	{
		ActionRedirect(IDEV_BET_START, 500);
	}

	return true;
}

//Ϊ��1�Ź������Ƶ��¼�
bool CServerGameDesk::OnEventSend1Card()
{
	CString str;
	str.Format("dxh : ��ǰ����λ��: %d", m_GameData.m_iGetCardPos);
	OutputDebugString(str);

	if (m_GameData.m_iCardsCount >= 5)
	{
		OutputDebugString("Server: ���������5����");
		return true;
	}

	TCards tagCards;
	memcpy(&m_GameData.m_iCards[m_GameData.m_iCardsCount], &m_GameData.m_byCards[m_GameData.m_iGetCardPos], MAX_THREE_CARDS);
	m_GameData.m_iGetCardPos += MAX_FIRST_CARDS;
	m_GameData.m_iCardsCount += MAX_FIRST_CARDS;

	memcpy(tagCards.byCards, m_GameData.m_iCards, sizeof(BYTE) * m_GameData.m_iCardsCount);
	tagCards.iCardsNum = m_GameData.m_iCardsCount;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SendGameData(i, &tagCards, sizeof(tagCards), MDM_GM_GAME_NOTIFY, ASS_SEND_4_5_CARD, 0);
	}	

	SendWatchData(m_bMaxPeople,&tagCards, sizeof(tagCards), MDM_GM_GAME_NOTIFY, ASS_SEND_4_5_CARD, 0);

	if (m_GameData.m_bAutoSendCard)
	{
		if (m_GameData.m_nTurnNums < 3)
		{
			m_GameData.m_nTurnNums++;

			// ��һ�Ź�����
			ActionRedirect(IDEV_SEND_4_5_CARD, 1200);
		}
		else 
		{
			// ��Ϸ����, ���н������
			OutputDebugString("dxh:  OnEventSend1Card��, ��Ϸ����, ���н������");
			ActionRedirect(IDEV_COMPARE_CARD, 500);
			return true;
		}
	}
	else
	{
		ActionRedirect(IDEV_BET_START, 500);
	}

	return true;
}

//�״���ע�¼�
bool CServerGameDesk::OnEventBetStart()
{
	OutputDebugString("Server:�״���ע�¼�");
	//��λ�����ע��ζ��ǰ��Ϸ�Ļ������1
	m_GameData.m_nTurnNums++;
	for (int i = 0; i < m_GameData.m_nTurnNums; i++)
	{
		SendGameData(i, MDM_GM_GAME_NOTIFY, ASS_SORT_OUT, 0);
	}

	SendWatchData(m_bMaxPeople, MDM_GM_GAME_NOTIFY, ASS_SORT_OUT, 0);

	// ����С��ע��ע
	//�����ǰ�ǵ�һ����ע��ת���Сäע�Զ���ע����\
	�ӵڶ��ֿ�ʼ��ת����λ�����ע����(Ҳ�ʹ�Сäע��ʼ)
	m_GameData.m_byTokenUser = GetLastUserStation(m_GameData.m_bSmallBlind);

	CString str;
	str.Format("YY:m_GameData.m_byTokenUser == %d",m_GameData.m_byTokenUser);
	OutputDebugString(str);

	m_GameData.m_iBetCounts = 0;
	m_GameData.m_iCallCounts = 0;
	m_GameData.m_iRaiseCounts = 0;
	//m_GameData.m_iAllinUser = 0;
	m_GameData.m_iCheckCount = 0;
	//m_GameData.m_iFoldCount = 0;
	m_GameData.m_bFirstStation = 255;

	OutputDebugString("Server: �״���ע�¼�");

    // �µ�һ�ֿ�ʼ
    m_GameData.m_bNewTurn = true;
	// �µ�һ�ֿ�ʼ
	m_GameData.m_bNewTurns = true;
	

	if(m_GameData.m_nTurnNums == 1)
	{
		// ��һ��ֻ���Ը�ע����ע��ȫ�¡�����
		m_GameData.MakeVerbs(UD_VF_CALL|UD_VF_ADD|UD_VF_FOLD|UD_VF_ALLIN);

		// ���д�Сäע��ע
		ActionRedirect(IDEV_BET_BLINDS_AUTO, 500);
	}
	else
	{
		// ��һ��֮��ÿ�ֿ�ʼ������ע�����ơ����ơ�ȫ��
		m_GameData.MakeVerbs(UD_VF_BET|UD_VF_FOLD|UD_VF_CHECK|UD_VF_ALLIN);

		// �µ�һ�ֿ�ʼ
		ActionRedirect(IDEV_BET_NEXT, 500);
	}

	return true;
}

//��Сäעǿ��ͬʱ��ע�¼�
bool CServerGameDesk::OnEventBetBlindsAuto()
{
	TBet tagBet;
	tagBet.nType = ET_AUTO;

	//Сäעǿ����ע
	m_GameData.m_byTokenUser = m_GameData.m_bSmallBlind;

	//Сäעǿ����ע
	tagBet.byUser = m_GameData.m_byTokenUser;
	tagBet.nMoney = m_GameData.m_tagDeskCfg.dz.iSmallBlindNote;

	// ģ����ע��Ϣ
	OnUserBet(m_GameData.m_bSmallBlind, &tagBet);
	CString str;
	str.Format("dxh: ------------- ��ע���: %d", m_GameData.m_byTokenUser);
	OutputDebugString(str);

	//��äעǿ����ע
	m_GameData.m_byTokenUser = m_GameData.m_bBigBlind;

	//��äעǿ����ע
	tagBet.byUser = m_GameData.m_byTokenUser;
	tagBet.nMoney = m_GameData.m_tagDeskCfg.dz.iBigBlindNote;

	// ģ����ע��Ϣ
	OnUserBet(m_GameData.m_bBigBlind, &tagBet);

	str.Format("dxh: ------------- ��ע���: %d", m_GameData.m_byTokenUser);
	OutputDebugString(str);

	// ֪ͨ��äע��һ����ҿ�ʼ��ע
	ActionRedirect(IDEV_BET_NEXT, 500);

	return true;
}

//��һλ�����ע�¼�
bool CServerGameDesk::OnEventBetNext()
{
	// �ƽ����Ƶ���һ�����
	m_GameData.m_byTokenUser = GetNextUserStation(m_GameData.m_byTokenUser);

	//�㲥����
	TToken tagToken;
	tagToken.byUser = m_GameData.m_byTokenUser;
	tagToken.byVerbFlag = m_GameData.m_byVerbFlag;
    tagToken.bNewTurn = m_GameData.m_bNewTurn;
	tagToken.bNewTurns = m_GameData.m_bNewTurns;
	tagToken.nTurnNums = m_GameData.m_nTurnNums;
	tagToken.nCallMoney = m_GameData.m_nMaxBetMoney - m_GameData.GetBetMoney(m_GameData.m_byTokenUser);
	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i,&tagToken,sizeof(TToken),MDM_GM_GAME_NOTIFY,ASS_TOKEN,0);
		}
	}	

	SendWatchData(m_bMaxPeople,&tagToken,sizeof(TToken),MDM_GM_GAME_NOTIFY,ASS_TOKEN,0);

    // �����Զ���������, �ͻ����ڹ����ʱ���ڲ���, ��������Զ��������
	OpenEventProtection(IDEV_BET_NEXT,m_GameData.m_tagDeskCfg.Time.byOperate + 10);
    //ActionRedirect(IDEV_AUTO_OPERATE, (m_GameData.m_tagDeskCfg.Time.byOperate + 10) * 1000);
	return true;
}

//��ʼ��һ�غ���Ϸ�¼�����
bool CServerGameDesk::OnEventNextRoundBegin()
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			if (!m_GameData.m_UserData[i].m_bIn)
			{
				return true;
			}

			m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;
		}
	}

	OutputDebugString("dxh: OnEventNextRoundBegin");
	GameBegin(ALL_ARGEE);
	return true;
}

// ����
bool CServerGameDesk::OnCompareCard()
{
	OutputDebugString("dxh: ���б�������");

	// ���б�������
	TCompareCard cmd;
	memset(&cmd, 0, sizeof(TCompareCard));

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i])
		{
			continue;
		}

		// �õ��������
		memcpy(cmd.bHandCards[i], m_GameData.m_UserData[i].m_byMyCards, sizeof(cmd.bHandCards[i]));
		cmd.nHandCardCount[i] = 2;

		CString str,str1;
		str.Format("jan: ���λ��: %d, �û�ID:%d, ����: ", i, m_pUserInfo[i]->m_UserData.dwUserID);

		for (int j = 0; j < 2; j++)
		{
			BYTE bNumber = m_GameData.m_UserData[i].m_byMyCards[j] & 0x0F;
			BYTE bShape = (m_GameData.m_UserData[i].m_byMyCards[j] & 0xF0) >> 4;
			str1.Format("%s%s(0x%x) ", szShape[bShape], szNumber[bNumber],  m_GameData.m_UserData[i].m_byMyCards[j]);
			str += str1;
		}

		str += "  ������:";
		for (int j = 0; j < 5; j++)
		{
			BYTE bNumber = m_GameData.m_iCards[j] & 0x0F;
			BYTE bShape = (m_GameData.m_iCards[j] & 0xF0)>>4;
			str1.Format("%s%s(0x%x) ", szShape[bShape], szNumber[bNumber], m_GameData.m_iCards[j]);
			str += str1;
		}

		OutputDebugString(str);

		BYTE byResultCard[7];
		memset(byResultCard, 0, sizeof(byResultCard));

		// ��������
		cmd.nCardKind[i] = m_Logic.AnalysisCard(m_GameData.m_UserData[i].m_byMyCards, 2, m_GameData.m_iCards, 5, byResultCard);
		cmd.nCardCount = 5;
		memcpy(m_GameData.m_bResultCards[i],byResultCard, sizeof(BYTE) * cmd.nCardCount);  // ����������
		memcpy(cmd.bCardsEx[i], &byResultCard[cmd.nCardCount], sizeof(BYTE) * 2);           // ʣ�����û�õ���

		memcpy(cmd.bCards[i], m_GameData.m_bResultCards[i], sizeof(BYTE) * cmd.nCardCount);
		
		
		str.Format("jan: ���λ��: %d, �û��ǳ�:%s,  ����: ID:%d  ����:%s, ��ֵ: ", i, m_pUserInfo[i]->m_UserData.nickName,  cmd.nCardKind[i], szName[cmd.nCardKind[i]]);
		OutputDebugString(str);

		for (int j = 0; j < 5; j++)
		{
			BYTE bNumber = cmd.bCards[i][j] & 0x0F;
			BYTE bShape = (cmd.bCards[i][j] & 0xF0) >> 4;
			str1.Format("%s%s(0x%x) ", szShape[bShape], szNumber[bNumber], cmd.bCards[i][j]);
			str += str1;
		}

		OutputDebugString(str);
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		// ֪ͨ�ͻ���
		SendGameData(i, &cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_COMPARE_CARD, 0);
	}
	
	SendWatchData(m_bMaxPeople,&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_COMPARE_CARD, 0);



	OutputDebugString("Server: �����������");

	// �����������
	ActionRedirect(IDEV_RESULT, 1500);
	return true;
}

///���ƣ�OnUserAgreeGame
///�������������ͬ����Ϸ
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return 
bool CServerGameDesk::OnUserAgreeGame(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser)
{
	//ֻҪ�����ͬ����Ϸ����ζ����Ϸ����ȴ��������ͬ���״̬
	//m_bGameStation = GS_WAIT_ARGEE;
	OutputDebugString("dxh: OnUserAgreeGame");
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}


///���ƣ�OnUserSetDiZhu
///������������������õ�ע
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return 
bool CServerGameDesk::OnUserCfgDesk(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	TCfgDesk* pCfgDesk = (TCfgDesk*)pData;

	//��ʱ�����Ӧ���϶�ͬ����Ϸ
	m_GameData.SetAgreeStateTo(bDeskStation);

	//��¼��עֵ��ͨ����ע���㶥ע
	m_GameData.m_tagDeskCfg.dz.iLower = m_GameData.m_tagDeskCfg.dz.iLowers[pCfgDesk->dz.bySelect];
	m_GameData.m_tagDeskCfg.dz.iUpper = m_GameData.m_tagDeskCfg.dz.iUppers[pCfgDesk->dz.bySelect];

	//�㲥�趨��ע����Ϣ
	for (int i  = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i,&m_GameData.m_tagDeskCfg,sizeof(TDeskCfg),MDM_GM_GAME_NOTIFY,ASS_DESK_CFG,0);
		}
	}

	SendWatchData(m_bMaxPeople,&m_GameData.m_tagDeskCfg,sizeof(TDeskCfg),MDM_GM_GAME_NOTIFY,ASS_DESK_CFG,0);

	//�㲥��ע�󣬱�־����Ϸ����GS_AGREE_NOTE״̬��ͬ���ע״̬��
	//m_bGameStation = GS_AGREE_NOTE;

	return true;
}

///���ƣ�OnUserSetNoteTimeout
///������������������õ�ע��ʱ
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return 
bool CServerGameDesk::OnUserCfgDeskTimeout(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	//��ʱ�����Ӧ���϶���ͬ����Ϸ���޸��û��������û�״̬
	m_GameData.SetDisAgreeStateTo(bDeskStation);

	//�·���ע��ʱ��Ϣ������ɢ����
	for (int i  = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL && i != bDeskStation)
		{
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_CFG_DESK_TIMEOUT,0);
		}
	}

	GameFinish(0,GF_SALE);

	return true;
}

///���ƣ�OnUserAgreeDiZhu
///���������������ͬ���ע
///@param bDeskStation λ��, pNetHead ��Ϣͷ,pData ��Ϣ����,uSize ��Ϣ������,uSocketID id,bWatchUser �Ƿ��Թ�
///@return 
bool CServerGameDesk::OnUserAgreeDeskCfg(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{

	//if (m_pUserInfo[bDeskStation] == NULL)
	//{
	//	return false;
	//}

	////�޸��û��������û�״̬
	//m_GameData.SetAgreeStateTo(bDeskStation);

	////�㲥���bDeskStationͬ����Ϣ

	//TAgree tagAgree;
	//tagAgree.byUser = bDeskStation;
	//tagAgree.bAgree = true;

	////�Ƿ������˶�ͬ����Ϸ
	//tagAgree.bAllAgree = (m_GameData.GetAgreeNums() == PLAY_COUNT);

	//for (int i = 0; i < PLAY_COUNT; ++i)
	//{
	//	if (m_pUserInfo[i])
	//	{
	//		SendGameData(i,&tagAgree,sizeof(TAgree),MDM_GM_GAME_NOTIFY,ASS_AGREE,0);
	//	}
	//}

	////���������Ҷ�ͬ���ע���趨����������������
	//if (tagAgree.bAllAgree)
	//{
	//	//�㲥�������׼���ã�tagAgree.bAllAgree = true����֪ͨ�󣬱�־����Ϸ����GS_PLAY_GAME״̬����Ϸ����״̬��\
	//	  Ҳ��־����Ϸ�Ľ����һ�غϣ��غϼ���Ҫ��1
	//	
	//	m_bGameStation = GS_PLAY_GAME;

	//	//����ϴ������
	//	ActionRedirect(IDEV_RAND_CARD,500);
	//}

	return true;
}

// ��Ҵ�����
bool CServerGameDesk::OnUserSubMoney(BYTE bDeskStation, void* pData)
{
	TSubMoney *pCmd = (TSubMoney*)pData;

	if (pCmd == NULL)
	{
		return false;
	}

	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}

	if (pCmd->nMoney[bDeskStation] <= m_GameData.m_tagDeskCfg.dz.iUntenMoney
	|| m_pUserInfo[bDeskStation]->m_UserData.i64Money < pCmd->nMoney[bDeskStation]
	|| m_pUserInfo[bDeskStation]->m_UserData.i64Money < m_GameData.m_tagDeskCfg.dz.iSubMinMoney)
	{
		// ���Ϸ�����
		return true;
	}

	if (pCmd->nMoney[bDeskStation] < m_GameData.m_tagDeskCfg.dz.iSubMinMoney) 
	{
		// ��ҵ�ǰ������������ʹ���ֵʱ������ʹ���ֵ���赱ǰ��ǰ������
		pCmd->nMoney[bDeskStation] = m_GameData.m_tagDeskCfg.dz.iSubMinMoney;
	}
	else if (pCmd->nMoney[bDeskStation] > m_GameData.m_tagDeskCfg.dz.iSubMaxMoney)
	{
		// ��ҵ�ǰ�����������ߴ���ֵʱ������ߴ���ֵ���赱ǰ��ǰ������
		pCmd->nMoney[bDeskStation] = m_GameData.m_tagDeskCfg.dz.iSubMaxMoney;
	}

	if (pCmd->nMoney[bDeskStation] > m_pUserInfo[bDeskStation]->m_UserData.i64Money)
	{
		pCmd->nMoney[bDeskStation] = m_pUserInfo[bDeskStation]->m_UserData.i64Money;
	}

	m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;

	m_GameData.m_UserData[bDeskStation].m_bIn = true;

	// ���������
	m_GameData.m_UserMoney[bDeskStation] = pCmd->nMoney[pCmd->bDeskStation];
	
	// ���õ�ǰ���ý��
	TSubMoney pBag;
	pBag.bDeskStation = bDeskStation;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			pBag.bIn[i] = m_GameData.m_UserData[i].m_bIn;
			
			CString str,str1;

			str.Format("Server��Ϸ��������-Server:��Ϸ����:%d,���ID:%d,����ǳ�:%s,��Ҵ�����:%I64d,�����ʵ���:%I64d",
				m_bDeskIndex,m_pUserInfo[i]->m_UserData.dwUserID,m_pUserInfo[i]->m_UserData.szName,pCmd->nMoney[i],
				m_pUserInfo[i]->m_UserData.i64Money);

			
			str1.Format("Server: ���:%d(λ��:%d), ������: %I64d, m_bIn: %d", m_pUserInfo[i]->m_UserData.dwUserID, i, m_GameData.m_UserMoney[i],  m_GameData.m_UserData[i].m_bIn);
			OutputDebugString(str1);
		}
	}

	memcpy(pBag.nMoney,m_GameData.m_UserMoney,sizeof(m_GameData.m_UserMoney));
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SendGameData(i, &pBag,sizeof(pBag),  MDM_GM_GAME_NOTIFY, ASS_SHOW_IN_MONEY, 0);
	}
	
	SendWatchData(m_bMaxPeople,&pBag,sizeof(pBag),  MDM_GM_GAME_NOTIFY, ASS_SHOW_IN_MONEY, 0);

	int nCount = 0;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			nCount++;
			if (false == m_GameData.m_UserData[i].m_bIn)
			{
				OutputDebugString("Server: �����δ������");
				return true;
			}
		}	
	}

	if (nCount >= 2)
	{
		// ��Ϸ��ʼ
		GameBegin(ALL_ARGEE);
	}

	return true;
}

///���ƣ�OnUserBet
///���������������ע��Ϣ
///@param bDeskStation λ��, pData ��Ϣ����
///@return 
bool CServerGameDesk::OnUserBet(BYTE bDeskStation, void* pData ,bool bUserforceExit)
{
	OutputDebugString("Server: ���������ע��Ϣ");
	
	// ���ǵ�ǰ�������
	if (m_GameData.m_byTokenUser != bDeskStation)
	{
		OutputDebugString("Server: ���ǵ�ǰ�������");
		return false;
	}
	// ��Ҳ����� �� ��������� �� ������ע��� �� ��;�������
	if (m_pUserInfo[bDeskStation] == NULL || m_GameData.m_bGiveUp[bDeskStation] || m_GameData.m_bNoBet[bDeskStation])
	{
		OutputDebugString("Server: ��Ҳ����ڻ����������");
		return true;
	}

	if (m_GameData.m_bAutoSendCard)
	{
		OutputDebugString("Server: �Ѿ���������, ������Ҷ����ɽ��в���, �ȴ���Ϸ��������");
		return true;
	}

	CString str;

    // ֹͣ�Զ���ʱ����
    KillMyTimer(IDEV_AUTO_OPERATE);

	TBet * pCmd = (TBet *)pData;

	switch (pCmd->nType)
	{
	case ET_BET:     // ��ע
	case ET_AUTO:
		{
			if (pCmd->nMoney < 0 || pCmd->nMoney > m_GameData.m_UserMoney[bDeskStation])
			{
				// ��ע�������������, ����Լ�������ȫ��
				pCmd->nMoney = m_GameData.m_UserMoney[bDeskStation];
				str.Format("Server:��ע�������������, ����Լ�������ȫ��:%I64d",pCmd->nMoney);
				OutputDebugString(str);
			}

			m_GameData.m_UserMoney[bDeskStation] -= pCmd->nMoney;
			m_GameData.m_nBetMoney[bDeskStation][m_GameData.m_nBetPool] += pCmd->nMoney;

			if (m_GameData.m_UserMoney[bDeskStation] == 0)
			{
				// ����ȫ�´���
				m_GameData.m_bNoBet[bDeskStation] = true;
				pCmd->nType = ET_ALLIN;
				m_GameData.m_bAllBetStation = bDeskStation;
				m_GameData.m_iAllinUser++;
				m_GameData.m_byAllPools[m_GameData.m_nBetPool]++;
				m_GameData.m_bAllBet[bDeskStation][m_GameData.m_nBetPool] = true;
			}

			int nAllBetMoney = m_GameData.GetBetMoney(bDeskStation);
			if (m_GameData.m_nMaxBetMoney < nAllBetMoney)
			{
				m_GameData.m_nMaxBetMoney = nAllBetMoney;  // ��¼���Ľ��
			}

			m_GameData.m_bBetStation = bDeskStation;
			m_GameData.m_iBetCounts++;
			CString str;
			str.Format("Server:m_GameData.m_bBetStation == %d,bDeskStation == %d",m_GameData.m_bBetStation,bDeskStation);
			OutputDebugString(str);
		}
		break;
	case ET_CALL:    // ��ע
		{
			// 2012-10-11 duanxiaohui add ��ֹ�ͻ����Ҹ����, ��֤��עΪһ����������ע
			int nAllMoeny = m_GameData.GetBetMoney(bDeskStation);
			if ((nAllMoeny + pCmd->nMoney) < m_GameData.m_nMaxBetMoney)
			{
				pCmd->nMoney = m_GameData.m_nMaxBetMoney - nAllMoeny;   // ��ע�쳣��, ��������Ӧ��Ҫ���Ľ��
			}		
			// end duanxiaohui

			if (pCmd->nMoney < 0 || pCmd->nMoney > m_GameData.m_UserMoney[bDeskStation])
			{
				// ��ע�������������, ����Լ�������ȫ��
				pCmd->nMoney = m_GameData.m_UserMoney[bDeskStation];
			}

			m_GameData.m_UserMoney[bDeskStation] -= pCmd->nMoney;
			m_GameData.m_nBetMoney[bDeskStation][m_GameData.m_nBetPool] += pCmd->nMoney;

			if (m_GameData.m_UserMoney[bDeskStation] == 0)
			{
				// ����ȫ�´���
				m_GameData.m_bNoBet[bDeskStation] = true;
				pCmd->nType = ET_ALLIN;
				m_GameData.m_bAllBetStation = bDeskStation;
				m_GameData.m_iAllinUser++;
				m_GameData.m_byAllPools[m_GameData.m_nBetPool]++;
				m_GameData.m_bAllBet[bDeskStation][m_GameData.m_nBetPool] = true;
			}
			else
			{
				m_GameData.m_iCallCounts++;
			}

			int nAllBetMoney = m_GameData.GetBetMoney(bDeskStation);
			if (m_GameData.m_nMaxBetMoney < nAllBetMoney)
			{
				m_GameData.m_nMaxBetMoney = nAllBetMoney;  // ��¼���Ľ��
			}
		}
		break;
	case ET_ADDNOTE: // ��ע    
		{ 
			if (pCmd->nMoney < 0 || pCmd->nMoney > m_GameData.m_UserMoney[bDeskStation])
			{
				// ��ע�������������, ����Լ�������ȫ��
				pCmd->nMoney = m_GameData.m_UserMoney[bDeskStation];
			}

			m_GameData.m_UserMoney[bDeskStation] -= pCmd->nMoney;
			m_GameData.m_nBetMoney[bDeskStation][m_GameData.m_nBetPool] += pCmd->nMoney;

			if (m_GameData.m_UserMoney[bDeskStation] == 0)
			{
				// ����ȫ�´���
				m_GameData.m_bNoBet[bDeskStation] = true;
				pCmd->nType = ET_ALLIN;
				m_GameData.m_bAllBetStation = bDeskStation;
				m_GameData.m_iAllinUser++;
				m_GameData.m_byAllPools[m_GameData.m_nBetPool]++;
				m_GameData.m_bAllBet[bDeskStation][m_GameData.m_nBetPool] = true;
			}
			else
			{
				m_GameData.m_iRaiseCounts++;
			}

			int nAllBetMoney = m_GameData.GetBetMoney(bDeskStation);
			if (m_GameData.m_nMaxBetMoney < nAllBetMoney)
			{
				m_GameData.m_nMaxBetMoney = nAllBetMoney;  // ��¼���Ľ��
			}

			m_GameData.m_bRaiseStation = bDeskStation;

			CString str;
			str.Format("Server:m_GameData.m_bRaiseStation == %d,bDeskStation == %d",m_GameData.m_bRaiseStation,bDeskStation);
			OutputDebugString(str);
		}
		break;
	case ET_ALLIN:   // ȫ��
		{
			pCmd->nMoney = m_GameData.m_UserMoney[bDeskStation];
			
			str.Format("Server:m_UserMoney = %I64d",m_GameData.m_UserMoney[bDeskStation]);
			OutputDebugString(str);

			m_GameData.m_bNoBet[bDeskStation] = true;
			m_GameData.m_bAllBetStation = bDeskStation;
			m_GameData.m_UserMoney[bDeskStation] -= pCmd->nMoney;
			m_GameData.m_nBetMoney[bDeskStation][m_GameData.m_nBetPool] += pCmd->nMoney; 
			m_GameData.m_iAllinUser++;
			m_GameData.m_byAllPools[m_GameData.m_nBetPool]++;
			m_GameData.m_bAllBet[bDeskStation][m_GameData.m_nBetPool] = true;

			__int64 nAllBetMoney = m_GameData.GetBetMoney(bDeskStation);
			if (m_GameData.m_nMaxBetMoney < nAllBetMoney)
			{
				m_GameData.m_nMaxBetMoney = nAllBetMoney;  // ��¼���Ľ��
			}
		}
		break; 
	case ET_CHECK:   // ����
		{
			if (m_GameData.m_bFirstStation == 255)
			{
				m_GameData.m_bFirstStation = bDeskStation;  // ��¼��һ���������
			}

			m_GameData.m_iCheckCount++;
		}
		break;
	case ET_FOLD:    // ����
		{
			m_GameData.m_bGiveUp[bDeskStation] = true;

			if(!bUserforceExit)
			{
				m_GameData.m_iFoldCount++;
			}

			if (m_GameData.m_byNTUser == bDeskStation)
			{
				m_GameData.m_byNTUser = 255;
			}
			else if (m_GameData.m_bSmallBlind == bDeskStation)
			{
				// Сäע�������, ��Сäע��Ϊ255
				m_GameData.m_bSmallBlind = 255;
			}
			else if (m_GameData.m_bBigBlind == bDeskStation)
			{
				// ��äע�������, �Ѵ�äע��Ϊ255
				m_GameData.m_bBigBlind = 255;
			}
			
		}
		break;
	default:
		break;
	}

	str.Format("Server: ���:%d, ��ע:%I64d, ��%I64d����ע��, ʣ����:%I64d",bDeskStation, pCmd->nMoney,m_GameData.m_nBetPool+1, m_GameData.m_UserMoney[bDeskStation]);
	OutputDebugString(str);

	// ����ע���ݷ��͵��ͻ�����ʾ
	TBet tagAnteVerb;
	tagAnteVerb.byUser = bDeskStation;
	tagAnteVerb.nType = pCmd->nType;
	tagAnteVerb.nMoney = pCmd->nMoney;
	//memcpy(tagAnteVerb.bUserman,m_GameData.m_bGiveUp,sizeof(m_GameData.m_bGiveUp));
	tagAnteVerb.nBetMoney = m_GameData.m_nMaxBetMoney;

	str.Format("Server:��ע��%I64d",tagAnteVerb.nBetMoney);
	OutputDebugString(str);

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i, &tagAnteVerb, sizeof(tagAnteVerb), MDM_GM_GAME_NOTIFY, ASS_BET_INFO,0);
		}
	}

	SendWatchData(m_bMaxPeople,&tagAnteVerb, sizeof(tagAnteVerb), MDM_GM_GAME_NOTIFY, ASS_BET_INFO,0);
	CloseEventProtection();

	// �ж���һ������
	if (pCmd->nType != ET_AUTO)
	{
		if (m_GameData.m_iFoldCount >= GetPlayerCount(true) - 1)
		{
			OutputDebugString("Server: �����������, ��Ϸ����");
			m_GameData.m_bGiveUpOver = true;

#ifdef CLOSE_CARD
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				//�������Ϸ�� ����û�� ��������� ������չʾ�Ƶ���Ϣ
				if(NULL != m_pUserInfo[i] && !m_GameData.m_bGiveUp[i])
				{
					SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_CAN_SHOW_CARD,0);
				}
			}

			// �����������, ��Ϸ����
			ActionRedirect(IDEV_RESULT, 4000);
#else
			// �����������, ��Ϸ����
			ActionRedirect(IDEV_RESULT, 1000);
#endif
			return true;
		}

		bool bEqual = false;

		//if (m_GameData.m_bBigBlind == bDeskStation)		// ���λ�õ��ڴ�äע
		{
			/*CString str;
			str.Format("Server:���λ�õ��ڴ�äע__���%dȫ��%I64d",m_GameData.m_nBetMoney[bDeskStation][m_GameData.m_nBetPool]);
			OutputDebugString(str);*/

			bEqual = true;

			__int64 nMoney = m_GameData.m_nBetMoney[bDeskStation][m_GameData.m_nBetPool];

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (i == bDeskStation || m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || m_GameData.m_bNoBet[i])
				{
					continue;
				}
			
				if (nMoney != m_GameData.m_nBetMoney[i][m_GameData.m_nBetPool])
				{
					bEqual = false;
					break;
				}	
			}

			CString str;
			if (!bEqual)
			{
				int nCount = 0;
				BYTE byStation = -1;
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || m_GameData.m_bNoBet[i])
					{
						continue;
					}

					nCount++;
					byStation = i;
				}

				// ���ֻʣ��һ����, �ж��Ƿ���Ҫ������ע����
				if (nCount == 1)
				{
					bEqual = true;
					nMoney = m_GameData.m_nBetMoney[byStation][m_GameData.m_nBetPool];

					for (int i = 0; i < PLAY_COUNT; i++)
					{
						if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || (m_GameData.m_bNoBet[i] && !m_GameData.m_bAllBet[i][m_GameData.m_nBetPool]))
						{
							continue;
						}

						// �ж����һ�������ע�Ľ���Ƿ����ȫ�µ������ע���, ������ֱ�ӽ��㣬С����Ҫ������ע
						if (nMoney < m_GameData.m_nBetMoney[i][m_GameData.m_nBetPool])
						{
							bEqual = false;
							break;
						}	
					}
				}
			
				str.Format("Server: ============nCount =  %d", nCount);
				OutputDebugString(str);
			}

			str.Format("Server: ============bEqual   %d", bEqual);
			OutputDebugString(str);
		}

		// �µ�һ�ֱ�־��Ϊfalse
		m_GameData.m_bNewTurn = false;
		
		// ��ע��ҵ��ϼҽ���, ��ʼ�µ�һ����ע���ֵ���äעʱ(ǰ��û���˼�ע), ��ʼ�µ�һ����ע
		BYTE bNextStation = GetNextUserStation(bDeskStation);

		str.Format("Server: m_GameData.m_bBetStation=%d,m_GameData.m_bRaiseStation=%d, ��ע��: %d, ��: %d, bEqual=%d, m_GameData.m_iBetCounts: %d, ��ǰ��ע��%d��,��һ���������%d,GetNextUserStation(bDeskStation) == %d,bNextStation == %d",
			m_GameData.m_bBetStation,m_GameData.m_bRaiseStation, bDeskStation, m_GameData.m_bBigBlind, bEqual, m_GameData.m_iBetCounts, m_GameData.m_nTurnNums, m_GameData.m_bFirstStation,GetNextUserStation(bDeskStation),bNextStation);
		OutputDebugString(str);

		bool temp = false;
		if (m_GameData.m_nTurnNums == 1)
		{
			if (m_GameData.m_iBetCounts >= 2)
			{
				temp = true;
			}
			else if (m_GameData.m_bBigBlind != bNextStation)
			{
				temp = true;
			}
		}
		else
		{
			if (m_GameData.m_iBetCounts > 1)
			{
				temp = true;
			}
			else if (m_GameData.m_bBigBlind != bNextStation && (m_GameData.m_bFirstStation == bNextStation
				|| m_GameData.m_bRaiseStation == bNextStation
				|| m_GameData.m_bBetStation == bNextStation))
			{
				temp = true;
			}
		}

		if ((m_GameData.m_bRaiseStation != 255 && bNextStation == m_GameData.m_bRaiseStation)
			//|| (bEqual && m_GameData.m_bRaiseStation == 255 && m_GameData.m_bBigBlind == bDeskStation && m_GameData.m_iCheckCount > 2)
			|| (bEqual && temp)
			//(m_GameData.m_iBetCounts > 2)
			|| (m_GameData.m_iCheckCount > 1 && m_GameData.m_bFirstStation == bNextStation && bEqual) // ȫ��
			|| bNextStation == 255)   // ȫ��  
		{
			m_GameData.m_iCheckCount = 0;

			// �µ�һ�ֱ�־��Ϊfalse
			m_GameData.m_bNewTurns = false;
			OutputDebugString("Server:��ע��ҵ��ϼҽ���, ��ʼ�µ�һ����ע���ֵ���äעʱ(ǰ��û���˼�ע), ��ʼ�µ�һ����ע");
			// ����ȫ��֮��, ��Ҫ����߳�
			if (m_GameData.m_byAllPools[m_GameData.m_nBetPool] > 0)
			{
				// ���б߳ؼ���
				str.Format("Server: ��%I64d���߳�����ȫ������:%d, ��Ҫ���б߳ز�������", m_GameData.m_nBetPool+1, m_GameData.m_byAllPools[m_GameData.m_nBetPool]);
				OutputDebugString(str);

				int i, j;
				bool bIsNewPool = false;
				int nBetPool = m_GameData.m_nBetPool;
				for (i = 0; i < m_GameData.m_byAllPools[m_GameData.m_nBetPool]; i++)
				{
					// ���ҳ�����С��ע���, ��Ϊ��ע������
					__int64 nMinMoney = /*2147483647*/9223372036854775807;
					bIsNewPool = false;

					for (j = 0; j < PLAY_COUNT; j++)
					{
						if (m_pUserInfo[j] == NULL || m_GameData.m_bGiveUp[j] || m_GameData.m_nBetMoney[j][nBetPool] <= 0/* || m_GameData.m_bNoBet[j]*/)
						{
							continue;
						}

						if (m_GameData.m_nBetMoney[j][nBetPool] < nMinMoney)
						{
							nMinMoney = m_GameData.m_nBetMoney[j][nBetPool];
						}
					}

					CString str;
					str.Format("Server: ��ǰ%d�߳�, ���ͽ��:%I64d ���зֳ�", nBetPool, nMinMoney);
					OutputDebugString(str);

					for (j = 0; j < PLAY_COUNT; j++)
					{
						if (m_pUserInfo[j] == NULL || m_GameData.m_bGiveUp[j] || m_GameData.m_nBetMoney[j][nBetPool] <= 0/* || m_GameData.m_bNoBet[j]*/)
						{
							continue;
						}

						str.Format("Server: ���:%d, ��ǰ��ע���:%I64d", m_pUserInfo[j]->m_UserData.dwUserID, m_GameData.m_nBetMoney[j][nBetPool]);
						OutputDebugString(str);

						// �жϸ������ע�Ľ���Ƿ����ȫѹ����ҽ��
						if (m_GameData.m_nBetMoney[j][nBetPool] > nMinMoney)
						{
							// �Ѷ���Ľ��ת���߳�ȥ
							__int64 nPoolMoney = m_GameData.m_nBetMoney[j][nBetPool] - nMinMoney;
							m_GameData.m_nBetMoney[j][nBetPool] -= nPoolMoney;
							m_GameData.m_nBetMoney[j][nBetPool+1] = nPoolMoney;

							str.Format("Server: ���:%d, �ֳغ�: %I64d, %I64d", m_pUserInfo[j]->m_UserData.dwUserID, m_GameData.m_nBetMoney[j][nBetPool], nPoolMoney);
							OutputDebugString(str);

							str.Format("Server: �ֳؽ��: %I64d", nPoolMoney);
							OutputDebugString(str);
							
							bIsNewPool = true;
						}
					}

					if (!bIsNewPool)
					{
						// û���±߳ز���, ������
						break;
					}

					// �߳�λ���Ƶ���һ���߳�
					nBetPool++;
				}

				m_GameData.m_nBetPool = nBetPool;

				TBetPool tagTbetPool;

				// �����³���Ϣ���ͻ���
				for (int i = 0; i <= m_GameData.m_nBetPool; i++)
				{
					// ��8����ע�صĽ�ҷ����ͻ���
					tagTbetPool.iBetPools[i] = m_GameData.GetBetMoney(i, false);
					CString str;
					str.Format("Server: ��%d���߳ؽ��: %I64d", i, tagTbetPool.iBetPools[i]);
					OutputDebugString(str);
				}
				
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					// ����
					SendGameData(i, &tagTbetPool, sizeof(tagTbetPool), MDM_GM_GAME_NOTIFY, ASS_BETPOOL_UP, 0);
				}

				SendWatchData(m_bMaxPeople,&tagTbetPool, sizeof(tagTbetPool), MDM_GM_GAME_NOTIFY, ASS_BETPOOL_UP, 0);
			}

			OutputDebugString("Server: ������һ�ַ��Ʋ���");

			int nCount = 0;
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || m_GameData.m_bNoBet[i]/* || i == bDeskStation*/)
				{
					continue;
				}

				nCount++;
			}

			if (nCount <= 1)
			{
				// ȫ��֮��û�˿�����ע�ˣ���Ҫ��ʣ�����ȫ����
				OutputDebugString("Server: ȫ��֮��û�˿�����ע�ˣ���Ҫ��ʣ�����ȫ����");
				m_GameData.m_bAutoSendCard = true;
			}

			// û�м�ע, ��һ�ֿ�ʼ
			if (m_GameData.m_nTurnNums == 1)
			{
				// �����Ź�����
				ActionRedirect(IDEV_SEND_3_CARD, 500);
			}
			else if (m_GameData.m_nTurnNums < 4)
			{
				// ��һ�Ź�����
				ActionRedirect(IDEV_SEND_4_5_CARD, 500);
			}
			else 
			{
				// ��Ϸ����, ���н������
				OutputDebugString("Server:  ��Ϸ����, ���н������");
				ActionRedirect(IDEV_COMPARE_CARD, 500);
				return true;
			}

			// ������עλ��
			m_GameData.m_bRaiseStation = -1;
			m_GameData.m_bBetStation = 255;
		}
		else
		{
			OutputDebugString("Server: ��һ����Ҳ���");

			int nCount = 0;
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || m_GameData.m_bNoBet[i] || i == bDeskStation)
				{
					continue;
				}

				nCount++;
			}

			if (nCount <= 0)
			{
				OutputDebugString("Server:  ��һ����Ҳ�����, ������ҷ�����ȫ��(û�пɲ��������), ��Ϸ����");
				m_GameData.m_bAutoSendCard = true;

				// û�м�ע, ��һ�ֿ�ʼ
				if (m_GameData.m_nTurnNums == 1)
				{
					// �����Ź�����
					ActionRedirect(IDEV_SEND_3_CARD, 500);
				}
				else if (m_GameData.m_nTurnNums < 4)
				{
					// ��һ�Ź�����
					ActionRedirect(IDEV_SEND_4_5_CARD, 500);
				}
				else 
				{
					// ��Ϸ����, ���н������
					OutputDebugString("Server:  ��Ϸ����, ���н������");
					ActionRedirect(IDEV_COMPARE_CARD, 500);
				}

				return true;
			}

			bool temps = true;
			if (m_GameData.m_nTurnNums == 1)
			{
				OutputDebugString("Server:m_GameData.m_nTurnNums == 1");
				if (m_GameData.m_iBetCounts > 2)
				{
					temps = true;
				}
				else if (m_GameData.m_bBigBlind == bNextStation)
				{
					temps = false;
				}
			}
			else
			{
				if (m_GameData.m_iBetCounts > 1)
				{
					temps = true;
				}
				else if (m_GameData.m_bBigBlind == bNextStation)
				{
					temps = false;
				}
			}

			// ��һ��֮��ÿ�ֿ�ʼ���Ը�ע����ע�����ơ����ơ�ȫ��
			if (/*m_GameData.m_iBetCounts > 0*/temps)
			{
				//OutputDebugString("Server:��һ��֮��ÿ�ֿ�ʼ���Ը�ע����ע�����ơ����ơ�ȫ��");
				// ����ע����, ���ע��ť����ע��ť�����ư�ť��ȫ�°�ť����ʾ
				m_GameData.MakeVerbs(UD_VF_CALL|UD_VF_ADD|UD_VF_FOLD/*|UD_VF_ALLIN*/);
			}
			else
			{
				//OutputDebugString("Server:��һȦ��äע����ʱ, ���Ƭ��ť����ע��ť�����ư�ť����ʾ");
				// ��һȦ��äע����ʱ, ���Ƭ��ť����ע��ť�����ư�ť����ʾ
				m_GameData.MakeVerbs(UD_VF_CHECK|UD_VF_ADD|UD_VF_FOLD);
				// ��û������ע: ��ע��ť�����ư�ť�����ư�ť��ȫ�°�ť����ʾ
				// m_GameData.MakeVerbs(UD_VF_BET|UD_VF_FOLD|UD_VF_CHECK|UD_VF_ALLIN);
			}

			// ������һ�������ע
			ActionRedirect(IDEV_BET_NEXT, 500);
		}
	}

	return true;
}


//������ҷ������
bool CServerGameDesk::OnUserSendCardFinish(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser)
{
	// ����Ҫ�ͻ���ͬ��
	//return true;

	////Ϊ��bDeskStation������÷����Ƶı��
	m_GameData.m_UserData[bDeskStation].m_bSendCardFinish = true;

	////����������δ�����ƣ����ܼ�������
	if (!m_GameData.IsAllPlayerSendCardFinish())
	{
		return true;
	}

	////������ɿ�ʼ�״���ע�¼�����
	OutputDebugString("Server: ������ҷ������");
	
	if (m_bFaiPaiFinish == false)
	{
		ActionRedirect(IDEV_BET_START,500);
	}
	m_bFaiPaiFinish = true;

	return true;
}

//������Ҷ���һ�غ�׼�����ź�
bool CServerGameDesk::OnUserNextRoundReq(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	TNextRoundReq* pNextRoundReq = (TNextRoundReq*)pData;

	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}

	TAgree tagAgree;
	tagAgree.byUser = bDeskStation;

	if (pNextRoundReq->nType == TNextRoundReq::READY)
	{
		//m_emUserAgree[bDeskStation] = US_AGREE;
		tagAgree.bAgree = true;
	}
	else if (pNextRoundReq->nType == TNextRoundReq::EXIT)
	{
		//m_emUserAgree[bDeskStation] = US_DISAGREE;
		tagAgree.bAgree = false;
	}
	else
	{
		return true;
	}

	//�㲥������Ƿ�ͬ�������Ϸ����Ϣ
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i,&tagAgree,sizeof(TAgree),MDM_GM_GAME_NOTIFY,ASS_AGREE,0);
		}
	}

	SendWatchData(m_bMaxPeople,&tagAgree,sizeof(TAgree),MDM_GM_GAME_NOTIFY,ASS_AGREE,0);

	//ֻҪ��һ�����˳���Ϸ�����޷�������Ϸ��֪ͨ���������ϵ����������˳�����������Ϸ

	if (pNextRoundReq->nType == TNextRoundReq::EXIT)
	{
		TNoPlayer tagNoPlayer;
		tagNoPlayer.bGameFinished = false;
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			if (m_pUserInfo[i])
			{
				SendGameData(i,&tagNoPlayer,sizeof(TNoPlayer),MDM_GM_GAME_NOTIFY,ASS_NO_PLAYER,0);
			}
		}

		SendWatchData(m_bMaxPeople,&tagNoPlayer,sizeof(TNoPlayer),MDM_GM_GAME_NOTIFY,ASS_NO_PLAYER,0);

		GameFinish(bDeskStation,GF_SALE);
		//__super::GameFinish(0,GF_SALE);//���ʹ��GameFinish�󣬿ͻ��޷��������˽ӿڣ�������

		return true;
	}

	if (GetPlayerCount() >= 2)
	{
		//�ͷŵ�ǰ�������е��¼�
		CloseEventProtection();

		//��ʼ��һ�غ���Ϸ�¼�����
		//ActionRedirect(IDEV_NEXT_ROUND_BEGIN,500);
		GameBegin(ALL_ARGEE);
	}

	return true;
}

//������ұ����Ҳ��������ٴ�����
bool CServerGameDesk::OnUserMoneyLack(BYTE bDeskStation)
{
	TMoneyLack  cmd;
	cmd.nbyUser = bDeskStation;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		SendGameData(i,&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_NOT_ENOUGH_MONEY, 0);
	}

	SendWatchData(m_bMaxPeople,&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_NOT_ENOUGH_MONEY, 0);
	
	if (m_pUserInfo[bDeskStation] != NULL)
	{
		// �߳��������
		//KickUserByID(m_pUserInfo[bDeskStation]->m_UserData.dwUserID);
	}

	// ��������λ�ϵ���Ϣ��������һ������ٴ�ʹ��
	m_GameData.m_UserMoney[bDeskStation] = 0;
	m_GameData.m_UserData[bDeskStation].m_bIn = false;

	return true;
}


//�����������������Ϸ������
bool CServerGameDesk::OnTestFinishRoundRequest(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser)
{
	// 	if (m_bGameStation != GS_PLAY_GAME)
	// 	{
	// 		return true;
	// 	}

	//�ͷŵ�ǰ�������е��¼�
	CloseEventProtection();

	//�����غϽ�������
	ActionRedirect(IDEV_ROUND_FINISH,500);

	return true;
}

//������ҷ������
bool CServerGameDesk::OnAgentSendCardsFinish()
{
	OutputDebugString("Server:������ҷ������");
	//�鿴��û����ҵ���
	if (m_bFaiPaiFinish == false)	
	{
		for (int i = 0; i < PLAY_COUNT; ++i)
		{
			//if (m_GameData.m_UserData[i].m_bNetCut)

			{
				OnUserSendCardFinish(i,NULL,NULL,0,0,false);
			}
		}
	}
	
	return true;
}

//���������¼���
bool CServerGameDesk::OnAgentOutCard()
{
	return true;
}

//��һ�ֿ�ʼ�¼�������
bool CServerGameDesk::OnAgentNextRoundBegin()
{
	//�鿴��û����ҵ���
	BYTE byNetCutUser = 255;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_GameData.m_UserData[i].m_bNetCut)
		{
			byNetCutUser = i;
			break;
		}
	}

	if (byNetCutUser != 255)
	{
		//���ߵ���������˳�
		TNextRoundReq NextRoundReq;
		NextRoundReq.nType = TNextRoundReq::EXIT;

		OnUserNextRoundReq(byNetCutUser,NULL,&NextRoundReq,sizeof(TNextRoundReq),0,false);
	}

	return true;
}

//����Ҫ�õ���Ϸ�ȴ���ʼ������
bool CServerGameDesk::OnGetGSWaitAgree(BYTE bDeskStation,UINT uSocketID,bool bWatchUser,TGSBase* pGSBase)
{
	TGSWaitAgree GSWaitAgree;
	CopyGSBase(dynamic_cast<TGSWaitAgree*>(&GSWaitAgree),pGSBase);

	//��������ݻ�ȡ����д�´�
    memcpy(GSWaitAgree.nSubMoney, m_GameData.m_UserMoney, sizeof(m_GameData.m_UserMoney));

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			GSWaitAgree.bReady[i] = m_GameData.m_UserData[i].m_bIn;
		}
	}

	return SendGameStation(bDeskStation,uSocketID,bWatchUser,&GSWaitAgree,sizeof(TGSWaitAgree));
}

//����Ҫ�õ���Ϸ��״̬������
bool CServerGameDesk::OnGetGSPlaying(BYTE bDeskStation,UINT uSocketID,bool bWatchUser,TGSBase* pGSBase)
{
	TGSPlaying GSPlaying;
	CopyGSBase(dynamic_cast<TGSPlaying*>(&GSPlaying),pGSBase);

	//��������ݻ�ȡ����д�´�

	// ��õõ����Ƶ����
	GSPlaying.byTokenUser = m_GameData.m_byTokenUser;

    // ��ǰ���ý��
    memcpy(GSPlaying.nSubMoney, m_GameData.m_UserMoney, sizeof(m_GameData.m_UserMoney));

    // ׯ��
    GSPlaying.byNTUser = m_GameData.m_byNTUser;

    // ��С����
    GSPlaying.bySmallBlind = m_GameData.m_bSmallBlind;
    GSPlaying.byBigBlind = m_GameData.m_bBigBlind;

    GSPlaying.byVerbFlag = m_GameData.m_byVerbFlag;
    GSPlaying.bNewTurn = m_GameData.m_bNewTurn;
	GSPlaying.bNewTurns = m_GameData.m_bNewTurns;

	// ��ע���
	GSPlaying.nCallMoney = m_GameData.m_nMaxBetMoney - m_GameData.GetBetMoney(m_GameData.m_byTokenUser);

    // ��ȡ������ϵ��˿�
    GSPlaying.nHandCardNums = m_GameData.m_UserData[bDeskStation].m_iCardsNum;
    memcpy(GSPlaying.byHandCard, m_GameData.m_UserData[bDeskStation].m_byMyCards,  GSPlaying.nHandCardNums * sizeof(BYTE));

    // ��ȡ������
    GSPlaying.nCardsCount = m_GameData.m_iCardsCount;
    memcpy(GSPlaying.byCards, m_GameData.m_iCards, GSPlaying.nCardsCount * sizeof(BYTE));

    // ��ȡ��ע���
    memcpy(GSPlaying.nBetMoney, m_GameData.m_nBetMoney, sizeof(m_GameData.m_nBetMoney));

	/*CString strMoney;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
		for (int j = 0; j < PLAY_COUNT; j++)
		{
			strMoney.Format("Server:���%d,��ȡ��ע���:%I64d",GSPlaying.nBetMoney[i][j]);
			OutputDebugString(strMoney);
		}
	}*/

    // ��ȡ������ע���
    memcpy(GSPlaying.bNoBet, m_GameData.m_bNoBet, sizeof(m_GameData.m_bNoBet));

    // ��ȡ�������
    memcpy(GSPlaying.bGiveUp, m_GameData.m_bGiveUp, sizeof(m_GameData.m_bGiveUp));

	CString str;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		str.Format("giwo:��ȡ������ң�%d",m_GameData.m_bGiveUp[i]);
		OutputDebugString(str);
	}

	// ��;�������
	GSPlaying.iUserStation = m_iUserStation[bDeskStation] ; 
		
    // ���б߳�����
    for (int i = 0; i <= m_GameData.m_nBetPool; i++)
    {
        GSPlaying.nBetPools[i] = m_GameData.GetBetMoney(i, false);
    }

	return SendGameStation(bDeskStation,uSocketID,bWatchUser,&GSPlaying,sizeof(TGSPlaying));
}

//����Ҫ�õ���Ϸ�غϽ���������
bool CServerGameDesk::OnGetGSWaitNextRound(BYTE bDeskStation,UINT uSocketID,bool bWatchUser,TGSBase* pGSBase)
{
	TGSWaitNextRound GSWaitNextRound;
	//CopyGSBase(dynamic_cast<TGSWaitNextRound*>(&GSWaitNextRound),pGSBase);

	//��������ݻ�ȡ����д�´�
    //memcpy(GSWaitNextRound.nSubMoney, m_GameData.m_UserMoney, sizeof(m_GameData.m_UserMoney));

	return SendGameStation(bDeskStation,uSocketID,bWatchUser,&GSWaitNextRound,sizeof(TGSWaitNextRound));
}

//˳ʱ��˳��õ��¼�λ��
BYTE CServerGameDesk::GetNextUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iNextStation = (byStation + PLAY_COUNT + 1) % PLAY_COUNT;

	// �ƹ���Ч������ѷ�������� �� �Ѿ�ȫ�µ���� �� ��;������Ϸ���
	while (m_pUserInfo[iNextStation] == NULL 
	|| m_GameData.m_bGiveUp[iNextStation] 
	|| (bIsAll && m_GameData.m_bNoBet[iNextStation])
	|| m_iUserStation[iNextStation] != TYPE_PLAY_GAME)
	{
		iNextStation = (iNextStation + PLAY_COUNT + 1) % PLAY_COUNT;
		nCount++;

		if (nCount >= PLAY_COUNT)
		{
			iNextStation = -1;
			break;
		}
	}

	return iNextStation;
}

//��ʱ��˳��õ��ϼ�λ��
BYTE CServerGameDesk::GetLastUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iLastStation = (byStation + PLAY_COUNT - 1) % PLAY_COUNT;

	// �ƹ���Ч������ѷ�������� �� �Ѿ�ȫ�µ���� �� ��;������Ϸ���
	while (m_pUserInfo[iLastStation] == NULL
	|| m_GameData.m_bGiveUp[iLastStation]
	|| (bIsAll && m_GameData.m_bNoBet[iLastStation])
	|| m_iUserStation[iLastStation] != TYPE_PLAY_GAME)
	{
		iLastStation = (iLastStation + PLAY_COUNT - 1) % PLAY_COUNT;

		nCount++;

		if (nCount >= PLAY_COUNT)
		{
			iLastStation = -1;
			break;
		}
	}

	return iLastStation;
}

///���ƣ�GetPlayerCount
///��������ȡ��Ϸ�������
///@return 
int CServerGameDesk::GetPlayerCount(bool bFold)
{
	int nCount = 0;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (bFold)
		{
			if (m_pUserInfo[i] != NULL && m_iUserStation[i] == TYPE_PLAY_GAME)
			{
				nCount++;
			}
		}
		else
		{
			if (m_pUserInfo[i] != NULL)
			{
				nCount++;
			}
		}
		
	}

	return nCount;
}

// �������ע��ʤ�����
void CServerGameDesk::CalculateWin(BYTE nSectionCount, CByteArray &arrayResult)
{
	arrayResult.RemoveAll();

	// �ҳ�����������
	// ����һ�����õ���ҿ�ʼ����
	BYTE bMaxStation = -1;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || m_GameData.m_nBetMoney[i][nSectionCount] <= 0)
		{
			continue ;
		}

		bMaxStation = i;
		break;
	}

	CString str;
	str.Format("Server: ��%d����ע��, ��һ���������: %d", nSectionCount, bMaxStation);
	OutputDebugString(str);

	if (bMaxStation == 255)
	{
		return ;
	}

	// ĿǰΪӮ��
	arrayResult.Add(bMaxStation);

	// �ҳ����ƺ�ʤ�����
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] == NULL || m_GameData.m_bGiveUp[i] || m_GameData.m_nBetMoney[i][nSectionCount] <= 0 || i == bMaxStation)
		{
			continue ;
		}

		int nRet = m_Logic.CompareCard(m_GameData.m_bResultCards[i], 5, m_GameData.m_bResultCards[bMaxStation], 5);

		if (nRet > 0)
		{
			arrayResult.RemoveAll();

			// Ӯ
			bMaxStation = i;
	
			arrayResult.Add(i);
		}
		else if (nRet == 0)
		{
			// ���
			arrayResult.Add(i);
		}
	}
}

// �ж�����Ƿ�������������
bool CServerGameDesk::CanGetGoodCard(BYTE byStation)
{
	if(!m_pUserInfo[byStation])
	{
		return false;
	}

	long int UserID = m_pUserInfo[byStation]->m_UserData.dwUserID;

	TCHAR szIniName[MAX_PATH];
	GET_SERVER_INI_NAME(szIniName);

	CINIFile f(szIniName);

	CString key = TEXT("superUserConfig");

	int	iSuperUserCount = 0;
	iSuperUserCount = f.GetKeyVal(key,"SuperUserCount",0);	//�����û�����

	if(iSuperUserCount <= 0)
	{
		return false;
	}

	long int iTempUserID = -1;
	CString sTemp = "";
	for(int i = 1; i <= iSuperUserCount; ++i)
	{
		sTemp.Format("SuperUserID%d",i);
		iTempUserID = f.GetKeyVal(key,sTemp,0);		//��ȡ�����û�ID

		//�������ļ��е�ID��� ��Ϊ�����û�
		if(UserID == iTempUserID)
		{
			return true;
		}
	}

	return false;
	
}

//�Զ�������Ϸ�¼�����
bool CServerGameDesk::OnEventAutoOperate()
{
    // �Զ���������
    TBet tagBet;
    tagBet.nType = ET_FOLD;
    tagBet.byUser = m_GameData.m_byTokenUser;
    tagBet.nMoney = 0;

    // ģ����ע��Ϣ
    OnUserBet(m_GameData.m_byTokenUser, &tagBet);

    return true;
}