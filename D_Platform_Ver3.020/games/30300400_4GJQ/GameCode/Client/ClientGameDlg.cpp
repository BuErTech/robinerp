#include "StdAfx.h"
#include "ClientGameDlg.h"
#include "GameSet.h"
#include ".\clientgamedlg.h"
#include "Platform\bzsound.h"

void Msg(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage[1024] = { 0 };
#ifdef WIN32
	_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
	vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage);
}

///�ж��Ƿ�Ϊ����״̬
///@param a ����ֵ
#define IsStation(a); \
if (GetStationParameter()==GS_WAIT_SETGAME || GetStationParameter()==GS_WAIT_ARGEE || GetStationParameter()==GS_WAIT_NEXT) \
{ \
	return a; \
}

/**
 * ������Ϸ״̬
 */
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	Msg("Test:Data ��ǰ״̬ %d  __id %d", GetStationParameter(),GetMeUserInfo()->bDeskStation); 
//	g_bIsWatcher = m_bWatchMode;
	if(m_bWatchMode)
	{
		m_PlayView.m_btContinue.ShowWindow(false);
		//m_PlayView.m_btleave.ShowWindow(false);
		m_PlayView.m_Logic.m_bWatchOther=m_bWatchOther;
		m_PlayView.m_btFinishDiaoDu.ShowWindow(false);
		m_PlayView.m_btLoadBuZhen.ShowWindow(false);
		m_PlayView.m_btSaveBuZhen.ShowWindow(false);
	}
	else
	{
		m_PlayView.m_Logic.m_bWatchOther=false;
	}

	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
		{
			if(uDataSize!=sizeof(GameStation_1))
			{
				MessageBox("����Ҫ�����µĿͻ���","ϵͳ��ʾ",0);
				OnCancel();
				return true;
			}
			GameStation_1 * pGameStation=(GameStation_1 *)pStationData;
			if(pGameStation->iMainVersion!=MAIN_VERSION||pGameStation->iLessVersion!=ASS_VERSION)
			{
				MessageBox("����Ҫ�����µĿͻ���","ϵͳ��ʾ",0);
				OnCancel();
				return true;
			}
			uRunPublish = pGameStation->uRunPublish;
			basepoint	= pGameStation->basepoint;
			
			if (!IsQueueGameRoom()) 
			{
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(true);
					//m_PlayView.m_btleave.ShowWindow(true);
				}
			}
			else
			{
				m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
				KillAllTimer();
				m_PlayView.m_btFinishDiaoDu.ShowWindow (SW_SHOW);
				m_PlayView.m_btLoadBuZhen.ShowWindow (SW_SHOW);
				m_PlayView.m_btSaveBuZhen.ShowWindow (SW_SHOW);

				m_PlayView.m_btFinishDiaoDu.EnableWindow (true);
				m_PlayView.m_btLoadBuZhen.EnableWindow (true);
				m_PlayView.m_btSaveBuZhen.EnableWindow (true);

				m_PlayView.m_Logic.StartBuzhen();
				ResetGameStation(RS_GAME_BEGIN);
			}
			return TRUE;
		}
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{
			if(uDataSize!=sizeof(GameStation_2))
			{
				MessageBox("����Ҫ�����µĿͻ���","ϵͳ��ʾ",0);
				OnCancel();
				return true;
			}
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			if(pGameStation->iMainVersion!=MAIN_VERSION||pGameStation->iLessVersion!=ASS_VERSION)
			{
				MessageBox("����Ҫ�����µĿͻ���","ϵͳ��ʾ",0);
				OnCancel();
				return true;
			}
			
			uRunPublish = pGameStation->uRunPublish;
			basepoint	= pGameStation->basepoint;
			ResetGameStation(RS_GAME_BEGIN);
			m_PlayView.m_Logic.Init();
			m_iLessCount=pGameStation->iPlayCount;
			
			for(int i=0;i<MAX_I;i++)
			{
				for(int j=0;j<MAX_J;j++)
				{
					int ii=-1,jj=-1;
					if(m_pGameInfo->uDeskPeople == 4)
						m_PlayView.m_Logic.ToMyCord (2,ii,jj,i,j);
					else
						m_PlayView.m_Logic.ToMyCord (0,ii,jj,i,j);
					if(ii < 0 || jj < 0)continue;
					m_PlayView.m_Logic.mBoard[ii][jj] = pGameStation->mBoard[i][j];
				}
			}

			if (!IsQueueGameRoom()) 
			{
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(true);
					SetGameTimer(GetMeUserInfo()->bDeskStation,20,ID_BEGIN_TIME);
					//m_PlayView.m_btleave.ShowWindow(true);
				}
			}
			else
			{
				KillAllTimer();
				m_PlayView.m_btFinishDiaoDu.ShowWindow (SW_SHOW);
				m_PlayView.m_btLoadBuZhen.ShowWindow (SW_SHOW);
				m_PlayView.m_btSaveBuZhen.ShowWindow (SW_SHOW);

				m_PlayView.m_btFinishDiaoDu.EnableWindow (true);
				m_PlayView.m_btLoadBuZhen.EnableWindow (true);
				m_PlayView.m_btSaveBuZhen.EnableWindow (true);

				m_PlayView.m_Logic.StartBuzhen();
			}


			m_PlayView.UpdateViewFace(NULL);
			/*

			CString strResult = "this is the result";//5.18 2009
			GameResult gameResult;
			for (int i=0; i<PLAY_COUNT; i++)
			{
			gameResult.iMoney[i] = 5;
			gameResult.iWardPoint[i] = 5;
			strcpy(gameResult.name[i], "nickname");
			}
			m_PlayView.m_Result.ShowMsg(TEXT(""),20,true,gameResult, Glb().m_nPowerOfGold);
			//m_PlayView.m_msgbox.ShowMsg(TEXT("\n\n\n\n\n���Ѹ���!"),10, false, gameResult);

			*/
			return TRUE;
		}
	
	case GS_BUZHEN:	//����״̬
	case GS_PLAY_GAME:	//��Ϸ������
		{
			//�����Ƿ����½�����
			m_PlayView.m_bFirstGame = false;

			int st=0;
			if(m_pGameInfo->uDeskPeople == 4)
				st=2;
			m_PlayView.m_Logic.Init();
			m_PlayView.m_Logic.m_iPlayerCount=GetPlayerCount();
			m_PlayView.UpdateViewFace(NULL);
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;

			m_iLessCount=pGameStation->iPlayCount;
			uRunPublish = pGameStation->uRunPublish;
			basepoint	= pGameStation->basepoint;
			m_PlayView.m_btThing.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btStop.EnableWindow(!m_bWatchMode);

			m_PlayView.m_Logic.m_iNowOutPeople = pGameStation->m_iNowOutPeople;	

			m_PlayView.m_Logic.ToMyCord (st,
				m_PlayView.m_Logic.m_iStartI,m_PlayView.m_Logic.m_iStartJ,
				 pGameStation->m_iStartI,pGameStation->m_iStartJ
				);

			m_PlayView.m_Logic.ToMyCord (st,
				m_PlayView.m_Logic.m_iEndI,m_PlayView.m_Logic.m_iEndJ,
				 pGameStation->m_iEndI,pGameStation->m_iEndJ
				);

			//m_PlayView.m_Logic.m_iStartI = pGameStation->m_iStartI;
			//m_PlayView.m_Logic.m_iStartJ = pGameStation->m_iStartJ;
			//m_PlayView.m_Logic.m_iEndI = pGameStation->m_iEndI;
			//m_PlayView.m_Logic.m_iEndJ = pGameStation->m_iEndJ;

			for(int i=0;i<MAX_I;i++)
		      for(int j=0;j<MAX_J;j++)
			  {
				  int ii=-1,jj=-1;
				  m_PlayView.m_Logic.ToMyCord (st,ii,jj,i,j);
				  if(ii < 0 || jj < 0)continue;
	              m_PlayView.m_Logic.mBoard[ii][jj] = pGameStation->mBoard[i][j];
			  }
			  if(m_PlayView.m_Logic.m_iNowOutPeople==GetMeUserInfo()->bDeskStation)
				  m_PlayView.m_Logic.m_bICanMove=true;
			if (m_PlayView.m_Logic.m_iNowOutPeople!=-1) 
			{
				SetGameTimer(m_PlayView.m_Logic.m_iNowOutPeople,pGameStation->iTimerCount - 3,ID_OUT_CARD);
				//SetGameTimer(m_PlayView.m_Logic.m_iNowOutPeople,m_iLessCount,ID_OUT_CARD);

			}
				

			m_PlayView.UpdateViewFace(NULL);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			//�����Ƿ����½�����
			m_PlayView.m_bFirstGame = false;

			//g_bIsWatcher=m_bWatchMode;
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;

			//��������
			m_iLessCount=pGameStation->iPlayCount;
			uRunPublish = pGameStation->uRunPublish;
			basepoint	= pGameStation->basepoint;
			if (!IsQueueGameRoom()) 
			{
				//��ʾ��ʼ��ť 
				if (GetMeUserInfo()->bUserState!=USER_ARGEE)
				{
					if (m_bWatchMode==false)
					{
						m_PlayView.m_btContinue.ShowWindow(SW_SHOW);
						//m_PlayView.m_btleave.ShowWindow(true);
						m_PlayView.m_btContinue.SetFocus();
					}
					SetGameTimer(GetMeUserInfo()->bDeskStation,20,ID_BEGIN_TIME);
				}
			}
			else
			{
				KillAllTimer();
				m_PlayView.m_btFinishDiaoDu.ShowWindow (SW_SHOW);
				m_PlayView.m_btLoadBuZhen.ShowWindow (SW_SHOW);
				m_PlayView.m_btSaveBuZhen.ShowWindow (SW_SHOW);

				m_PlayView.m_btFinishDiaoDu.EnableWindow (true);
				m_PlayView.m_btLoadBuZhen.EnableWindow (true);
				m_PlayView.m_btSaveBuZhen.EnableWindow (true);

				m_PlayView.m_Logic.StartBuzhen();
			}

			
			return TRUE;
		}
	}


	return false;
}

/**
 * ��Ϸ����
 */
bool CClientGameDlg::H081_UserContinueEnd(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	if (uDataSize!=sizeof(GameEndStruct)) 
		return FALSE;

	IsStation(true);
	GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
	KillGameTimer(0);
	CString stime;//*shingo*
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);

    GameResult  gameResult;
	if(pGameEnd->iTurePoint[0]==0)
	{
		if(m_PlayView.m_bRightPower)
		{
			TCHAR path[MAX_PATH] = {0};
			wsprintf(path,TEXT("sound\\he.mp3"));
			m_playsound2.PlayMusic(m_hWnd,path,1);
		}
	}

	//�����������ʾ�ɼ�
	CString str="\n";
	if(GetPlayerCount()==2)
		str+="\n";

	str+="������Ϸ�����\n";
	str+="������������- - - - - -";
	m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));

	str = "";
	BYTE iDoublePointEffect = 0;
	__int64 nMoneyTemp = 1;
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nMoneyTemp*=10;
	}

	
	CString s = "..\\";// CBcfFile::GetAppPath ();/////����·��	

#ifdef	VIDEO
	CBcfFile f( "30300405.bcf");
#else
	CBcfFile f( "30300400.bcf");
#endif	

	CString key=TEXT("config");	

	CString sec = _T("");

	CString strtemp;
	
	for (int i=0;i<4&&i<m_pGameInfo->uDeskPeople;i++)
	{		
		if (m_pUserInfo[i]!=NULL)
		{

			//if(m_pUserInfo[i]->GameUserInfo.iDoublePointTime>curtime && pGameEnd->iTurePoint[i]>0)
			//20081125 ˫�����ֿ�Ч��
			iDoublePointEffect = (1<<i);
			if((iDoublePointEffect & pGameEnd->iDoublePointEffect)
				&& pGameEnd->iTurePoint[i] >0)
			{
				pGameEnd->iTurePoint[i]*=2;
				sec.Format("ResultTipDouble%d", m_pGameInfo->uComType);
				str.Format(f.GetKeyVal(key, sec, ""),
					m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]);

				if (m_pGameInfo->uComType != 1)
				{
					strtemp.Format("%I64d",nMoneyTemp*pGameEnd->iChangeMoney[i]);
					str+=strtemp;
				}				

//#ifdef TWENTY
//				str.Format("%s%s �÷� %d��˫�����ֿ�Ч���� ͭ��%I64d\n",str.GetBuffer(), m_pUserInfo[i]->GameUserInfo.nickName, \
//					pGameEnd->iTurePoint[i], nMoneyTemp*pGameEnd->iChangeMoney[i]);
//#else
//				if(GetComType()!=TY_MONEY_GAME)
//					str.Format("%s%s �÷� %d��˫�����ֿ�Ч����\n",str.GetBuffer(),m_pUserInfo[i]->GameUserInfo.nickName, \
//					pGameEnd->iTurePoint[i]);
//				else
//					str.Format("%s%s �÷� %d��˫�����ֿ�Ч���� ���%I64d\n",str.GetBuffer(), m_pUserInfo[i]->GameUserInfo.nickName, \
//					pGameEnd->iTurePoint[i], nMoneyTemp*pGameEnd->iChangeMoney[i]);
//#endif
			}
			else
			{				
				sec.Format("ResultTip%d", m_pGameInfo->uComType);
				str.Format(f.GetKeyVal(key, sec, ""),
					m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]);

				if (m_pGameInfo->uComType != 1)
				{
					strtemp.Format("%I64d",nMoneyTemp*pGameEnd->iChangeMoney[i]);
					str+=strtemp;
				}			

//#ifdef TWENTY
//				str.Format("%s%s  �÷� %d  ͭ��%I64d\n",str.GetBuffer(),m_pUserInfo[i]->GameUserInfo.nickName, \
//					pGameEnd->iTurePoint[i], nMoneyTemp*pGameEnd->iChangeMoney[i]);
//#else
//				if(GetComType()!=TY_MONEY_GAME)
//				{
//					str.Format("%s%s  �÷� %d\n",str.GetBuffer(), m_pUserInfo[i]->GameUserInfo.nickName, pGameEnd->iTurePoint[i]);
//				}
//				else
//				{
//					str.Format("%s%s  �÷� %d  ���%I64d\n",str.GetBuffer(),m_pUserInfo[i]->GameUserInfo.nickName, \
//						pGameEnd->iTurePoint[i], nMoneyTemp*pGameEnd->iChangeMoney[i]);
//				}
//#endif
			}
	
			m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
			//���ý�����ʾ��Ϣ
			_stprintf_s(gameResult.name[i],sizeof(gameResult.name[i]), _T("%s"),m_pUserInfo[i]->GameUserInfo.nickName);
			gameResult.iMoney[i]=pGameEnd->iChangeMoney[i];
			gameResult.iWardPoint[i]=pGameEnd->iTurePoint[i];

			m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i),pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
			
		}
	}	
	m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));//��Ϸ���
	str="������������- - - - - -\n";
	m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));//��Ϸ���

	m_PlayView.m_Result.ShowMsg(TEXT(""), 20, /*true*/false, gameResult, Glb().m_nPowerOfGold);

	/*
	m_MessageHandle.InsertNormalMessage("������Ϸ�����");
	m_MessageHandle.InsertNormalMessage("������������");
	CString str;
	for (int i=0;i<4&&i<m_pGameInfo->uDeskPeople;i++)
	{
	if (m_pUserInfo[i]!=NULL)
	{
	if(GetComType()!=TY_MONEY_GAME)
		str.Format("%s �÷� %d",m_pUserInfo[i]->GameUserInfo.szName,pGameEnd->iTurePoint[i]);
	else
		str.Format("%s �÷� %d ��� %d",m_pUserInfo[i]->GameUserInfo.szName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
	m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
	}
	}
	m_MessageHandle.InsertNormalMessage("������������");
	*/
	if (m_bWatchMode==FALSE)
	{
		m_PlayView.m_btContinue.ShowWindow(SW_SHOW);
		//m_PlayView.m_btleave.ShowWindow(true);
		m_PlayView.m_btContinue.SetFocus();
	}
	m_PlayView.m_btThing.EnableWindow(SW_HIDE);			///< ����Ҫ��
	m_PlayView.m_btStop.EnableWindow(SW_HIDE);				///< ֹͣ����
	SetGameTimer(GetMeUserInfo()->bDeskStation,20,ID_BEGIN_TIME);

	//��������
	if(m_pGameInfo->bEnableSound)
		PlaySound(TEXT("GAME_OVER"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
	SetStationParameter(GS_WAIT_NEXT);
	ResetGameStation(RS_GAME_END);
	return TRUE;

}

/**
 * ��ʱ����Ϣ
 * @param bDeskStation ���ID
 * @param uTimeID ʱ��ID
 * @param uTimeCount ��ʱ��ֵ
 */
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount<=1)
			{
				//ʱ�䵽��
				if (m_iAfterWaitAction == 1)
				{
					OnHitBegin(0,0);
				}
				else
				{
					SendMessage(WM_CLOSE,0,0);
				}
				
				//SetStationParameter(GS_BUZHEN);
				return true;
			}
			if (uTimeCount<=10) 
			{
				if(m_pGameInfo->bEnableSound)
				{
					//PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
					TCHAR path[MAX_PATH] = {0};
					wsprintf(path,TEXT("sound\\timeout.mp3"));
					m_playsound2.PlayMusic(m_hWnd,path);
				}
			}

			return true;
		}
	case ID_OUT_CARD:	//����
		{
			int iViewStation=ViewStation(bDeskStation);
			if (uTimeCount<=1)	//ʱ�䵽��
			{
				//�жϳ�ʱ
				if((m_PlayView.m_Logic.m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
					m_bTimeOutCount++;
				if (m_bTimeOutCount>=5 && (m_PlayView.m_Logic.m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
				{
					KillGameTimer(0);
					SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
					SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
					PostMessage(WM_CLOSE,0,0);
					AFCMessageBox(TEXT("��ʱ����̫�࣬��Ϸ������"),m_pGameInfo->szGameName,AFC_CONFIRM); //,this);
					return false;
				}
				else if(m_PlayView.m_Logic.m_iNowOutPeople==GetMeUserInfo()->bDeskStation && !m_bWatchMode)
				{
					CString str;
					str.Format("��ע�⣬���Ѿ���ʱ %d ���ˣ���ʱ��λᵱ�����ܴ���",m_bTimeOutCount);
					m_MessageHandle.InsertNormalMessage(str.GetBuffer());
					//
					m_PlayView.m_btStop.EnableWindow(FALSE);//5.18 2009
					m_PlayView.m_btThing.EnableWindow(FALSE);
					m_PlayView.m_Logic.SetStartEndXY();
				}

				if ((m_PlayView.m_Logic.m_iNowOutPeople==GetMeUserInfo()->bDeskStation)&&(m_bWatchMode==FALSE))
				{
					tagNextStep m;
		             m.station=m_PlayView.m_Logic.m_iNowOutPeople;
					 SendGameData(&m,sizeof(m),MDM_GM_GAME_NOTIFY,ASS_PASS,0);
					//m_PlayView.m_Logic.AutoOutCard();
				}
				return true;
			}
			if ((uTimeCount<=6)&&(m_bWatchMode==FALSE)&&(iViewStation==2)) 
			{
				if(m_pGameInfo->bEnableSound)
					PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			}
			return true;
		}
	}
	return true;
}

bool CClientGameDlg::H041_UserLose(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	KillTimer(ID_FLASH_CHESS);
	tagNextStep* p= (tagNextStep*)pNetData;
	m_PlayView.m_Logic.ClearColorAllQiZi(m_PlayView.m_Logic.GetColorFromStation (p->station) );
	m_PlayView.m_Logic.ClearColorAllQiZi(m_PlayView.m_Logic.GetColorFromStation (p->station2) );
	m_PlayView.UpdateViewFace(NULL);

	GameResult gameResult;
	if(p->station == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
		m_PlayView.m_msgbox.ShowMsg(TEXT("\n\n\n\n\n���Ѹ���!"),10, false, gameResult);
	if(p->station2 == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
		m_PlayView.m_msgbox.ShowMsg(TEXT("\n\n\n\n\n���Ѹ���!"),10,false, gameResult);
	return true;
}

/**
 * �û�����
 * @param pNetData �������ݰ�
 * @param uDataSize ���ݰ���С
 */
bool CClientGameDlg::H040_UserOutCard(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	tagMoveResult *p=(tagMoveResult *)pNetData;

	if(p->station==GetMeUserInfo()->bDeskStation&&p->loseColor>=0)
	{
		if(p->FromJq==JQ_TYPE_NAME_JQ)
		{//����Ե��Ǿ���,�������,�Ժ󶼲�������.
			m_PlayView.m_Logic.m_iJingQiI=p->m_iEndI;
			m_PlayView.m_Logic.m_iJingQiJ=p->m_iEndJ;
		}
	}

	if(m_PlayView.m_bRightPower)
	{
		int mystation=GetMeUserInfo()->bDeskStation;
		int s=m_PlayView.m_Logic.GetJq(p->station,p->m_iStartI,p->m_iStartJ);
		int e=m_PlayView.m_Logic.GetJq(p->station,p->m_iEndI,p->m_iEndJ);
		int sn,en;
		sn=GET_A_JQ_NAME(s);
		en=GET_A_JQ_NAME(e);
		int ton=GET_A_JQ_NAME(p->ToJq);
		int fromn=GET_A_JQ_NAME(p->FromJq);
		//TCHAR sz[100];
		//wsprintf(sz,"ms:%d ton%d sn%d en%d",GetMeUserInfo()->bDeskStation,ton,sn,en);
		//MessageBox(sz);

		TCHAR path[MAX_PATH] = {0};
		if(p->FromJq==JQ_TYPE_NAME_JQ)
		{
			int losestation=m_PlayView.m_Logic.GetStationFromColor(p->loseColor);
			
			if((mystation==losestation || (mystation+2)%4==losestation)
				&&p->loseColor2==-1)
			{
				wsprintf(path,TEXT("sound\\shu.mp3"));
				m_playsound2.PlayMusic(m_hWnd,path,1);
			}
			else
			{
				wsprintf(path,TEXT("sound\\win.mp3"));
				m_playsound2.PlayMusic(m_hWnd,path,1);
			}
		}
		else
		{
			if(en==JQ_TYPE_NAME_NONE)
			{//����
				wsprintf(path,TEXT("sound\\zuoqi.mp3"));
				m_playsound2.PlayMusic(m_hWnd,path,1);
			}
			else
			{
				if(JQ_TYPE_NAME_DL==en&&sn==JQ_TYPE_NAME_GB)
				{
					//�ڵ�����
					wsprintf(path,TEXT("sound\\walei.mp3"));
					m_playsound2.PlayMusic(m_hWnd,path,1);
				}
				else if(JQ_TYPE_NAME_ZD==sn||JQ_TYPE_NAME_ZD==en)
				{
					wsprintf(path,TEXT("sound\\zadan.mp3"));
					m_playsound2.PlayMusic(m_hWnd,path,1);
				}
				else if(JQ_TYPE_NAME_NONE==ton)
				{
					wsprintf(path,TEXT("sound\\etoe.mp3"));
					m_playsound2.PlayMusic(m_hWnd,path,1);
				}
				else if(ton==en)
				{
					wsprintf(path,TEXT("sound\\stob.mp3"));
					m_playsound2.PlayMusic(m_hWnd,path,1);
				}
				else if(ton==sn)
				{
					wsprintf(path,TEXT("sound\\btos.mp3"));
					m_playsound2.PlayMusic(m_hWnd,path,1);
				}
			}
		}
	}

	if(p->FromJq==JQ_TYPE_NAME_JQ)
		p->FromJq=JQ_TYPE_NONE;

	if(p->m_iStartI!=-1)
	{
		m_MResultPath.Init();
		m_MResultPath.PathMsg = *p;
		m_PlayView.m_Logic.InitPath();

		// Ѱ�ҵ�һ��λ��
		for (int j=0; j<MAX_I; j++)
		{
			for (int k=0; k<MAX_J; k++)
			{
				// ��Ϊ�ǵ�һ��λ��
				if (255!=m_MResultPath.PathMsg.cPath[j][k] && 0==abs(j-p->m_iStartI) && 0==abs(k-p->m_iStartJ))
				{
					m_MResultPath.CurPath.x = j;
					m_MResultPath.CurPath.y = k;
					m_MResultPath.bContinueInsertPos[j][k] = TRUE;

					j = MAX_I;
					k = MAX_J;
					break;
				}
			}
		}

		m_MResultPath.iCurStep = 1;
		SetTimer(ID_SHOW_PATH, 1*p->iStep, NULL);
	}

	CString AllResult,TmpResult;
	AllResult.Format("Test:DDDD %d",p->iStep);
	for (int i=0;i<17; i++)
		for (int j=0;j<17; j++)
			if (m_MResultPath.PathMsg.cPath[i][j]!=255)
			{
				TmpResult.Format("  (%d,%d)",i,j);
				AllResult+=TmpResult;
			}
			Msg(AllResult);

		
	m_PlayView.m_Logic.m_iNowOutPeople= p->nextstation;

	if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation==m_PlayView.m_Logic.m_iNowOutPeople))
	{
		/*if(!m_PlayView.m_Logic.bCanIMove())
		{//���ӿ�������
			tagMove  m;
			m.m_iStartI=-1;
			m.m_iStartJ=-1;
			m.m_iEndI=-1;
			m.m_iEndJ=-1;
			SendGameData(&m,sizeof(m),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);	
			return true;
		}*/
		
		//wushuqun 2008.8.20
		//�޸ġ������뿪�� "��ǰ��������ť������

		m_PlayView.m_btStop.EnableWindow(true);
		m_PlayView.m_btThing.EnableWindow(true);
	    m_PlayView.m_Logic.m_bICanMove=true;
		KillTimer(ID_FLASH_CHESS);
	}
	else
	{
		//�޸ġ������뿪�� "��ǰ��������ť������
		m_PlayView.m_btStop.EnableWindow(FALSE);
		m_PlayView.m_btThing.EnableWindow(false);
	}
	if (m_PlayView.m_Logic.m_iNowOutPeople!=-1) 
			 SetGameTimer(m_PlayView.m_Logic.m_iNowOutPeople,m_iLessCount,ID_OUT_CARD);
	else
		KillAllTimer();
	
	m_PlayView.UpdateViewFace(NULL);
	return TRUE;
}

/**
 * ��ʼ������Ϸ
 * @param pNetData �������ݰ�
 * @param uDataSize ���ݰ���С
 */
bool CClientGameDlg::H030_NextStep(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	if(GS_BUZHEN==GetStationParameter()&&m_pGameInfo->bEnableSound)
	{
		TCHAR path[MAX_PATH] = {0};
		wsprintf(path,TEXT("sound\\finishbuzhen.mp3"));
		m_playsound2.PlayMusic(m_hWnd,path,1);
		wsprintf(path,TEXT("sound\\backsound.mp3"));
		m_playsound.PlayMusic(m_hWnd,path);
	}
	SetStationParameter(GS_PLAY_GAME);

	tagNextStep *s =(tagNextStep *)pNetData;

	m_PlayView.m_Logic.m_iNowOutPeople= s->station;
	if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation==m_PlayView.m_Logic.m_iNowOutPeople))
	{
		//�޸ġ������뿪�� "��ǰ��������ť������
        //wushuqun 2008.8.20
		m_PlayView.m_btStop.EnableWindow(true);
		m_PlayView.m_btThing.EnableWindow(true);

	    m_PlayView.m_Logic.m_bICanMove=true;		
	}
	if (m_PlayView.m_Logic.m_iNowOutPeople!=-1) 
	{

		SetGameTimer(m_PlayView.m_Logic.m_iNowOutPeople,m_iLessCount,ID_OUT_CARD);
	}

	m_PlayView.UpdateViewFace(NULL);

	return TRUE;
}

/**
 * ͬ���û��뿪
 * @param pNetData �������ݰ�
 * @param uDataSize ���ݰ���С
 */
bool CClientGameDlg::H0011_UserBuZhen(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	tagBuZhen * bz=(tagBuZhen *)pNetData;

	m_PlayView.m_Logic.CopyOtherBuZhen (*bz);

	m_PlayView.UpdateViewFace(NULL);
	return true;
}

int CClientGameDlg::GetPlayerCount()
{
	int count=0;
	for(int i=0;i<4;i++)
	{
		if(m_pUserInfo[i])
			count++;
	}
	return count;
}

/**
 * ��Ϸ��ʼ
 * @param pNetData �������ݰ�
 * @param uDataSize ���ݰ���С
 */
bool CClientGameDlg::H010_GameBegin(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	m_bTimeOutCount=0;
	//m_PlayView.Init();
	if (uDataSize!=sizeof(BeginGameStruct)) return FALSE;
	BeginGameStruct * pBegin=(BeginGameStruct *)pNetData;
	m_bTimeOutCount=0;
	SetStationParameter(GS_BUZHEN);
	m_PlayView.m_Logic.m_iPlayerCount=GetPlayerCount();
	if(m_PlayView.m_Logic.bFinishBuzhen())
		return true;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		m_pUserInfo[i]->GameUserInfo.bUserState = USER_PLAY_GAME;
	}

	if(!IsWatcher( ))
	{
		if(m_pGameInfo->bEnableSound)
		{
			TCHAR path[MAX_PATH] = {0};
			wsprintf(path,TEXT("sound\\buzhen.mp3"));
			m_playsound.PlayMusic(m_hWnd,path);
		}

		m_PlayView.m_Logic.m_bICanMove = true;
		m_PlayView.m_btFinishDiaoDu.ShowWindow (SW_SHOW);
		m_PlayView.m_btLoadBuZhen.ShowWindow (SW_SHOW);
		m_PlayView.m_btSaveBuZhen.ShowWindow (SW_SHOW);

		m_PlayView.m_btFinishDiaoDu.EnableWindow (true);
		m_PlayView.m_btLoadBuZhen.EnableWindow (true);
		m_PlayView.m_btSaveBuZhen.EnableWindow (true);

	}
	else
	{
		m_PlayView.m_Logic.m_bICanMove = false;
		m_PlayView.m_btFinishDiaoDu.ShowWindow (SW_HIDE);
		m_PlayView.m_btLoadBuZhen.ShowWindow (SW_HIDE);
		m_PlayView.m_btSaveBuZhen.ShowWindow (SW_HIDE);
	}

	m_PlayView.m_GameNoteView.SetMessage(NULL);
	if(!IsWatcher( ))
	{
		m_PlayView.m_btStop.EnableWindow(FALSE);//5.18 2009
		m_PlayView.m_btThing.EnableWindow(FALSE);
	}
	if (m_bWatchMode==TRUE) 
	{
		KillAllTimer();
		//ResetGameStation(RS_GAME_BEGIN);
	}
	else
	{
		/*m_PlayView.m_btStop.EnableWindow(true);
		m_PlayView.m_btThing.EnableWindow(true);*/
		
	}

	KillAllTimer();
	m_PlayView.KillCheckTimer ();
	if(!IsWatcher( ))
		m_PlayView.SetCheckTimer(90 * 1000);
	m_PlayView.UpdateViewFace(NULL);
	return TRUE;
}

/**
 * ������ж�ʱ��
 */
void CClientGameDlg::KillAllTimer()
{
	memset(m_PlayView.m_uTime,0,sizeof(m_PlayView.m_uTime));
	KillGameTimer(ID_BEGIN_TIME);
	KillGameTimer(ID_OUT_CARD);
	KillTimer(ID_FLASH_CHESS);
	
	return;
}

bool CClientGameDlg::H000_UserSetGameJu(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
//	g_bIsWatcher = m_bWatchMode;
	if (uDataSize!=sizeof(UserArgeeGame)) return FALSE;
	SetStationParameter(GS_WAIT_ARGEE);
	UserArgeeGame * pGameSet=(UserArgeeGame *)pNetData;
				
	//��������
	m_iLessCount=pGameSet->iPlayCount;
//	m_iPalyCardCount=pGameSet->iCardCount;

	if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation!=0))
	{
		m_PlayView.m_GameNoteView.SetMessage(NULL);
		//m_PlayView.m_GameInfo.SetGameSetInfo(1,1,1,1);
		m_PlayView.m_GameInfo.SetGameSetInfo(pGameSet->iCardCount,pGameSet->iPlayCount,
							3,5);
	}
	else m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ�������ҿ�ʼ��Ϸ"));
	return TRUE;
}

int GetStationParameter()
{
	if(!g_pCClientGameDlg)return -1;


    return g_pCClientGameDlg->GetStationParameter();
}

bool  IsWatcher( )
{
	return g_pCClientGameDlg->IsWatcher( );
}

int  GetStation( int viewStation)
{//�滭���溯��   m_rcMjDesk
	for (int i=0;i<4;i++)
	{
		if(g_pCClientGameDlg->ViewStation(i) == viewStation)return i;
	}
	return -1;
}

/**
 * ��Ϸ��Ϣ������
 */
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	Msg("Test:Data ��ǰ��Ϣ %d  __id %d", pNetHead->bAssistantID,GetMeUserInfo()->bDeskStation);

	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_LOSE:
			{
				return H041_UserLose(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_BUZHEN:
			{
				return H0011_UserBuZhen(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}		
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				return H081_UserContinueEnd(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_HAVE_THING:	//�û������뿪
			{
				return H082_UserHaveThing(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_LEFT_RESULT:	//�����뿪��� 
			{
				return H083_UserHaveThingResult(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_MESSAGE:
			{
				if (uDataSize!=sizeof(MessageStruct))
				{
					
					return FALSE;
				}
				MessageStruct *pMessage=(MessageStruct*)pNetData;
				m_MessageHandle.InsertSystemMessage(pMessage->Message);
				return true;
			}
		case ASS_STOP_THING://�Լ�������ǰ����
			{
				return H084_DuiJiaHaveThing(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_AGREE_STOP://������ǰ�������
			{
				return H085_DuiJiaHaveThingResult(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}

		case ASS_NEXT_STEP://ASS_GAME_PLAY:		//��ʼ������Ϸ
			{
				return H030_NextStep(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_OUT_CARD:		//�û�����
			{
				return H040_UserOutCard(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}		
		
		case ASS_GAME_POINT:	//�û�����
			{
//				return H043_UserFen(pNetHead, pNetData,  uDataSize, pClientSocket);
			}		
		case ASS_SALE_END:	//��Ϸ��ȫ����
		case ASS_CUT_END:	//�û�ǿ���뿪
			{
				return H080_UserLeftOrGameAnQuanFinish(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
			
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				return H000_UserSetGameJu(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		case ASS_BEGIN_UPGRADE:		//��Ϸ��ʼ
			{
				m_PlayView.m_Result.ShowWindow(SW_HIDE);
				 return H010_GameBegin(pNetHead, pNetData,  uDataSize/*, pClientSocket*/);
			}
		

		}
	}
	
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize/*,pClientSocket*/);
}

/**
 * ��Ϸ��ȫ����,�û�ǿ���뿪
 */
bool CClientGameDlg::H080_UserLeftOrGameAnQuanFinish(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
    if (uDataSize!=sizeof(GameCutStruct)) 
		return FALSE;

	IsStation(true);
	GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
	KillGameTimer(0);

	if(pGameEnd->bDeskStation != GetMeUserInfo()->bDeskStation&&m_pUserInfo[pGameEnd->bDeskStation]!=NULL)
	{
		//��ʾ��Ϣ
		TCHAR szMessage[100];
		if (pNetHead->bAssistantID==ASS_CUT_END)
			wsprintf(szMessage,TEXT("���ڡ� %s ��ǿ���˳�����Ϸ������"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);
		else 
			wsprintf(szMessage,TEXT("���ڡ� %s ���뿪����Ϸ������"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);
		//	m_PlayView.m_StationView.CleanAllInfo(TRUE);
		m_MessageHandle.InsertNormalMessage(szMessage);
	}
	if(m_pGameInfo->bEnableSound)
		PlaySound(TEXT("GAME_OVER"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
	SetStationParameter(GS_WAIT_ARGEE);
	m_PlayView.KillCheckTimer();
	m_PlayView.m_btFinishDiaoDu.ShowWindow(false);
	m_PlayView.m_btLoadBuZhen.ShowWindow(false);
	m_PlayView.m_btSaveBuZhen.ShowWindow(false);
	m_PlayView.m_btThing.EnableWindow(SW_HIDE);			///< ����Ҫ��
	m_PlayView.m_btStop.EnableWindow(SW_HIDE);				///< ֹͣ����
	KillAllTimer();

	//��һ������
/*	if (m_bShowIngLast)
	{
		m_bShowIngLast=FALSE;
		KillTimer(ID_SHOW_LAST);
		//for (int i=0;i<4;i++)
//			m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);
	}
	m_bShowLast=FALSE;
	m_iLastCardCount=0;*/
	ResetGameStation(RS_GAME_CUT);

	//�����ƾ�
	//if ((GetMeUserInfo()->bDeskStation==0)&&(m_bWatchMode==FALSE))
	//	m_PlayView.m_GameSet.SetGameSetInfo(1);//m_iCardCount);
	//else
	//	m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ����������ƾ�"));
	if(!m_bWatchMode)
	{
		m_PlayView.m_btContinue.ShowWindow(true);
		SetGameTimer(GetMeUserInfo()->bDeskStation,20,ID_BEGIN_TIME);
		//m_PlayView.m_btleave.ShowWindow(true);
	}
	if(pGameEnd->bDeskStation==GetMeUserInfo()->bDeskStation&&pNetHead->bAssistantID==ASS_SALE_END)
		OnCancel();
	return TRUE;
}

/**
 * �û������뿪
 */
bool CClientGameDlg::H082_UserHaveThing(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	HaveThingStruct * pThing=(HaveThingStruct *)pNetData;
	if (m_PlayView.m_ThingMessage.GetSafeHwnd()==NULL) 
	{
		m_PlayView.m_ThingMessage.Create(IDD_THING_MESSAGE, &m_PlayView);
		m_PlayView.m_ThingMessage.m_pParent = &m_PlayView;
	}	
	m_PlayView.m_ThingMessage.SetLeaveMessage(pThing->szMessage,m_pUserInfo[pThing->pos]->GameUserInfo.nickName);
	m_PlayView.m_ThingMessage.CenterWindow(&m_PlayView);
	m_PlayView.m_ThingMessage.ShowWindow(SW_SHOW);
	return TRUE;
}

/**
 * �����뿪���
 */
bool CClientGameDlg::H083_UserHaveThingResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	if (uDataSize!=sizeof(LeaveResultStruct)) return FALSE;
	LeaveResultStruct * pLeftResult=(LeaveResultStruct *)pNetData;
	CString strBuffer;
	if (!pLeftResult->bArgeeLeave) 
//	strBuffer.Format(TEXT("��� %s ͬ����Ϸ������"),m_UserInfo[pLeftResult->bDeskStation].szName);
//	else 
	{
		strBuffer.Format(TEXT("��� %s ��ͬ����Ϸ�����������ֽ����������뿪�߿����뿪��"),m_pUserInfo[pLeftResult->bDeskStation]->GameUserInfo.nickName);
		m_MessageHandle.InsertNormalMessage(strBuffer.GetBuffer(MAX_PATH));
	}
	return TRUE;
}

/**
 * �Լ�������ǰ����
 */
bool CClientGameDlg::H084_DuiJiaHaveThing(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	/*
	if (m_PlayView.m_StopMessage.GetSafeHwnd()==NULL) 
	{
		m_PlayView.m_StopMessage.Create(IDD_STOP_MESSAGE,this);
		m_PlayView.m_StopMessage.m_pParent=this;
	}
	*/
	m_PlayView.m_StopMessage.CenterWindow(&m_PlayView);
	m_PlayView.m_StopMessage.ShowWindow(SW_SHOW);
	return true;
}

/**
 * �Լ�������ǰ�������
 */
bool CClientGameDlg::H085_DuiJiaHaveThingResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	if (uDataSize!=sizeof(StopResultStruct))return FALSE;
	StopResultStruct *pStop=(StopResultStruct*)pNetData;
	if (pStop->bArgee) 
	{
		m_MessageHandle.InsertNormalMessage("�Լ�ͬ����ǰ������");
	}
	else 
	{
		m_MessageHandle.InsertNormalMessage("�Լ���Ϊ�����������Ļ��ᡣ");
	}
	return true;
}

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClassInExe)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(IDM_BEGIN,OnHitBegin)
//	ON_MESSAGE(IDM_LOOK_LAST,OnHitLastTurn)
	ON_MESSAGE(IDM_HAVE_THING,OnHitHaveThing)
	ON_MESSAGE(IDM_SEND_THING,OnSendHaveThing)
	ON_MESSAGE(IDM_ARGEE_LEAVE,OnArgeeUserLeft)
	ON_MESSAGE(IDM_STOP,OnStop)
	ON_MESSAGE(IDM_STOP_THING,OnStopThing)			//������ǰ�����¼�
	ON_MESSAGE(IDM_ARGEE_STOP,OnAgreeStop)			//ͬ����ǰ�����¼�
	ON_MESSAGE(IDM_SET_FLASH_CHESS,OnFlashChess)
	
	ON_WM_TIMER()
END_MESSAGE_MAP()

CClientGameDlg * g_pCClientGameDlg=NULL;

/**
 * ���캯��
 */
CClientGameDlg::CClientGameDlg() : CLoveSendClassInExe(&m_PlayView)
{
	g_pCClientGameDlg=this;
	m_GameSet=NULL;
	m_iVersion=7;
	m_iVersion2=0;
	m_iLessCount=0;

	m_bTimeOutCount=0;
	m_iAfterWaitAction = 2;
}

/**
 * ��������
 */
CClientGameDlg::~CClientGameDlg()
{
}

/**
 * ���ݰ󶨺���
 */
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClassInExe::DoDataExchange(pDX);
	DoDataExchangeWebVirFace(pDX);
}

/**
 * ��ʼ������
 */
BOOL CClientGameDlg::OnInitDialog()
{
	m_PlayView.m_showcoredlg.m_ucomtype=GetComType();
	CLoveSendClassInExe::OnInitDialog();
	//���±���
	SetDialogIcon(LoadIcon(GetModuleHandle(CLIENT_DLL_NAME),MAKEINTRESOURCE(IDI_CLIENT_ROOM)),TRUE);
	
	m_PlayView.m_bRightPower=GetProfileInt(TEXT("RightPower"),TRUE);

	m_playsound.Create(_T("STATIC"), "", WS_CHILD|WS_VISIBLE,  CRect(0, 0, 20, 20), this, 1234);
	m_playsound2.Create(_T("STATIC"), "", WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), this, 1235);
	m_playsound.ShowWindow(false);
	m_playsound2.ShowWindow(false);
	m_PlayView.m_Logic.InitPath();

	//CString s = _T("GameOption.bcf");	//����·��
	CString s;
#ifdef	VIDEO
	s = _T("30300405.bcf");	//����·��
#else
	s = _T("30300400.bcf");	//����·��
#endif		
	if (CBcfFile::IsFileExist(s))
	{
		CBcfFile f(s);
		m_iAfterWaitAction = f.GetKeyVal(_T("config"), _T("AfterWaitAction"), 2);
		if (m_iAfterWaitAction != 1 && m_iAfterWaitAction != 2)
		{
			m_iAfterWaitAction = 1;
		}
	}

	return TRUE;
}

/**
 * �������ý���
 */
void CClientGameDlg::ResetGameFrame()
{
	m_playsound.StopMusic();
	m_playsound2.StopMusic();
	m_bTimeOutCount=0;
	
	m_PlayView.SetGamePoint();
	m_PlayView.SetNTStation(-1);
	m_PlayView.m_GameNoteView.SetMessage(NULL);
	for (int i=0;i<4;i++)
	{
	}
	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	if (m_PlayView.m_HaveThing.GetSafeHwnd()) m_PlayView.m_HaveThing.DestroyWindow();
	if (m_PlayView.m_ThingMessage.GetSafeHwnd()) m_PlayView.m_ThingMessage.DestroyWindow();
	if (m_PlayView.m_GameSet.GetSafeHwnd()) m_PlayView.m_GameSet.DestroyWindow();
//	if(m_GameSet->GetSafeHwnd())	m_GameSet->EndDialog(0);
	return __super::ResetGameFrame();
}

/**
 * ��ʼ��ť����
 */
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
	//memset(m_PlayView.m_Logic.m_Path, 1, sizeof(m_PlayView.m_Logic.m_Path));//del
	//m_PlayView.m_Logic.m_Path[0][0]=1;//�ҷ�
	//m_PlayView.m_Logic.m_Path[0][1]=2;//��
	//m_PlayView.m_Logic.m_Path[0][2]=3;//�Ϸ�
	//m_PlayView.m_Logic.m_Path[0][3]=4;//�·�
	//m_PlayView.m_Logic.m_Path[0][4]=5;//����
	//m_PlayView.m_Logic.m_Path[0][5]=6;//����
	//m_PlayView.m_Logic.m_Path[0][6]=7;//����
	//m_PlayView.m_Logic.m_Path[0][7]=8;//����

	//m_PlayView.UpdateViewFace(NULL);
	//return 0;//del
	OnControlHitBegin();
	return 0;
}

bool CClientGameDlg::OnControlHitBegin()
{
	if ((GetStationParameter()==GS_WAIT_SETGAME)&&(GetMeUserInfo()->bDeskStation==0))
	{
		tagGameSetInfo * pGameSet=m_PlayView.m_GameSet.GetGameSetInfo();
		if (pGameSet!=NULL)
		{
			UserArgeeGame Agree;
//			Agree.iBeginStation=pGameSet->iBeginStation;
//			Agree.iEndStation=pGameSet->iEndStation;
			Agree.iPlayCount=pGameSet->iPlayCount;
			Agree.iCardCount=pGameSet->iCardCount;
			SendGameData(&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
		}
		m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
		//m_PlayView.m_btleave.ShowWindow(SW_HIDE);

	}
	else if ((GetStationParameter()==GS_WAIT_NEXT)||(GetStationParameter()==GS_WAIT_ARGEE))
	{
		m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
		//m_PlayView.m_btleave.ShowWindow(SW_HIDE);
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}
	KillAllTimer();
//	m_PlayView.m_StationView.CleanAllInfo(FALSE);
//	m_PlayView.m_StationView.m_iTurePoint=0;
	//ResetGameStation(RS_GAME_BEGIN);
	m_PlayView.m_btFinishDiaoDu.ShowWindow (SW_SHOW);
	m_PlayView.m_btLoadBuZhen.ShowWindow (SW_SHOW);
	m_PlayView.m_btSaveBuZhen.ShowWindow (SW_SHOW);

	m_PlayView.m_btFinishDiaoDu.EnableWindow (true);
	m_PlayView.m_btLoadBuZhen.EnableWindow (true);
	m_PlayView.m_btSaveBuZhen.EnableWindow (true);

	m_PlayView.m_Logic.StartBuzhen();
	m_PlayView.UpdateViewFace(NULL);
	m_PlayView.m_Result.ShowWindow(false);
	m_PlayView.m_Result.KillTimer(IDR_HIDE);
	return true;
}

/**
 * �������°�ť����
 */
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	if (m_PlayView.m_HaveThing.GetSafeHwnd()==NULL) 
	{
		AfxSetResourceHandle(GetModuleHandle(GET_CLIENT_DLL_NAME(szTempStr)));
		m_PlayView.m_HaveThing.Create(IDD_THING,&m_PlayView);
		m_PlayView.m_HaveThing.m_pParnet=&m_PlayView;
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	m_PlayView.m_HaveThing.CenterWindow(&m_PlayView);
	m_PlayView.m_HaveThing.ShowWindow(SW_SHOW);
	m_PlayView.m_HaveThing.SetFocus();
	return 0;
}

/**
 * ��ǰ����
 */
LRESULT	CClientGameDlg::OnStop(WPARAM wparam, LPARAM lparam)
{
//	if(m_PlayView.m_bMeSide && m_PlayView.m_iGamePoint>=80 && !m_bWatchMode)
	{
		/*
		if (m_PlayView.m_StopThing.GetSafeHwnd()==NULL) 
		{
			m_PlayView.m_StopThing.Create(IDD_STOP,this);
			m_PlayView.m_StopThing.m_pParnet=this;
		}
		*/
		m_PlayView.m_StopThing.CenterWindow(&m_PlayView);
		m_PlayView.m_StopThing.ShowWindow(SW_SHOW);
		m_PlayView.m_StopThing.SetFocus();
		
	}
	return 0;
}

/**
 * ������ǰ����
 */
LRESULT	CClientGameDlg::OnStopThing(WPARAM wparam, LPARAM lparam)
{
//	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME && m_PlayView.m_bMeSide && m_PlayView.m_iGamePoint>=80)
	{
		
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
	}
	return 0;
}

/**
 * ͬ����ǰ����
 */
LRESULT	CClientGameDlg::OnAgreeStop(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
		StopResultStruct Stop;
		Stop.bArgee=(BYTE)wparam;
		SendGameData(&Stop,sizeof(Stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
	}
	return 0;
}


/**
 * ������˸����
 */
LRESULT	CClientGameDlg::OnFlashChess(WPARAM wparam, LPARAM lparam)
{
	if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation==m_PlayView.m_Logic.m_iNowOutPeople))
	{
		CRect rcEnd;
		int ix, iy;
		POINT ptTmp;
		m_PlayView.m_Logic.SetFlashChess(ix, iy, true);
		
		ptTmp.x = ix;
		ptTmp.y = iy;
		m_PlayView.m_Logic.GetRefurbishRECT(rcEnd, ptTmp);
		m_PlayView.UpdateViewFace(&rcEnd);

		SetTimer(ID_FLASH_CHESS, 500, NULL);
	}

	return 0;
}

/**
 * �����뿪������
 */
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()>GS_WAIT_ARGEE)
	{
		m_PlayView.m_btThing.EnableWindow(FALSE);
		HaveThingStruct HaveThing;
		HaveThing.pos=0;
		::lstrcpy(HaveThing.szMessage,(char *)wparam);
		SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}

	return 0;
}

/**
 * �û������뿪
 */
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	LeaveResultStruct Leave;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	Leave.bArgeeLeave=(BYTE)wparam;
	SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	return 0;
}

/**
 * ������Ϸ
 */
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	m_PlayView.m_btFinishDiaoDu.EnableWindow (false);
	m_PlayView.m_btFinishDiaoDu.ShowWindow (false);
	m_PlayView.m_btLoadBuZhen.EnableWindow (false);
	m_PlayView.m_btLoadBuZhen.ShowWindow (false);
	m_PlayView.m_btSaveBuZhen.EnableWindow (false);
	m_PlayView.m_btSaveBuZhen.ShowWindow (false);
	m_PlayView.m_StopThing.ShowWindow(false);
	m_PlayView.m_StopMessage.ShowWindow(false);
	
	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	if (m_PlayView.m_HaveThing.GetSafeHwnd()) m_PlayView.m_HaveThing.DestroyWindow();
	if (m_PlayView.m_ThingMessage.GetSafeHwnd()) m_PlayView.m_ThingMessage.DestroyWindow();	
		
	switch (iGameStation)
	{
	case RS_ALL_VIEW:
	case RS_GAME_CUT:
		{
			//�����Ƿ����½�����
			m_PlayView.m_bFirstGame = false;

			m_PlayView.Init();
			m_PlayView.SetGamePoint();
			m_PlayView.SetNTStation(-1);
			if (m_PlayView.m_GameSet.GetSafeHwnd()) m_PlayView.m_GameSet.DestroyWindow();
			m_playsound.StopMusic();
			m_playsound2.StopMusic();
			break;
		}
	case RS_GAME_BEGIN:
		{
			m_PlayView.m_Result.ShowWindow(false);
			m_PlayView.Init();
			m_PlayView.SetGamePoint();
			m_PlayView.SetNTStation(-1);
			if (m_PlayView.m_GameSet.GetSafeHwnd()) m_PlayView.m_GameSet.DestroyWindow();
			break;
		}
	case RS_GAME_END:
		m_PlayView.m_Logic.Init ();
		KillTimer(ID_FLASH_CHESS);
		break;
	}
	
	return;
}

void CClientGameDlg::OnGameSetting()
{
	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	CGameSet GameSetDlg(this);
	m_GameSet=&GameSetDlg;
	GameSetDlg.m_bSound=m_pGameInfo->bEnableSound;
	GameSetDlg.m_bShowUser=m_pGameInfo->bShowUserInfo;	
	GameSetDlg.m_bEnableWatch=m_pGameInfo->bEnableWatch;
	GameSetDlg.m_bRightPower=GetProfileInt(TEXT("RightPower"),TRUE);

	if (GameSetDlg.DoModal()==IDOK)
	{
		m_GameSet=NULL;
		//�������
		bool bSendWatch=((m_bWatchMode==false)&&(m_pGameInfo->bEnableWatch!=GameSetDlg.m_bEnableWatch));

		//��������
		m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
		m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
		m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
		WriteProfileInt(TEXT("RightPower"),GameSetDlg.m_bRightPower?1:0);
		m_PlayView.m_bRightPower=GameSetDlg.m_bRightPower;//������Ч

		//��������
		if (bSendWatch==true)
		{
			MSG_GM_WatchSet WatchSet;			
			memset(&WatchSet,0,sizeof(WatchSet));
			WatchSet.dwUserID=0;
			SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);
		}
		if(!m_bWatchMode)
		{
			if(m_pGameInfo->bEnableSound)
			{
				TCHAR path[MAX_PATH] = {0};

				if(GS_BUZHEN==GetStationParameter())
				{
					wsprintf(path,TEXT("sound\\buzhen.mp3"));
					m_playsound.PlayMusic(m_hWnd,path);
				}
				else if(GS_BUZHEN<GetStationParameter())
				{
					wsprintf(path,TEXT("sound\\backsound.mp3"));
					m_playsound.PlayMusic(m_hWnd,path);
				}
			}
			else
			{
				m_playsound.StopMusic();
				m_playsound2.StopMusic();
			}
		}
	}
	m_GameSet=NULL;

	AfxSetResourceHandle(GetModuleHandle(NULL));

	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{
	if (m_bWatchMode==true)
	{
		if (m_bWatchOther)
			m_MessageHandle.InsertNormalMessage(TEXT("����������Թ�����Ϸ"));
		else
			m_MessageHandle.InsertNormalMessage(TEXT("��Ҳ��������Թ�����Ϸ"));
		m_PlayView.m_Logic.m_bWatchOther=m_bWatchOther;
		m_PlayView.Invalidate(true);
	}
}

/**
 * �û��뿪
 */
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if ((bWatchUser==false)&&(pUserItem!=NULL))
	{
		if (GetStationParameter()==GS_WAIT_ARGEE)
		{
			/*for (int i=0;i<4;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
				{
					m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
					m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
				}
			}
			if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
			m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ����������ƾ�"));
			m_PlayView.UpdateViewFace(NULL);*/
			tagBuZhen  bz;
			bz.station=bDeskStation;
			bz.Count=30;
			for(int i=0;i<bz.Count;i++)
			{
				bz.CardArr[i]=JQ_TYPE_NONE;
			}			
			m_PlayView.m_Logic.CopyOtherBuZhen(bz);
		}
	}
	//�Թ���Ҳ�뿪��Ϸ��
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser==TRUE)
	{
		OnCancel();
	}
	return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	
}

void CClientGameDlg::OnTimer(UINT nIDEvent)
{

	if (nIDEvent==ID_FLASH_CHESS)
	{
		CRect rcEnd;
		int ix, iy;
		POINT ptTmp;
		m_PlayView.m_Logic.SetFlashChess(ix, iy, !m_PlayView.m_Logic.GetFlashChess());
		
		ptTmp.x = ix;
		ptTmp.y = iy;
		m_PlayView.m_Logic.GetRefurbishRECT(rcEnd, ptTmp);
		m_PlayView.UpdateViewFace(&rcEnd);
	}
	else if (nIDEvent==ID_SHOW_PATH)
	{
		// [tmpX,tmpY]��Ҫ���ü�ͷ��λ�ã�
		// ��Ϊ��Ҫȷ���������ԲŻ���FrontPath, BackPath
		// FrontPath��ʾ����λ�õ�ǰһλ�ã�BackPath��ʾ����λ�õĺ�һλ��
		for (int j=0; j<MAX_I; j++)
		{
			for (int k=0; k<MAX_J; k++)
			{
				// λ�ò���Ϊ�գ�ǰ��λ��x,y��Ҫ��һ������ֵΪ1
				//��Ϊʲôֻ��Ҫһ������ʱ��һ������Ϊ0�����϶���һ��Ϊ1��
				// bContinueInsertPosition ��λ���Ƿ��Ѿ�����
				if ( 255!=m_MResultPath.PathMsg.cPath[j][k]    &&    \
					(1==abs(j-m_MResultPath.CurPath.x)||1==abs(k-m_MResultPath.CurPath.y))    &&    \
					!m_MResultPath.bContinueInsertPos[j][k] )
				{
					// cPath ����ÿ��λ�õ����������������������ġ������λ��֮������û������
					// �����ж������ǲ�������
					if (255!=m_MResultPath.CurPath.x && 255!=m_MResultPath.CurPath.y   &&   \
						1!=abs(m_MResultPath.PathMsg.cPath[j][k] - \
						m_MResultPath.PathMsg.cPath[m_MResultPath.CurPath.x][m_MResultPath.CurPath.y]))
					{
						continue;
					}

					// ȷ����һ��λ��
					m_MResultPath.BackPath.x = j;
					m_MResultPath.BackPath.y = k;
					//���ù켣
					m_PlayView.m_Logic.UserPath(m_MResultPath.PathMsg.station, m_MResultPath.CurPath.x, \
						m_MResultPath.CurPath.y, m_MResultPath.FrontPath, m_MResultPath.BackPath);
					//�����ƶ����������ӣ����������ս����
					m_PlayView.m_Logic.UserMovePathCheen(m_MResultPath.PathMsg.station, m_MResultPath.FrontPath, \
						m_MResultPath.CurPath, m_MResultPath.BackPath,m_MResultPath.PathMsg.m_iEndI, m_MResultPath.PathMsg.m_iEndJ);
					//���ˢ������
					CRect rcCurrent,rcFront,rcEnd;
					m_PlayView.m_Logic.GetRefurbishRECT(rcFront, m_MResultPath.FrontPath);
					m_PlayView.UpdateViewFace(&rcFront);
					m_PlayView.m_Logic.GetRefurbishRECT(rcCurrent, m_MResultPath.CurPath);
					m_PlayView.UpdateViewFace(&rcCurrent);
					m_PlayView.m_Logic.GetRefurbishRECT(rcEnd, m_MResultPath.BackPath);
					m_PlayView.UpdateViewFace(&rcEnd);
					//rcCurrent.UnionRect(rcCurrent, rcFront);
					//rcCurrent.UnionRect(rcCurrent, rcEnd);
					//m_PlayView.UpdateViewFace(&rcCurrent);
					Msg("Test:DDDD ��ǰ����%d rcFront(%d,%d,%d,%d) rcCurrent(%d,%d,%d,%d) rcEnd(%d,%d,%d,%d)",rcCurrent.left,rcCurrent.top,rcCurrent.right,rcCurrent.bottom,\
						rcCurrent.left,rcCurrent.top,rcCurrent.right,rcCurrent.bottom, rcEnd.left, rcEnd.top, rcEnd.right, rcEnd.bottom);

					// ����λ����Ϣ��׼����һλ������
					m_MResultPath.FrontPath.x = m_MResultPath.CurPath.x;
					m_MResultPath.FrontPath.y = m_MResultPath.CurPath.y;
					m_MResultPath.CurPath.x = m_MResultPath.BackPath.x;
					m_MResultPath.CurPath.y = m_MResultPath.BackPath.y;
					m_MResultPath.BackPath.x = 255;
					m_MResultPath.BackPath.y = 255;
					m_MResultPath.bContinueInsertPos[j][k] = TRUE;
				}
			}
		}

		if (m_MResultPath.iCurStep++ >= m_MResultPath.PathMsg.iStep)
		{
			KillTimer(ID_SHOW_PATH);
			m_PlayView.m_Logic.UserMove(m_MResultPath.PathMsg);
			m_PlayView.UpdateViewFace(NULL);
		}
	}
	CLoveSendClassInExe::OnTimer(nIDEvent);
}

void CClientGameDlg::OnCancel()
{
	//��ѯ״̬
	if (!m_bWatchMode && GetStationParameter()>=GS_BUZHEN&&GetStationParameter()<GS_WAIT_NEXT)
	{
		TCHAR sz[200];
		CString s = CBcfFile::GetAppPath ();/////����·��	

#ifdef	VIDEO
		CBcfFile f( "30300405.bcf");
#else
		CBcfFile f( "30300400.bcf");
#endif	
		
		CString key=TEXT("config");		
		CString sec;

		sec.Format("FouceQuitTip%d", m_pGameInfo->uComType);

		int nMoneyTemp = 1;
		for(int i=0; i<Glb().m_nPowerOfGold; i++)
		{
			nMoneyTemp*=10;
		}
		if (GetComType() == TY_MONEY_GAME)
		{
			wsprintf(sz,f.GetKeyVal(key, sec, ""),basepoint*uRunPublish*nMoneyTemp);
		}
		else
		{
			wsprintf(sz,f.GetKeyVal(key, sec, ""),uRunPublish,basepoint*uRunPublish*nMoneyTemp);
		}
		
		
		if (IDOK!=AFCMessageBox(sz,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
			return;
		
		SendGameData(150,3,0);
	}

	/////�ͷ���Ƶ��Դ  
	//if( NULL != CPlayVideo::CreatePlayVideo())
	//	CPlayVideo::CreatePlayVideo()->Video_Release();

	m_PlayView.m_bFirstGame = true;

	AFCCloseFrame();

	__super::OnCancel();
}

BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	if(m_pGameInfo->uDeskPeople==2)
	{
		if(bDeskStation==GetMeUserInfo()->bDeskStation)
			return 2;
		else
			return 0;
	}
	BYTE bViewStation=(bDeskStation+m_pGameInfo->uDeskPeople-GetMeUserInfo()->bDeskStation);
	switch (m_pGameInfo->uDeskPeople)
	{
	case 1: { bViewStation+=1; break; }
	case 2: { bViewStation+=1; break; }
	case 3: { bViewStation+=1; break; }
	case 4: { bViewStation+=2; break; }
	case 5: { bViewStation+=2; break; }
	case 6: { bViewStation+=3; break; }
	case 7: { bViewStation+=3; break; }
	case 8: { bViewStation+=4; break; }
	}
	return bViewStation%m_pGameInfo->uDeskPeople;
}

/**
 * Ϊ��Ƶģ�����ش˺���
 */
bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}

#ifdef	VIDEO
	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && bWatchUser )
	{
		 m_PlayView.m_bWatch=bWatchUser;
	}
	 if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
	{    
		CPlayVideo *pPlayVideo=CPlayVideo::CreatePlayVideo();
		if(NULL!=pPlayVideo)
		{ 
			CString NickName=TEXT("");
			int    byteDeskNO=0;
			UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
			NickName+=UInfo.nickName;
			byteDeskNO=UInfo.bDeskNO;
			UINT	uiRoomID = m_pGameInfo->uRoomID;
			UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

			ST_INITVIDEO stInitVideo;
			stInitVideo.iRoomPassType = 0;
			stInitVideo.nRoomID		  = uiBRRoomID;
			stInitVideo.pParentWnd    = &m_PlayView;
			stInitVideo.strRoomPass   = _T("");
			stInitVideo.strUserName	  = NickName;
			stInitVideo.strUserPass   = _T("");
			if( pPlayVideo->Video_Initialize(stInitVideo) )
			{
				//AppendLog("��Ƶ��ʼ���ɹ�!");
				//������Ƶ������
				pPlayVideo->Connect();
			}
			else
			{
				//pPlayVideo->Video_Release();
				//AppendLog("��Ƶ��ʼ��ʧ��!");
				return 0;
			}

		} 
		 

	}
#endif 
	return TRUE;
}

bool	CClientGameDlg::AFCCloseFrame()
{
#ifdef	VIDEO
		///�ͷ���Ƶ��Դ  
	if( NULL != CPlayVideo::CreatePlayVideo())
		CPlayVideo::CreatePlayVideo()->Video_Release();
#endif

	return __super::AFCCloseFrame();
}

///
///����ǰ��ұ��ʵõ��ַ���
///@param [in] int nNum Ҫ��ʾ������
///@param [out] CString &Str ���ص��ַ���
///@param [in] int nPower ��ұ���
///@param [in] bool bIsBlank �����Ƿ�ӿո�
///@return void
///

void GetNumString(int nNum, CString &Str, int nPower,bool bIsBlank)
{
	if (0 == nNum)
	{
		Str.Format("0");
		return;
	}
	/// �ݴ���
	TCHAR strTmp[50];
    TCHAR str[50];
	/// Ŀǰֻ�������0��������������ַ�������Ҫ������һ��0
	wsprintf( strTmp, TEXT("%d"), nNum);
	for (int i=0; i<nPower; ++i)
	{
		_tcscat( strTmp, TEXT("0"));
	}
	/// ��λ��ÿ��λ��һ����ʾ
	int nLen = _tcslen(strTmp);	// �ַ�������
	int nTimes = nLen % 3;	// ����ַ������ԷֳɵĽ�����������1��
	
	int nSrc = 0;
	int nDes = 0;

	for (int i=0; i<nLen; ++i)
	{
		if ((i>0)&&(i%3==nTimes)&&bIsBlank)
		{
			str[nDes++] = TEXT(' ');
		}
		str[nDes++] = strTmp[nSrc++];
	}
	str[nDes] = TEXT('\0');
	Str.Format("%s",str);
	return ;
}