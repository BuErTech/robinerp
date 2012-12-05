#include "StdAfx.h"
#include "GameSet.h"
#include "math.h"
#include ".\clientgamedlg.h"
#include "playcardsound.h"
#include "bzsound.h"
#ifdef SUPER_VERSION
#include "platform/CardSet.h"
#endif

//#include "..\..\��Ϸ����������\include\�ͻ���\�����\resource.h"
//#pragma comment(lib,"vfw32.lib")

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClassInExe)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(IDM_BEGIN,OnHitBegin)			//��ʼ��ť
	ON_MESSAGE(IDM_BEGINDISPLAY,OnHitBeginDisplayCards)			//���ƿ�ʼ��ť
	ON_MESSAGE(IDM_LEFT_HITCARD,OnHitCard)			//�����
	ON_MESSAGE(IDM_HAVE_THING,OnHitHaveThing)		//����Ҫ��
	ON_MESSAGE(IDM_SEND_THING,OnSendHaveThing)		//����Ҫ��
	ON_MESSAGE(IDM_LOOK_LAST,OnLastTurn)
	ON_MESSAGE(IDM_AUTOCARD,OnAuto)					//�й�

	ON_MESSAGE(IDM_ARGEE_LEAVE,OnArgeeUserLeft)		//ͬ���û��뿪
	ON_MESSAGE(IDM_RIGHT_HITCARD,OnRightHitCard)	//�Ҽ�����
	ON_MESSAGE(IDM_CALL_SCORE,OnCallScore)				//�з�
	ON_MESSAGE(IDM_SORT,OnHitSortCard)				//����

	ON_MESSAGE(IDM_PASS,OnHitPassCard)				//����
	ON_MESSAGE(IDM_CUE,OnCue)					//��ʾ
	ON_MESSAGE(IDM_OUT_CARD,OnHitOutCard)			//����

	ON_MESSAGE(IDM_ROB_NT,OnRobNT)					//������
	ON_MESSAGE(IDM_ADD_DOUBLE,OnAddDouble)					//�Ӱ�
	ON_MESSAGE(IDM_SHOW_CARD,OnShowCard)					//����
	ON_MESSAGE(IDM_SELECT_NUM,OnSelectNum)					//ѡ��
	ON_WM_TIMER()

END_MESSAGE_MAP()

void MyMsgBox(const char *lpszFormat, ...)
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
	AfxMessageBox(szMessage);
}

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClassInExe(&m_PlayView)
{
	m_bAutoCard=false;
	//	m_iToux=FALSE;
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;	
	m_iBeenPlayGame=0;
	////	m_iCardCount=0;
	m_iMeCardCount=0;
	m_bShowIngLast=FALSE;
	m_bShowLast=FALSE;
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	//	m_bisexit=false;
	m_iNowOutPeople=-1;		//��ǰ������
	m_bCurrentOperationStation = 255;//��ǰ�з���
	m_iFirstOutPeople=-1;
	m_iBigOutPeople = -1;
	m_iNtPeople=-1;
	m_ResultCallScore=-1;			//��ʼ��
	m_btAutoTime=0;
	m_bSortCard=0;
	m_bTimeOutCount=0;
	m_iBaseCount = 0;
	m_iAfterWaitAction = 1;		//��ʼ��Ϊ�Զ���ʼ 090311a1 JJ
	//::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_PlayView.SetAuto(i,false);
		m_PlayView.SetAwardPoint(i,0);
		m_iAIStation[i] = 0;
	}

	m_PlayView.m_MeBackCard.SetCard(0,NULL,0);
	m_hMciWnd1=NULL;
	m_hMciWnd2=NULL;
	m_hMciWnd3=NULL;
	m_bPlay= FALSE;
	m_bIsStartDisplay = FALSE;//�Ƿ����ƿ�ʼ
	bInitFinish = false; //��ֹ�Թ۱���
}

void CClientGameDlg::SetColor()				//���õ�ǰ��ɫ
{	
	colorresource.m_crDeskTxColor=RGB(240,240,240);
	colorresource.m_crListBkColor=RGB(240,240,250);
	colorresource.m_crListTxColor=RGB(0,0,0);	

	colorresource.m_crNormalTxColor=RGB(50,255,50);
	colorresource.m_crSysHeadTxColor=RGB(255,0,0);
	colorresource.m_crSystemTxColor=RGB(255,255,255);


	colorresource.m_crTalkBkColor=RGB(233,233,233);
	colorresource.m_crTalkNameColor=RGB(255,255,0);
	colorresource.m_crTalkTxColor=RGB(255,255,255);
}
//��������
CClientGameDlg::~CClientGameDlg()
{
	if(m_hMciWnd1)
	{
		MCIWndStop(m_hMciWnd1);
		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
		MCIWndStop(m_hMciWnd2);
		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}	

	if(m_hMciWnd3)
	{
		MCIWndStop(m_hMciWnd3);
		MCIWndDestroy(m_hMciWnd3);
		m_hMciWnd3=NULL;
	}
}

//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClassInExe::DoDataExchange(pDX);
	DoDataExchangeWebVirFace(pDX);
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClassInExe::OnInitDialog();

	m_PlayView.m_showcoredlg.m_ucomtype=GetComType();
	//���±���
	SetDialogIcon(LoadIcon(GetModuleHandle(GET_CLIENT_DLL_NAME(szTempStr)),MAKEINTRESOURCE(IDI_CLIENT_ROOM)),TRUE);
	m_bRightPower=GetProfileInt(TEXT("RightPower"),FALSE);

	/////////////////090311a1 JJ ��Ӷ�ȡ�����ļ������ÿ�������ʱ���������Զ���ʼ�����Զ��뿪
	CString s = CBcfFile::GetAppPath ()+SKIN_FOLDER+_T(".bcf");	//����·��
	//CString s = CINIFile::GetAppPath ();/////����·��
	//CBcfFile f( s +SKIN_FOLDER +".bcf");
	if (CBcfFile::IsFileExist(s))
	{
		CBcfFile f(s);
		m_iAfterWaitAction = f.GetKeyVal(_T("GameSet"), _T("AfterWaitAction"), 1);
		if (m_iAfterWaitAction != 1 && m_iAfterWaitAction != 2)
		{
			m_iAfterWaitAction = 1;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	bInitFinish = true;
	//BeginAnimal(ANIMAL_SBOMB0);
	//BeginAnimal(ANIMAL_SBOMB1);
	//BeginAnimal(ANIMAL_SBOMB2);
	/*BYTE card[2] = {0};
	card[0] = 0x0D;
	card[1] = 0x4E;
	BeginAnimal(ANIMAL_BBOMB0, card, 2);*/
	//BeginAnimal(ANIMAL_DZWIN);
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//��������
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//����ʱ̬��Ϣ
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;

			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
			//��ʾ���ô���
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);

			//CString s;
			//s.Format("CUsingSkinClass size in game:%d" ,sizeof(CUsingSkinClass));
			//AfxMessageBox(s);
			if(!m_bWatchMode)
			{
// 				m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
// 				//m_PlayView.m_btStartDisplayCards.ShowWindow(!m_bWatchMode);
// 				if(m_PlayView.m_btLeave.m_hWnd) 
// 				{
// 					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
// 				}
// 				m_PlayView.m_btContinue.SetFocus();
// 
// 				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
				if (!IsQueueGameRoom()) 
				{ 
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					if(m_PlayView.m_btLeave.m_hWnd)
						m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
				} 
				else
				{
					ResetGameStation(RS_GAME_BEGIN);
				}

#ifdef AI_MACHINE
				SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif			
				m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
			}
			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
	case GS_WAIT_BACK:	//�ȴ������״̬
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//��������
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//����ʱ̬��Ϣ
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;

			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			//���ý���
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
            //m_PlayView.m_btStartDisplayCards.ShowWindow(SW_HIDE);
			if(m_PlayView.m_btLeave.m_hWnd) 
			{
				m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
			}

			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btThing.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.ShowWindow(!m_bWatchMode);

			m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);

			m_PlayView.m_bFirstGame = FALSE;
			//���õ�ǰ�й����
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				m_PlayView.SetAuto(ViewStation(i),pGameStation->bAuto[i]);
			}

			if(!m_bWatchMode)
				m_bAutoCard = pGameStation->bAuto[GetMeUserInfo()->bDeskStation];
			//if(m_bAutoCard)
			//	m_PlayView.m_btAuto.LoadButtonBitmap(".\\image\\ddz\\Auto_less.bmp",false);
			//else 
			//	m_PlayView.m_btAuto.LoadButtonBitmap(".\\image\\ddz\\Auto.bmp",false); 
			//�޸ĸ��û����е���
			int iPos = 0;
			for(int i = 0;i < PLAY_COUNT; i ++)
			{
				m_CardCount[ViewStation(i)] = pGameStation->iUserCardCount[i];
				::CopyMemory(m_Card[ViewStation(i)],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[ViewStation(i)]);
				iPos+=m_CardCount[ViewStation(i)];
				m_Logic.SortCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);

				m_PlayView.m_HandCard[ViewStation(i)].SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);

				if(i == GetMeUserInfo()->bDeskStation)
				{
					::CopyMemory(m_iMeCardList,m_Card[ViewStation(i)],m_CardCount[ViewStation(i)]);
					m_iMeCardCount=m_CardCount[ViewStation(i)];
					m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
					m_PlayView.m_MeCard.SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				}
			}
#ifdef MY_TWO
			m_PlayView.m_ThreeCard.SetCard(NULL,NULL,17);
#endif
			m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
			m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
			switch(pGameStation->iGameFlag)
			{
			case GS_FLAG_CALL_SCORE:
				{
					SetGameTimer(pGameStation->iCallScorePeople,m_iThinkTime,ID_CALL_SCORE_TIME);		//�з�ʱ������
					m_PlayView.SetOrientation(ViewStation(pGameStation->iCallScorePeople));

					for(int i = 0;i < PLAY_COUNT; i ++)
						m_PlayView.SetCallScoreValue(ViewStation(i),pGameStation->iCallScore[i]);
					if(pGameStation->iCallScorePeople == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
						ShowCallScoreBt(pGameStation->iCallScoreResult);
					break;
				}
			case GS_FLAG_ROB_NT:
				{
					break;
				}
			case GS_FLAG_ADD_DOUBLE:
				{
					break;
				}
			case GS_FLAG_SHOW_CARD:
				{
					break;
				}
			}
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//��������
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//����ʱ̬��Ϣ
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			//	m_iBeginNt = pGameStation->iBeginNt;
			//m_iEndNt = pGameStation->iEndNt;
			//������Ϸ��Ϣ
			//m_Logic.SetTwoIsNT(pGameStation->bTwoIsNt);
			//m_Logic.SetHaveKing(pGameStation->bHaveKing);
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.SetGameMultiple((int)pow(2.0,pGameStation->iBase));

			m_iNowOutPeople = pGameStation->iOutCardPeople;
			m_iFirstOutPeople = pGameStation->iFirstOutPeople;
			m_iBigOutPeople = pGameStation->iBigOutPeople;

			m_PlayView.m_bFirstGame = FALSE;
			//���ý��水ť
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			//m_PlayView.m_btStartDisplayCards.ShowWindow(SW_HIDE);
			if(m_PlayView.m_btLeave.m_hWnd) 
			{
				m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
			}
			m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);

			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btThing.EnableWindow(!m_bWatchMode&&(GetComType()!=2));
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.ShowWindow(!m_bWatchMode);
#ifdef MY_TWO
			m_PlayView.m_ThreeCard.SetCard(NULL,NULL,17);
#endif
			//���õ���
			m_PlayView.SetNTStation(ViewStation(pGameStation->iUpGradePeople));
			//�����й����
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				m_PlayView.SetAuto(ViewStation(i),pGameStation->bAuto[i]);				//�й�
				m_PlayView.SetAwardPoint(ViewStation(i),pGameStation->iAwardPoint[i]);	//����
				m_PlayView.SetPeopleMultiple(ViewStation(i),pGameStation->iPeopleBase[i]);
			}
			if(!m_bWatchMode)
				m_bAutoCard = pGameStation->bAuto[GetMeUserInfo()->bDeskStation];

			m_PlayView.SetCallScoreResult(pGameStation->iCallScoreResult);		//���ýзֽ��

			//�����˿�

			int iPos = 0;
			for(int i = 0; i < PLAY_COUNT; i ++)
			{	//�����û���
				m_CardCount[ViewStation(i)] = pGameStation->iUserCardCount[i];
				::CopyMemory(m_Card[ViewStation(i)],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[ViewStation(i)]);
				iPos += m_CardCount[ViewStation(i)];
				m_Logic.SortCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				m_PlayView.m_HandCard[ViewStation(i)].SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				//�Լ���
				if(i == GetMeUserInfo()->bDeskStation)
				{
					::CopyMemory(m_iMeCardList,m_Card[ViewStation(i)],m_CardCount[ViewStation(i)]);
					m_iMeCardCount = m_CardCount[ViewStation(i)];
					m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);

					m_PlayView.m_MeCard.SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				}

				//��������
				m_iDeskCardCount[i] = pGameStation->iDeskCardCount[i];
				::CopyMemory(m_DeskCard[i],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_iDeskCardCount[i]);
				iPos += m_iDeskCardCount[i];
				m_Logic.SortCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);
				m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);

				m_PlayView.m_HandCard[ViewStation(i)].SetShowCard(pGameStation->iArrIsDisplayCard[i]);//�������ƿ�ʼ����ҵ�����״̬
			}
			//��������״̬����
			if(GetMeUserInfo()->bDeskStation != pGameStation->iUpGradePeople)
			{
				m_PlayView.m_HandCard[ViewStation(pGameStation->iUpGradePeople)].SetShowCard(pGameStation->iRobNT[pGameStation->iUpGradePeople] >= 2);
			}
			//�������һ���������
			m_iBaseCount = pGameStation->iBaseOutCount;
			::CopyMemory(m_iBaseCard,&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_iBaseCount);
			
			iPos += m_iBaseCount;
			//m_Logic.SortCard(&pGameStation->iUserCardList[iPos],NULL,sizeof(BYTE)*pGameStation->iBackCardCount);
			m_iBackCount = pGameStation->iBackCardCount;
			::CopyMemory(m_iBackCard,&pGameStation->iUserCardList[iPos],sizeof(BYTE)*pGameStation->iBackCardCount);
			m_PlayView.m_MeBackCard.SetCard(&pGameStation->iUserCardList[iPos],NULL,sizeof(BYTE)*pGameStation->iBackCardCount);
			m_PlayView.m_MeBackCard.SetShowCard(true);
			m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);

			iPos += m_iBackCount;

			m_Logic.SetLai(m_iBackCard[1]);
			m_PlayView.SetLai(m_iBackCard[1]);
			m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
			m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);

			//{{�޸���ʾ����
			BYTE iReplaceCardList[45];
			for (int i=0; i<PLAY_COUNT; ++i)
			{
				memset(&m_Replace,0,sizeof(m_Replace));
				m_Logic.GetCardReplaceShape(m_DeskCard[i],m_iDeskCardCount[i],m_Replace) ;

				if (m_Logic.GetReplaceCardCount(m_DeskCard[i],m_iDeskCardCount[i]) &&
					m_Logic.GetReplaceCardCount(m_DeskCard[i],m_iDeskCardCount[i]) != m_iDeskCardCount[i])
				{
					::CopyMemory(iReplaceCardList, m_Replace.ReplaceList[0], sizeof(BYTE)*m_iDeskCardCount[i]);
					if (m_Replace.ReplaceListShape[0] == UG_BOMB)
					{
						::CopyMemory(iReplaceCardList, m_DeskCard[i], sizeof(BYTE)*m_iDeskCardCount[i]);
					}
				}
				else
				{
					continue;
				}

				//::CopyMemory(m_iBaseCard,iReplaceCardList,sizeof(BYTE)*m_iBaseCount);
				if (4 == m_iDeskCardCount[i])//����4����ʱ
				{
					if (m_Logic.IsShamBomb(iReplaceCardList,m_iDeskCardCount[i]))//������ը
					{
						iReplaceCardList[0] = 48 - m_Logic.GetCardHuaKind(iReplaceCardList[3]) + iReplaceCardList[3];
						iReplaceCardList[1] = iReplaceCardList[0] - 16;
						iReplaceCardList[2] = iReplaceCardList[1] - 16;
						iReplaceCardList[3] = iReplaceCardList[2] - 16;
						m_PlayView.m_bIsFakeBomb = TRUE;
					}

					else//��������һʱ
					{ 

						BYTE byteTempCardVal;//��ʱ���Ƶ�ֵ 
						BYTE byteTempArr[20] = {0};//��ʱ����
						BYTE byteVal1;//����һ��"һ"���Ƶ�ֵ

						for (int k=0; k<4; k++)//ȡ����һ�е�"��"����
						{
							byteTempArr[m_Logic.GetCardNum(iReplaceCardList[k])]++;
						}
						for (int k=0; k<20; k++)//ȡ����һ�е�"��"���ֵ��Ƶ�ֵ
						{
							if (3 == byteTempArr[k])
							{
								byteTempCardVal = k;
							}
						}
						for (int k=0; k<4; k++)//ȡ����һ�е�"һ"�������±겢��ֵ
						{
							if (byteTempCardVal != m_Logic.GetCardNum(iReplaceCardList[k]))
							{
								byteVal1 = k;
							}
						}

						iReplaceCardList[3] = iReplaceCardList[byteVal1];
						if (m_Logic.GetCardNum(iReplaceCardList[3]) == m_Logic.GetCardNum(m_PlayView.m_bLaiCardList[0]))
						{
							iReplaceCardList[3] = iReplaceCardList[3] - 16;
						}
						iReplaceCardList[0] = 48 - m_Logic.GetCardHuaKind(iReplaceCardList[!byteVal1]) 
							+ iReplaceCardList[!byteVal1];
						iReplaceCardList[1] = iReplaceCardList[0] - 16;
						iReplaceCardList[2] = iReplaceCardList[1] - 16;
					}
				}
				::CopyMemory(m_DeskCard[i],iReplaceCardList,sizeof(BYTE)*m_iDeskCardCount[i]);
				m_PlayView.m_bIsFakeBomb = TRUE;

				m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);
			}
			//}}�޸���ʾ����

			m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);

			//��ʱ��
			if (m_iNowOutPeople != -1)
			{
				SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
				m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));
			}
			//�ж��Ƿ��Լ�����	
			if (!m_bWatchMode && (m_iNowOutPeople == GetMeUserInfo()->bDeskStation))
			{
				//�Լ���ǰ�������
				m_PlayView.m_UserCard[1].SetCard(0,NULL,0);
				m_PlayView.m_btPass.ShowWindow(m_iNowOutPeople != m_iBigOutPeople);
				m_PlayView.m_btPass.EnableWindow(m_iNowOutPeople != m_iBigOutPeople);

				m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
				m_PlayView.m_btCue.ShowWindow(SW_SHOW);
				m_PlayView.m_btCue.EnableWindow(TRUE);
				OnHitCard(0,0);
				if(m_bAutoCard )
					SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);

#ifdef AI_MACHINE
				SetGameTimer(m_iNowOutPeople,(rand() % GAME_RAND_TIME)+1,ID_OUT_CARD);
#endif
			}

			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//��������
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//����ʱ̬��Ϣ
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;
			//	m_iBeginNt = pGameStation->iBeginNt;
			//m_iEndNt = pGameStation->iEndNt;
			//������Ϸ��Ϣ
			//m_Logic.SetTwoIsNT(pGameStation->bTwoIsNt);
			//m_Logic.SetHaveKing(pGameStation->bHaveKing);
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
			//��ʾ���ô���
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);
			m_PlayView.SetGameMultiple(1);

			//��ʾ��ʼ��ť 
			if(!m_bWatchMode)
			{
				if (!IsQueueGameRoom()) 
				{ 
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					if(m_PlayView.m_btLeave.m_hWnd)
						m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_BEGIN_TIME);
				} 
				else
				{
					ResetGameStation(RS_GAME_BEGIN);
				}

    			//m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
				//	if(m_PlayView.m_btLeave.m_hWnd) 
				//{
			   //		m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
			   //	}
				m_PlayView.m_btContinue.SetFocus();
				//�������Զ���ʼ			
				//if(2==GetComType())
				//{
				//	SetGameTimer(GetMeUserInfo()->bDeskStation,GAME_AUTO_TIME,ID_BEGIN_TIME);
				//}
				//else
				//SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
#ifdef AI_MACHINE
				SetGameTimer(GetMeUserInfo()->bDeskStation,(rand() % GAME_RAND_TIME)+1,ID_BEGIN_TIME);
#endif
				m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
			}
			return TRUE;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY&&bInitFinish)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				if (uDataSize!=sizeof(UserArgeeGame)) 
					return FALSE;
				SetStationParameter(GS_WAIT_ARGEE);
				UserArgeeGame * pGameSet=(UserArgeeGame *)pNetData;
				return TRUE;
			}
		case ASS_GAME_BEGIN:		//��Ϸ��ʼ
			{				
				if (uDataSize != sizeof(BeginUpgradeStruct))
					return FALSE;
				BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)pNetData;
				m_bTimeOutCount = 0;
				m_btAutoTime = 0;
				m_iBeenPlayGame ++;
				SetStationParameter(GS_SEND_CARD);
				m_PlayView.m_GameNoteView.SetMessage(NULL);

				if (m_bWatchMode == TRUE) 
				{
					KillAllTimer();
					ResetGameStation(RS_GAME_BEGIN);
				}
				m_PlayView.m_btAuto.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btThing.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btSortCard.EnableWindow(!m_bWatchMode);

				m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
				m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
		
				return TRUE;
			}
		case ASS_GAME_PREPARE:	//����һ����
			{
				if (uDataSize!=sizeof(PrepareStruct)) 
					return FALSE;
				PrepareStruct *pPrepare = (PrepareStruct *)pNetData;;
		
			//	m_PlayView.m_MeBackCard.SetCard(&pPrepare->bCard,NULL,sizeof(BYTE)*1);
				m_PlayView.m_MeBackCard.SetShowCard(TRUE);
				m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);

				return TRUE;
			}
		case ASS_SEND_CARD:			//����������
			{
				if (uDataSize!=sizeof(SendCardStruct)) 
					return FALSE;
				SendCardStruct * pSendCard=(SendCardStruct *)pNetData;

				int desk = ViewStation(pSendCard->bDeskStation);

				if(pSendCard->bDeskStation != GetMeUserInfo()->bDeskStation)
				{
					m_Card[desk][m_CardCount[desk]++] = pSendCard->bCard;
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
				    return true;
				}
				m_iMeCardList[m_iMeCardCount++] = pSendCard->bCard;
				m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
				m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
#ifdef MY_TWO
				m_PlayView.m_ThreeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
#endif
				m_PlayView.UpdateViewFace(NULL);

				if(m_pGameInfo->bEnableSound)		//����
				{
					VoiceOut(255,"OUT_CARD.wav");//SetPlaySound(SOUND_SEND_CARD);
				}
				return TRUE;
			}
		case ASS_SEND_ALL_CARD:			//һ�ν�һ���˵���ȫ������
			{
				if (uDataSize!=sizeof(SendAllStruct)) 
					return FALSE;
				SendAllStruct * pSendAll=(SendAllStruct *)pNetData;
				int iPos = 0;
				for(int i = 0;i < PLAY_COUNT; i ++)
				{
					int desk = ViewStation(i);
					m_CardCount[desk] = pSendAll->iUserCardCount[i];
					::CopyMemory(m_Card[desk],&pSendAll->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[desk]);
					iPos += m_CardCount[desk];
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);

					if(i == GetMeUserInfo()->bDeskStation)
					{
						::CopyMemory(m_iMeCardList,m_Card[desk],m_CardCount[desk]);
						m_iMeCardCount = m_CardCount[desk];
						m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
						m_PlayView.m_MeCard.SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					}
				}
#ifdef MY_TWO
				m_PlayView.m_ThreeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
#endif

				if(m_pGameInfo->bEnableSound)		//����
				{
					VoiceOut(255,"OUT_CARD.wav");//SetPlaySound(SOUND_SEND_CARD);
	
				}
				return TRUE;
			}
		case ASS_SEND_CARD_MSG://������Ϣ
			{
				if(uDataSize!=sizeof(SendCardStruct))
					return FALSE;
				SendCardStruct *pCardMsg = (SendCardStruct*)pNetData;
				pCardMsg->bDeskStation ;
				wsprintf(m_CueMessage,"��� %s ���Ƚе���",m_pUserInfo[pCardMsg->bDeskStation ]->GameUserInfo.nickName);
				m_MessageHandle.InsertNormalMessage(m_CueMessage);
			//	m_PlayView.m_UserCard[ViewStation(pCardMsg->bDeskStation)].SetCard(&pCardMsg->bCard,NULL,sizeof(BYTE)*1);
				return TRUE;

			}
		case ASS_SEND_FINISH:		//�������
			{
				byte card[]={0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,0x0B,0x0C,0x0D,0x0E};		//12��
				m_PlayView.m_MeBackCard.SetShowCard(0);	
				m_PlayView.m_MeBackCard.SetCard(card,NULL,m_iBackCount);
				m_PlayView.m_MeBackCard.ShowWindow(1);
				for(int i = 0; i < PLAY_COUNT; i ++)
					m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_START.WAV");//SetPlaySound(SOUND_START_DEF);
				return TRUE;
			}
		case ASS_CALL_SCORE:			//�з�
			{	
				SetStationParameter(GS_WAIT_BACK);
				CallScoreStruct *score=(CallScoreStruct *)pNetData;	
				m_bCurrentOperationStation = score->bDeskStation;
				if(m_bCurrentOperationStation != 255)
				{
					SetGameTimer(m_bCurrentOperationStation,m_iCallScoreTime,ID_CALL_SCORE_TIME);
					m_PlayView.SetOrientation(ViewStation(m_bCurrentOperationStation));
				}

				if(!m_bWatchMode && GetMeUserInfo()->bDeskStation == score->bDeskStation )
				{
					ShowCallScoreBt(score->iValue);
					//if(m_pGameInfo->bEnableSound)		//����
					//SetPlaySound(-1,SOUND_CALL_SCORE);
					if(m_bAutoCard)
						SetGameTimer(m_bCurrentOperationStation,GAME_AUTO_TIME,ID_CALL_SCORE_TIME);
#ifdef AI_MACHINE
					int result = rand()%4;
					if(result > score->iCallScore && result > 0 && result <= 3)
						;
					else 
						result = 0;
					OnCallScore(result,result);
#endif 
				}
				return true;
			}
		case ASS_CALL_SCORE_RESULT:			//�зֽ��
			{
				if (uDataSize != sizeof(CallScoreStruct)) 
					return FALSE;
				KillTimer(ID_CALL_SCORE_TIME);
				HideButton();
				CallScoreStruct *score=(CallScoreStruct*)pNetData;
				m_PlayView.SetCallScoreValue(ViewStation(score->bDeskStation),score->iValue);		//��ʾ���з�״��
				if(m_pGameInfo->bEnableSound)
				{
					/*switch(score->iValue)
					{
					case 0:
						VoiceOut(score->bDeskStation,"human_Jiaofen0.mp3");
						
						break;
					case 10:
						VoiceOut(score->bDeskStation,"human_jiaofen1.mp3");
						break;
					case 20:
						VoiceOut(score->bDeskStation,"human_jiaofen2.mp3");
						break;
					case 30:
						VoiceOut(score->bDeskStation,"human_jiaofen3.wav");
						break;
					default:
						VoiceOut(255,"GAME_WARN.WAV");
						break;
					}*/
					SetSceneSound(SOUND_CALL_SCORE,*this,m_pUserInfo[score->bDeskStation]->GameUserInfo.bBoy,score->iValue);
				}
				return true;
			}
		case ASS_CALL_SCORE_FINISH:
			{
				if (uDataSize != sizeof(CallScoreStruct)) 
					return FALSE;
				KillTimer(ID_CALL_SCORE_TIME);
				HideButton();

				//�������з����
				for(int i = 0; i < PLAY_COUNT; i ++)
					m_PlayView.SetCallScoreValue(i, -1);		

				m_bCurrentOperationStation = 255;

				CallScoreStruct *score=(CallScoreStruct*)pNetData;

				int iViewStation = ViewStation(score->bDeskStation);

				m_PlayView.SetNTStation(iViewStation);
				m_PlayView.SetCallScoreResult(score->iValue);

				////������@ʾ
				//if(m_pUserInfo[score->bDeskStation] != NULL)
				//{
				//	TCHAR szMessage[100];
				//	wsprintf(szMessage,TEXT("[��ʾ]�зֽ��: ����Ϊ%s,������ %d ��!"),m_pUserInfo[score->bDeskStation]->GameUserInfo.nickName,score->iValue);
				//	m_MessageHandle.InsertNormalMessage(szMessage);
				//}
				return true;
			}
		case ASS_ROB_NT:	//������
			{
				if (uDataSize != sizeof(RobNTStruct)) 
					return FALSE;
				RobNTStruct * pRobNT=(RobNTStruct *)pNetData;
				m_bCurrentOperationStation = pRobNT->bDeskStation ;

				if(m_bCurrentOperationStation != 255)
				{
					SetGameTimer(m_bCurrentOperationStation,m_iCallScoreTime,ID_ROB_NT_TIME);

					m_PlayView.SetOrientation(ViewStation(m_bCurrentOperationStation));
				}

				//����������
				if(GetMeUserInfo()->bDeskStation == pRobNT->bDeskStation )
				{
					m_PlayView.m_btRobNT.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btRobNT.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btRobNT2.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btRobNT2.EnableWindow(!m_bWatchMode);
#ifdef AI_MACHINE
					SetGameTimer(m_bCurrentOperationStation,(rand() % GAME_RAND_TIME)+1,ID_ROB_NT_TIME);
#endif			//if(m_bAutoCard)
					//	SetGameTimer(m_bCurrentOperationStation,GAME_AUTO_TIME,ID_ROB_NT_TIME);
				}

				return TRUE;
			}
		case ASS_ROB_NT_RESULT://���������
			{
				if (uDataSize != sizeof(RobNTStruct)) 
					return FALSE;

				RobNTStruct * pRobNTResult=(RobNTStruct *)pNetData;

				/////////////////�޸ļӱ����� (090402b1 JJ)
				//if(m_pGameInfo->bEnableSound)		//����
				//	SetPlaySound(SOUND_ROB_NT);
				if(m_pGameInfo->bEnableSound)		//����
				{
					if (pRobNTResult->iValue == 1)
					{
						if (m_pUserInfo[pRobNTResult->bDeskStation])
							SetDdzSound(SOUND_ROB_NT,m_hWnd,m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.bBoy);
						else
							SetDdzSound(SOUND_ROB_NT,m_hWnd);
					}
					else
					{	
						if (m_pUserInfo[pRobNTResult->bDeskStation])
							SetDdzSound(SOUND_ROB_NT_NOT,m_hWnd,m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.bBoy);
						else
							SetDdzSound(SOUND_ROB_NT_NOT,m_hWnd);
					}
				}
				////////////////////////////////////////end
				/*				if(m_pUserInfo[pRobNTResult->bDeskStation] != NULL&& pRobNTResult->iValue)
				{
				TCHAR szMessage[200];
					wsprintf(szMessage,"[��ʾ] %s ������!",m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(szMessage);
				}*/	
				return TRUE;
			}
		case ASS_GAME_MULTIPLE:
			{
				if (uDataSize!=sizeof(AwardPointStruct)) 
					return FALSE;
				AwardPointStruct *awardpoint=(AwardPointStruct*)pNetData;

				m_PlayView.SetGameMultiple((int)pow(2.0,awardpoint->iBase));
				return TRUE;
			}
		case ASS_ROB_NT_FINISH:		//����������
			{
				if (uDataSize!=sizeof(RobNTStruct)) 
					return FALSE;

				RobNTStruct * pRobNTResult=(RobNTStruct *)pNetData;

				if(m_pUserInfo[pRobNTResult->bDeskStation] != NULL)
				{
					TCHAR szMessage[200];
					wsprintf(szMessage,"[��ʾ] %s �ɹ���������!",m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(szMessage);
					m_PlayView.SetNTStation(ViewStation(pRobNTResult->bDeskStation));
				}
			
				return true;
			}
		case ASS_BACK_CARD_EX:		//��չ��������
			{
				if (uDataSize!=sizeof(BackCardExStruct)) 
					return FALSE;
				BackCardExStruct * pBackCard=(BackCardExStruct *)pNetData;
				//�@ʾ����
				m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);		//��ʾ����
				m_PlayView.m_MeBackCard.SetCard(pBackCard->iBackCard,NULL,pBackCard->iBackCardCount);
				m_Logic.SetLai(pBackCard->iBackCard[1]);
				m_PlayView.SetLai(pBackCard->iBackCard[1]);
				m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
				m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
				//��������
				m_iNtPeople = pBackCard->iGiveBackPeople;
				if(m_iNtPeople != GetMeUserInfo()->bDeskStation)
				{
					int desk = ViewStation(m_iNtPeople);
					::CopyMemory(&m_Card[desk][m_CardCount[desk]],pBackCard->iBackCard,sizeof(BYTE)*(pBackCard->iBackCardCount));
					m_CardCount[desk] += pBackCard->iBackCardCount;
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					return true;
				}				
				//���ý���
				m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
				if (/*!m_bWatchMode &&*/ GetMeUserInfo()->bDeskStation == pBackCard->iGiveBackPeople)
				{
					BYTE bUp[45];
					::memset(bUp,0,sizeof(bUp));
					::CopyMemory(&m_iMeCardList[m_iMeCardCount],pBackCard->iBackCard,sizeof(BYTE)*pBackCard->iBackCardCount);
					m_iMeCardCount += pBackCard->iBackCardCount;
					m_Logic.SortCard(m_iMeCardList,bUp,m_iMeCardCount);
					m_PlayView.m_MeCard.SetCard(m_iMeCardList,bUp,m_iMeCardCount);
				}
				return TRUE;
			}

		case ASS_ADD_DOUBLE://�Ӱ�
			{
				if (uDataSize!=sizeof(AddDoubleStruct)) 
					return FALSE;
				AddDoubleStruct * pAddDouble=(AddDoubleStruct *)pNetData;

				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iAddDoubleTime,ID_ADD_DOUBLE_TIME);

				//�Ӱ�
				if(GetMeUserInfo()->bDeskStation != pAddDouble->bDeskStation &&!m_bWatchMode)
				{
					m_PlayView.m_btAddDouble.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btAddDouble2.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btAddDouble.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btAddDouble2.EnableWindow(!m_bWatchMode);

					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
#ifdef AI_MACHINE
					OnAddDouble(rand()%2,rand()%2);
#endif			
				}				
				return TRUE;
			}
		case ASS_ADD_DOUBLE_RESULT://�Ӱ����
			{
				if (uDataSize!=sizeof(AddDoubleStruct)) 
					return FALSE;
				KillTimer(ID_ADD_DOUBLE_TIME);
				AddDoubleStruct * pAddDoubleResult=(AddDoubleStruct *)pNetData;
				//���Ӹ��˱���
				m_PlayView.SetPeopleMultiple(ViewStation(pAddDoubleResult->bDeskStation),pAddDoubleResult->iValue);
				//���üӰ�����
				/////////////////�޸ļӱ����� (090402b1 JJ)
				//if(m_pGameInfo->bEnableSound)		//����
				//	SetPlaySound(SOUND_ADD_DOUBLE);
				if(m_pGameInfo->bEnableSound)		//����
				{
					if (pAddDoubleResult->iValue == 1)
					{
						if (m_pUserInfo[pAddDoubleResult->bDeskStation])
							SetDdzSound(SOUND_ADD_DOUBLE,m_hWnd,m_pUserInfo[pAddDoubleResult->bDeskStation]->GameUserInfo.bBoy);
						else
							SetDdzSound(SOUND_ADD_DOUBLE,m_hWnd);
					}
					else
					{	
						if (m_pUserInfo[pAddDoubleResult->bDeskStation])
							SetDdzSound(SOUND_ADD_DOUBLE_NOT,m_hWnd,m_pUserInfo[pAddDoubleResult->bDeskStation]->GameUserInfo.bBoy);
						else
							SetDdzSound(SOUND_ADD_DOUBLE_NOT,m_hWnd);
					}
				}
				////////////////////////////////////////end

				m_PlayView.m_btAddDouble.ShowWindow(SW_HIDE);
				m_PlayView.m_btAddDouble2.ShowWindow(SW_HIDE);

				return true;
			}
		case ASS_ADD_DOUBLE_FINISH:
			{
				m_PlayView.m_MeBackCard.SetShowCard(true);
				KillTimer(ID_ADD_DOUBLE_TIME);
				m_PlayView.m_btAddDouble.ShowWindow(SW_HIDE);
				m_PlayView.m_btAddDouble2.ShowWindow(SW_HIDE);
				return true;
			}
		case ASS_SHOW_CARD://����
			{
				if (uDataSize!=sizeof(ShowCardStruct)) 
					return FALSE;
				ShowCardStruct  * pShow = (ShowCardStruct*)pNetData;
				SetGameTimer(pShow->bDeskStation,m_iAddDoubleTime,ID_SHOW_CARD);
				m_PlayView.SetOrientation(ViewStation(pShow->bDeskStation));
				
				m_bCurrentOperationStation = pShow->bDeskStation ;

				if(!m_bWatchMode && GetMeUserInfo()->bDeskStation == pShow->bDeskStation && !m_bIsStartDisplay) //�Ƿ�Ϊ������ʾ���ư�ť
				{
					m_PlayView.m_btShowCard.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btShowCard2.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btShowCard.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btShowCard2.EnableWindow(!m_bWatchMode);
				}

				if (!m_bWatchMode && m_bIsStartDisplay)  //�������ѡ�����ƿ�ʼʱ
				{
					m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
					m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);
					m_PlayView.m_btShowCard.EnableWindow();
					m_PlayView.m_btShowCard2.EnableWindow();
					OnShowCard(1,1); //����
				}


				return TRUE;
			}
		case ASS_SHOW_CARD_RESULT: //�ӷ���˴��ص����ƽ��
			{
				if (uDataSize!=sizeof(ShowCardStruct)) 
					return FALSE;
				KillTimer(ID_SHOW_CARD);
				m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
				m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);
				ShowCardStruct  * pShow = (ShowCardStruct*)pNetData;
				if(pShow->iValue)
				{
					m_PlayView.m_HandCard[ViewStation(pShow->bDeskStation)].SetShowCard(TRUE);
					TCHAR szMessage[200];
					wsprintf(szMessage, "%s ����,��Ϸ����x2", m_pUserInfo[pShow->bDeskStation]->GameUserInfo.nickName);
					if (m_PlayView.m_iNtStation == ViewStation(pShow->bDeskStation))
					{
						wsprintf(szMessage, "��������,��Ϸ����x2");
					}
					m_MessageHandle.InsertNormalMessage(szMessage);
				}
				////////////////////////////////090402b1 JJ�����������
				if(m_pGameInfo->bEnableSound)		//����
				{
					if (pShow->iValue == 1)
					{
						if (m_pUserInfo[pShow->bDeskStation])
							SetDdzSound(SOUND_SHOW_CARD,m_hWnd,m_pUserInfo[pShow->bDeskStation]->GameUserInfo.bBoy);
						else
							SetDdzSound(SOUND_SHOW_CARD,m_hWnd);
					}
				}
				///////////////////////////////////////////////////end
				return TRUE;
			}
		case ASS_SHOW_CARD_FINISH:
			{
				return true;
			}
		case ASS_GAME_PLAY:		//��ʼ������Ϸ
			{
				//����ۿ�,�����й�
				if (uDataSize!=sizeof(BeginPlayStruct)) 
					return FALSE;

				m_byteHitPass = 0;

				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;
				m_bTimeOutCount=0;
				SetStationParameter(GS_PLAY_GAME);
				m_iBigOutPeople = m_iNowOutPeople=m_iFirstOutPeople = pBeginInfo->iOutDeskStation;
				if(m_iNowOutPeople != -1)
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));
				}	

				if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation==m_iNowOutPeople))
				{
					m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.EnableWindow(false);			//��һ��������һ���ó���
					m_PlayView.m_btPass.ShowWindow(SW_HIDE);
					m_PlayView.m_btCue.ShowWindow(SW_SHOW);
					m_PlayView.m_btCue.EnableWindow(TRUE);

					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

					OnHitCard(0,0);
					if(m_bAutoCard)
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,(rand()%GAME_RAND_TIME )+ 1,ID_OUT_CARD);
#endif 					
				}
				//BOOL bUpGrade=((GetMeUserInfo()->bDeskStation==m_iNowOutPeople)||
				//	((GetMeUserInfo()->bDeskStation+2)%4==m_iNowOutPeople));
				//				m_PlayView.SetGamePoint(0,!bUpGrade);
				for (int i = 0; i < PLAY_COUNT; i++) 
					m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				m_PlayView.m_GameNoteView.SetMessage(NULL);


				//SetPlaySound(-1,SOUND_GAME_BEGIN);
				return TRUE;
			}
		case ASS_OUT_CARD_RESULT:  //���ƺ�ӷ���˴��ص�������
			{
				OutCardMsg * pOutCardInfo=(OutCardMsg *)pNetData;
				KillAllTimer();
				m_PlayView.SetOrientation(-1);

				m_iDeskCardCount[pOutCardInfo->bDeskStation] = pOutCardInfo->iCardCount;
				::CopyMemory(m_DeskCard[pOutCardInfo->bDeskStation],pOutCardInfo->iCardList,sizeof(BYTE)*pOutCardInfo->iCardCount);

				//��������
				m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
				m_PlayView.m_btPass.ShowWindow(SW_HIDE);
				m_PlayView.m_btCue.ShowWindow(SW_HIDE);
				m_PlayView.m_SelectLaiZhi.ShowWindow(SW_HIDE);


				//Ϊ��һ��������
				if(pOutCardInfo->iCardCount > 0)
				{
					if(m_pGameInfo->bEnableSound)
					{
						switch(m_Logic.GetCardShape(pOutCardInfo->iCardList,pOutCardInfo->iCardCount))
						{
						case UG_ONLY_ONE:
							{
								TCHAR szTemp[20];
								ZeroMemory(szTemp,sizeof(szTemp));
								int iTemp=m_Logic.GetCardNum(pOutCardInfo->iCardList[0]);
								if(iTemp==14)
								{
									iTemp=1;
								}else if(iTemp>14)
								{
									iTemp--;
								}
								wsprintf(szTemp,"human_%d.wav",iTemp);
								VoiceOut(pOutCardInfo->bDeskStation,szTemp);
							}
							break;
						case UG_DOUBLE:
							{
								TCHAR szTemp[20];
								ZeroMemory(szTemp,sizeof(szTemp));
								int iTemp=m_Logic.GetCardNum(pOutCardInfo->iCardList[0]);
								if(iTemp==14)
								{
									iTemp=1;
								}else if(iTemp>14)
								{
									VoiceOut(pOutCardInfo->bDeskStation,"OUT_CARD.wav");
									break;
								}
								wsprintf(szTemp,"human_DB_%d.wav",iTemp);
								VoiceOut(pOutCardInfo->bDeskStation,szTemp);								

							}
							break;
						case UG_STRAIGHT:
						case UG_VARIATION_STRAIGHT:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_LINE.wav");
							}
							break;
						case UG_DOUBLE_SEQUENCE:
						case UG_VARIATION_DOUBLE_SEQUENCE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_DB_LINE.wav");
							}
							break;
						case UG_FOUR_TWO:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_RECT_ONE.wav");
							}
							break;
						case UG_FOUR_TWO_DOUBLE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_RECT_DB.wav");
							}
							break;
						case UG_THREE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_TRI.wav");
							}
							break;
						case UG_THREE_ONE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_TRI_ONE.wav");
							}
							break;
						case UG_THREE_DOUBLE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_TRI_DB.wav");
							}
							break;
						case UG_BOMB:
						case UG_SHAM_BOMB:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_MOST_CARD.wav");
							
							}
							break;
						case UG_KING_BOMB:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_MISSILE.wav");
							
							}
							break;
						default:
							VoiceOut(255,"OUT_CARD.wav");
							break;
						}
						//int ishape = m_Logic.GetCardShape(pOutCardInfo->iCardList, pOutCardInfo->iCardCount);
						//if (ishape == UG_ONLY_ONE || ishape == UG_DOUBLE)
						//{
						//	BYTE cardNum = 0;
						//	if ( ishape == UG_ONLY_ONE && (pOutCardInfo->iCardList[0]==0x4E || pOutCardInfo->iCardList[0]==0x4F) )
						//	{
						//		cardNum = pOutCardInfo->iCardList[0];
						//	}
						//	else
						//		cardNum = m_Logic.GetCardNum(pOutCardInfo->iCardList[0]);
						//	if (m_pUserInfo[pOutCardInfo->bDeskStation])			  //ע��,���·�����������ڱ�ʾ�����Ƿ��Ǳ����״γ���
						//		SetDdzCardSound(cardNum, m_hWnd, ishape, pOutCardInfo->iNextDeskStation, m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//	else
						//		SetDdzCardSound(cardNum, m_hWnd, pOutCardInfo->iNextDeskStation, ishape);
						//}
						//else
						//{
						//	switch(ishape)
						//	{
						//	case UG_BOMB:  //ը��  
						//	case UG_SHAM_BOMB:
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_BOMB,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_BOMB,m_hWnd);
						//		break;
						//	case UG_KING_BOMB:     //��ը
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_KINGBOMB,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_KINGBOMB,m_hWnd);
						//		break;
						//	case UG_DOUBLE_SEQUENCE: //����
						//	case UG_VARIATION_DOUBLE_SEQUENCE:
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_LIANDUI,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_LIANDUI,m_hWnd);
						//		break;
						//	case UG_STRAIGHT: //˳��
						//	case UG_VARIATION_STRAIGHT: 
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_SHUNZI,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_SHUNZI,m_hWnd);
						//		break;
						//	case UG_THREE: //����
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_SAN,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_SAN,m_hWnd);
						//		break;
						//	case UG_THREE_ONE: //����һ
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_SANDAIY,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_SANDAIY,m_hWnd);
						//		break;
						//	case UG_THREE_DOUBLE: //����һ��
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_SANDAID,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_SANDAID,m_hWnd);
						//		break;
						//	case UG_FOUR_TWO: //�Ĵ���
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_SIDAIE,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_SIDAIE,m_hWnd);
						//		break;
						//	case UG_FOUR_TWO_DOUBLE: //�Ĵ�һ��
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_SIDAID,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_SIDAID,m_hWnd);
						//		break;
						//	case UG_VARIATION_THREE_SEQUENCE: //�ɻ�
						//	case UG_THREE_SEQUENCE:
						//	case UG_VARIATION_THREE_ONE_SEQUENCE:
						//	case UG_THREE_ONE_SEQUENCE:
						//	case UG_VARIATION_THREE_DOUBLE_SEQUENCE:
						//	case UG_THREE_DOUBLE_SEQUENCE:
						//		if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//			SetDdzSound(SOUND_OUT_CARD_PLANE,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//		else
						//			SetDdzSound(SOUND_OUT_CARD_PLANE,m_hWnd);
						//		break;
						//	default:
						//		SetPlaySound(SOUND_OUT_CARD_DEF);
						//		break;
						//	}
						//}
					}

					m_PlayView.SetPass(ViewStation(pOutCardInfo->bDeskStation),0);
					m_iBigOutPeople = pOutCardInfo->bDeskStation;
				}
				else
				{
					m_PlayView.SetPass(ViewStation(pOutCardInfo->bDeskStation),1);

					if(m_pGameInfo->bEnableSound)
					{
						//if (m_pUserInfo[pOutCardInfo->bDeskStation])
						//	SetDdzSound(SOUND_OUT_CARD_PASS,m_hWnd,m_pUserInfo[pOutCardInfo->bDeskStation]->GameUserInfo.bBoy);
						//else
						//	SetDdzSound(SOUND_OUT_CARD_PASS,m_hWnd);
						switch(rand()%4)
						{
						case 0:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_A.wav");
								break;
							}
						case 1:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_B.wav");
								break;
							}
						case 2:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_C.wav");
								break;
							}
						case 3:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_D.wav");
								break;
							}
						default:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_A.wav");
								break;
							}
						}
					}
				}

				///���Ӵ�����20100531
				int iReplaceCount = 0;
				BYTE iReplaceCard[4] ={0};
				int iSameCount = 0;
				BYTE iSameCard[20] ={0};
				/////////////////////////

				//��¼������Ϣ
				if (pOutCardInfo->iCardCount > 0)
				{
					m_iBaseCount = pOutCardInfo->iCardCount;
				
					if (pOutCardInfo->bIsReplace)
					{
						::CopyMemory(m_iBaseCard,pOutCardInfo->iReplaceCardList,sizeof(BYTE)*m_iBaseCount);
                           
						if (4 == pOutCardInfo->iCardCount)//����4����ʱ
						{
							if (m_Logic.IsShamBomb(pOutCardInfo->iReplaceCardList,pOutCardInfo->iCardCount))//������ը
							{
								pOutCardInfo->iReplaceCardList[0] = 48 - m_Logic.GetCardHuaKind(pOutCardInfo->iReplaceCardList[3]) + pOutCardInfo->iReplaceCardList[3];
								pOutCardInfo->iReplaceCardList[1] = pOutCardInfo->iReplaceCardList[0] - 16;
								pOutCardInfo->iReplaceCardList[2] = pOutCardInfo->iReplaceCardList[1] - 16;
								pOutCardInfo->iReplaceCardList[3] = pOutCardInfo->iReplaceCardList[2] - 16;
								m_PlayView.m_bIsFakeBomb = TRUE;
							}

							else//��������һʱ
							{ 
								CString str;
								str.Format("zhtlogg:pOutCardInfo->iReplaceCardList[]=%d,%d,%d,%d",pOutCardInfo->iReplaceCardList[0],pOutCardInfo->iReplaceCardList[1],pOutCardInfo->iReplaceCardList[2],pOutCardInfo->iReplaceCardList[3]);
								OutputDebugString(str);
								BYTE byteTempCardVal;//��ʱ���Ƶ�ֵ 
								BYTE byteTempArr[20] = {0};//��ʱ����
								BYTE byteVal1;//����һ��"һ"���Ƶ�ֵ

								for (int k=0; k<4; k++)//ȡ����һ�е�"��"����
								{
									if (pOutCardInfo->iReplaceCardList[k] > 0x40 && pOutCardInfo->iReplaceCardList[k]!=0x4E && pOutCardInfo->iReplaceCardList[k]!=0x4F)
									{
										pOutCardInfo->iReplaceCardList[k] -= 0x40;
									}
								}
								//for (int k=0; k<4; k++)//ȡ����һ�е�"��"����
								//{
								//	byteTempArr[m_Logic.GetCardNum(pOutCardInfo->iReplaceCardList[k])]++;
								//}
								//for (int k=0; k<20; k++)//ȡ����һ�е�"��"���ֵ��Ƶ�ֵ
								//{
								//	if (3 == byteTempArr[k])
								//	{
								//		byteTempCardVal = k;
								//	}
								//}
								//for (int k=0; k<4; k++)//ȡ����һ�е�"һ"�������±겢��ֵ
								//{
								//	if (byteTempCardVal != m_Logic.GetCardNum(pOutCardInfo->iReplaceCardList[k]))
								//	{
								//		byteVal1 = k;
								//	}
								//}

								//pOutCardInfo->iReplaceCardList[3] = pOutCardInfo->iReplaceCardList[byteVal1];
								//if (m_Logic.GetCardNum(pOutCardInfo->iReplaceCardList[3]) == m_Logic.GetCardNum(m_PlayView.m_bLaiCardList[0]))
								//{
								//	pOutCardInfo->iReplaceCardList[3] = pOutCardInfo->iReplaceCardList[3] - 16;
								//}
								//pOutCardInfo->iReplaceCardList[0] = 48 - m_Logic.GetCardHuaKind(pOutCardInfo->iReplaceCardList[!byteVal1]) 
								//	+ pOutCardInfo->iReplaceCardList[!byteVal1];
								//pOutCardInfo->iReplaceCardList[1] = pOutCardInfo->iReplaceCardList[0] - 16;
								//pOutCardInfo->iReplaceCardList[2] = pOutCardInfo->iReplaceCardList[1] - 16;


							}


						}
						::CopyMemory(m_DeskCard[pOutCardInfo->bDeskStation],pOutCardInfo->iReplaceCardList,sizeof(BYTE)*pOutCardInfo->iCardCount);
						m_PlayView.m_bIsFakeBomb = TRUE;

						//CString str;
						//for (int t=0; t<4; t++)
						//{
						//	str.Format("pOutCardInfo->iReplaceCardList:%d,m_Logic.GetCardNum:%d",pOutCardInfo->iReplaceCardList[t],
						//		m_Logic.GetCardNum(pOutCardInfo->iReplaceCardList[t]));
						//	OutputDebugString(str);
						//}

					}
					else
					{
						 m_PlayView.m_bIsFakeBomb = FALSE;
						::CopyMemory(m_iBaseCard,pOutCardInfo->iCardList,sizeof(BYTE)*m_iBaseCount);
					}

					if(pOutCardInfo->bDeskStation!=GetMeUserInfo()->bDeskStation)
					{
						BYTE desk = ViewStation(pOutCardInfo->bDeskStation);
						m_Logic.RemoveCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,m_Card[desk],m_CardCount[desk]);
						m_CardCount[desk] -= pOutCardInfo->iCardCount;
						m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
						m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					}
					else			//ɾ���Լ����˿�
					{
						m_Logic.RemoveCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,m_iMeCardList,m_iMeCardCount);
						m_iMeCardCount -= pOutCardInfo->iCardCount;
						m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
						m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
					}
				}

				//��ʾ�˿�
				int iViewStation = ViewStation(pOutCardInfo->bDeskStation);
				m_Logic.SortCard(m_DeskCard[pOutCardInfo->bDeskStation],NULL,m_iDeskCardCount[pOutCardInfo->bDeskStation]);

				if (pOutCardInfo->bIsReplace)//�д����Ʋ�ȥ�Ҵ�����
				{
					m_Logic.CompareCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,pOutCardInfo->iReplaceCardList,
						pOutCardInfo->iCardCount,iSameCard, iSameCount, iReplaceCard, iReplaceCount);
				}
	
				m_PlayView.m_UserCard[iViewStation].SetCard(m_DeskCard[pOutCardInfo->bDeskStation],NULL,m_iDeskCardCount[pOutCardInfo->bDeskStation]
				,iReplaceCard, iReplaceCount);///���Ӵ������������
				for (int i=0; i<m_iDeskCardCount[pOutCardInfo->bDeskStation];i++)
				{
					CString str;
					str.Format("zhtlogg:m_iDeskCardCount[%d]=%02d %02d",i,m_DeskCard[pOutCardInfo->bDeskStation][i],pOutCardInfo->iCardList[i]);
					OutputDebugString(str);
				}
				

				//���������Ʋ���ը������ 090330a1 JJ
				if (m_Logic.IsBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount) || m_Logic.IsShamBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount) )
				{
					m_PlayView.m_UserCard[iViewStation].ShowWindow(SW_HIDE);
					BeginAnimal(ANIMAL_SBOMB0+ViewStation(pOutCardInfo->bDeskStation));
				}
				if (m_Logic.IsKingBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount))
				{
					for (int i = 0; i < PLAY_COUNT; i++)
					{
						m_PlayView.m_UserCard[i].ShowWindow(SW_HIDE);	
					}
					BeginAnimal(ANIMAL_BBOMB0+ViewStation(pOutCardInfo->bDeskStation), pOutCardInfo->iCardList,pOutCardInfo->iCardCount);
				}
				//end

				m_PlayView.UpdateViewFace(NULL);
				return TRUE;
			}
		case ASS_AWARD_POINT://����
			{
				if (uDataSize != sizeof(AwardPointStruct)) 
					return FALSE;
				AwardPointStruct *awardpoint = (AwardPointStruct*)pNetData;
				m_PlayView.SetAwardPoint(ViewStation(awardpoint->bDeskStation),awardpoint->iAwardPoint);
				m_PlayView.SetGameMultiple((int)pow(2.0,awardpoint->iBase));
				/*if(m_pGameInfo->bEnableSound )
				{
					SetPlaySound(SOUND_OUT_CARD_BOMB);
				}*/

				return true;
			}
		case ASS_OUT_CARD:		//�û�����
			{
				OutCardMsg * pOutCardInfo = (OutCardMsg *)pNetData;
				KillAllTimer();

				//�ж���һ������
				m_iNowOutPeople = pOutCardInfo->iNextDeskStation;
				m_PlayView.SetPass(ViewStation(m_iNowOutPeople),0);

				if (m_iNowOutPeople != -1) 
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));
					if(m_iAIStation[m_iNowOutPeople])
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);

				}
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation)&&(m_bWatchMode==FALSE))
				{
					m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetCard(NULL,NULL,0);;	//�Լ���ǰ����0
					m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.EnableWindow(TRUE);
					m_PlayView.m_btCue.ShowWindow(SW_SHOW);
					m_PlayView.m_btCue.EnableWindow(TRUE);

					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

					OnHitCard(0,0);

					if(m_bAutoCard)
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,(rand()%GAME_RAND_TIME) + 1,ID_OUT_CARD);
#endif 
				}
				return TRUE;
			}
		case ASS_AI_STATION:
			{
				if(uDataSize != sizeof(UseAIStation))
					return FALSE;
				UseAIStation *ai = (UseAIStation*)pNetData;
				if (m_iAIStation[ai->bDeskStation] == ai->bState)
				{
					return TRUE;
				}
				m_iAIStation[ai->bDeskStation] = ai->bState;
				//������@ʾ
				TCHAR szMessage[100];
				if(m_pUserInfo[ai->bDeskStation]!=NULL)
				{
					wsprintf(szMessage,TEXT("[��ʾ] %s %s�������й�"),
						m_pUserInfo[ai->bDeskStation]->GameUserInfo.nickName,ai->bState?TEXT("����"):TEXT("ȡ��"));
				}
				else
				{
					wsprintf(szMessage,TEXT("[��ʾ] %d ��λ�� %s�������й�"),
						ViewStation(ai->bDeskStation),ai->bState?TEXT("����"):TEXT("ȡ��"));
				}
				m_MessageHandle.InsertNormalMessage(szMessage);
				if(ai->bState)//�������
				{
					if(GetStationParameter() == 22)//����
					{
						if(ai->bDeskStation == m_iNowOutPeople)
							ReplaceOutCard(m_iNowOutPeople);
					}
					if(GetStationParameter() == 21)//�з�
					{
						if(ai->bDeskStation == m_bCurrentOperationStation)
							ReplaceCallScore(m_bCurrentOperationStation);
					}
				}
				return true;
			}
		case ASS_ONE_TURN_OVER:
			{
				m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btThing.EnableWindow(TRUE);
				//������һ�ֳ������
				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					m_iLastCardCount[i] = m_iDeskCardCount[i];
					::CopyMemory(m_iLastOutCard[i],m_DeskCard[i],sizeof(BYTE)*m_iLastCardCount[i]);
				}
				return true;
			}
		case ASS_NEW_TURN:		//��һ�ֳ���
			{
				if(uDataSize!=sizeof(NewTurnStruct))
					return FALSE;
				NewTurnStruct *turn = (NewTurnStruct *)pNetData; 

				m_byteHitPass = 0;

				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					m_PlayView.SetPass(i,0);
				}
				//m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
				//������һ�ֳ������
				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					m_iLastCardCount[i] = m_iDeskCardCount[i];
					::CopyMemory(m_iLastOutCard[i],m_DeskCard[i],sizeof(BYTE)*m_iLastCardCount[i]);
				}
				m_btAutoTime=0;
				m_iBaseCount=0;
				m_iBigOutPeople = m_iNowOutPeople = m_iFirstOutPeople = turn->bDeskStation ;
				for (int i = 0;i < PLAY_COUNT;i++)	
					m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				//ɾ���˿���Ϣ
				for (int i = 0;i < PLAY_COUNT;i ++)
					m_iDeskCardCount[i] = 0;

				if(m_iNowOutPeople != -1)
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));
					if(m_iAIStation[m_iNowOutPeople])
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);

					/*if(m_pGameInfo->bEnableSound)
					{
					srand(GetCurrentTime());
					if(rand()%10 >= 8)
					SetSceneSound(SOUND_ERROR,m_pUserInfo[m_iNowOutPeople]->GameUserInfo.bBoy);
					}*/
				}
				if ((m_bWatchMode==false)&&(GetMeUserInfo()->bDeskStation == m_iNowOutPeople))
				{
					m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.EnableWindow(FALSE);
					m_PlayView.m_btPass.ShowWindow(SW_HIDE);
					m_PlayView.m_btCue.ShowWindow(SW_SHOW);
					m_PlayView.m_btCue.EnableWindow(TRUE);

					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

					OnHitCard(0,0);
					if(m_bAutoCard)
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,rand()%GAME_RAND_TIME + 1,ID_OUT_CARD);
#endif 
				}

				return TRUE;
			}
		case ASS_SAFE_END:	//��Ϸ��ȫ����
			{
				if (uDataSize!=sizeof(GameCutStruct)) 
					return FALSE;
				GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
				KillGameTimer(0);
				if(pGameEnd->bDeskStation != GetMeUserInfo()->bDeskStation&&m_pUserInfo[pGameEnd->bDeskStation]!=NULL)
				{
					//if(m_pGameInfo->bEnableSound)
						//SetPlaySound(SOUND_GAME_OVER);
					//SetSceneSound(SOUND_USER_LEFT,m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.bBoy);
				}
				//SetPlaySound(-1,SOUND_GAME_OVER);
				SetStationParameter(GS_WAIT_ARGEE);	
				KillAllTimer();
				ResetGameStation(RS_GAME_CUT);
				//�Ƿ�Ϊ������
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					//m_PlayView.m_btStartDisplayCards.ShowWindow(!m_bWatchMode);
					if(m_PlayView.m_btLeave.m_hWnd) 
					{
						m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					}
					m_PlayView.m_btContinue.SetFocus();
					if(GetComType() != TY_MATCH_GAME)
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif				
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
					//if(2 == GetComType())
					//{
					//	SetGameTimer(GetMeUserInfo()->bDeskStation,GAME_AUTO_TIME,ID_BEGIN_TIME);
					//}
					//SetGameTimer(GetMeUserInfo()->bDeskStation,GAME_AUTO_TIME,ID_BEGIN_TIME);
					//m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
				}
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()!=NULL) 
				{
					m_PlayView.m_ThingMessage.DestroyWindow();
					m_PlayView.m_ThingMessage.m_pParent=NULL;
				}
				return TRUE;
			}
		case ASS_CUT_END:	//�û�ǿ���뿪
			{
				if (uDataSize!=sizeof(GameCutStruct))
					return FALSE;
				GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
				KillGameTimer(0);
				//��Ϸ����
				GameFinishNotify CutNotify;
				::memset(&CutNotify,0,sizeof(CutNotify));

				if(pGameEnd->bDeskStation != GetMeUserInfo()->bDeskStation&&m_pUserInfo[pGameEnd->bDeskStation]!=NULL)
				{

					//��ʾ��Ϣ
					TCHAR szMessage[100];
					if (pNetHead->bAssistantID==ASS_CUT_END)
						wsprintf(szMessage,TEXT("���ڡ� %s ��ǿ���˳�,��Ϸ������"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);
					else 
						wsprintf(szMessage,TEXT("���ڡ� %s ���뿪����Ϸ������"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);

					m_MessageHandle.InsertNormalMessage(szMessage);
					for(int i=0;i<PLAY_COUNT;i++)
					{
						//�½���
						if(m_pUserInfo[i]!=NULL)
						{
							wsprintf(CutNotify.name[i],"%s",m_pUserInfo[i]->GameUserInfo.nickName);
							CutNotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
							CutNotify.iMoney[i]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
							CutNotify.iAwardPoint[i]=0;
						}
						else
						{
							memset(CutNotify.name[i],0,sizeof(CutNotify.name[i]));
							CutNotify.iWardPoint[i]=0;
							CutNotify.iMoney[i]=0;
							CutNotify.iAwardPoint[i]=0;
						}
					}
				
				}	
				if(m_pGameInfo->bEnableSound)
						SetPlaySound(SOUND_GAME_OVER);

				SetStationParameter(GS_WAIT_ARGEE);
				ResetGameStation(RS_GAME_CUT);

				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					//m_PlayView.m_btStartDisplayCards.ShowWindow(!m_bWatchMode);
					if(m_PlayView.m_btLeave.m_hWnd) 
					{
						m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					}
					m_PlayView.m_btContinue.SetFocus();

					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif
				}
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()!=NULL) 
				{
					m_PlayView.m_ThingMessage.DestroyWindow();
					m_PlayView.m_ThingMessage.m_pParent=NULL;
				}
				return TRUE;
			}
		case ASS_NO_CALL_SCORE_END://���˽з�ֱ�ӽ�����һ��
			{
				m_MessageHandle.InsertNormalMessage("���˽���,��Ϸ��ʼ��һ��!");
				KillGameTimer(0);
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(RS_GAME_BEGIN);

				return true;
			}
		case ASS_CONTINUE_END:	//��Ϸ����
			{
				if (uDataSize!=sizeof(GameEndStruct))
					return FALSE;
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				KillGameTimer(0);
				//��Ϸ����
				GameFinishNotify finishnotify;
				::memset(&finishnotify,0,sizeof(finishnotify));
				//��Ϸ����
				CString str;
				

				CString strMoney;
				m_MessageHandle.InsertNormalMessage(TEXT("������Ϸ�����"));
				m_MessageHandle.InsertNormalMessage(TEXT("������������"));
#ifdef FKDDZ
				str.Format(TEXT("�׷�:%d,����������X %d ��,ը�� X %d ��,δ���� X %d �� ��X %d ��"),
					pGameEnd->iCallScore,
					(int)pow(2.0,pGameEnd->iRobMul),(int)pow(2.0,pGameEnd->iBombMul),(int)pow(2.0,pGameEnd->iOutCardMul),(int)pow(2.0,pGameEnd->iTotalMul));
#endif
				m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
				for (int i = 0; i < PLAY_COUNT; i ++)
				{
					if (m_pUserInfo[i]!=NULL)
					{
						m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i)/*viewstation*/,pGameEnd->iTurePoint[i]/*�ı�ķ���*/,pGameEnd->iChangeMoney[i]);
                        SetIntToCharLjl(strMoney,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold);
						str.Format(TEXT("%s ���� %I64d ��� %s"),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],strMoney/*pGameEnd->iChangeMoney[i]*/);
						m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
						
						//���ý�����ʾ��Ϣ
						wsprintf(finishnotify.name[i],TEXT("%s"),m_pUserInfo[i]->GameUserInfo.nickName);
						finishnotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
						finishnotify.iMoney[i]=pGameEnd->iChangeMoney[i];
					}
				}
				finishnotify.iBasePoint=pGameEnd->iTotalMul;
				finishnotify.iCallScore = pGameEnd->iCallScore;
				finishnotify.iUpGradePeople=pGameEnd->iUpGradeStation;
				finishnotify.iTax = pGameEnd->iTax;
				finishnotify.iGameStyle = GetComType();
				for (int i=0; i<4; i++)
				{
					::CopyMemory(finishnotify.iBombCard[i], pGameEnd->iBombCard[i], sizeof(pGameEnd->iBombCard[i]));
				}
				m_PlayView.m_Result.SetMessage(finishnotify,ViewStation(pGameEnd->iUpGradeStation));
				//m_PlayView.m_Result.ShowWindow(SW_SHOW); //090330a3 JJ
   
				m_PlayView.m_Result.SetPower(Glb().m_nPowerOfGold);
				m_PlayView.m_Result.ShowWindow(SW_SHOW);
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(RS_GAME_END);

				//������� 090330a1 JJ
				if (pGameEnd->iTurePoint[pGameEnd->iUpGradeStation] > 0)
				{
					BeginAnimal(ANIMAL_DZWIN);
				} 
				else
				{
					BeginAnimal(ANIMAL_DZLOSE);
				}
				//end

				if(!m_bWatchMode)
				{
					//m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode); 090330a3 JJ
					//if(m_PlayView.m_btLeave.m_hWnd) 
					//{
					//	m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					//}
					//end
					//m_PlayView.m_btStartDisplayCards.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));

#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif
				}
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()!=NULL) 
				{
					m_PlayView.m_ThingMessage.DestroyWindow();
					m_PlayView.m_ThingMessage.m_pParent=NULL;
				}
				return true;
			}
		case ASS_TERMINATE_END://�������
			{
				SetStationParameter(GS_WAIT_ARGEE);
				KillAllTimer();
				ResetGameStation(RS_GAME_END);
				m_MessageHandle.InsertNormalMessage(TEXT("[��ʾ]����Ϸ�Ѿ�ֹͣ,���˳���Ϸ!"));

				if(m_PlayView.m_btLeave.m_hWnd) 
				{
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
				}
				m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
				//m_PlayView.m_btStartDisplayCards.ShowWindow(!m_bWatchMode);
				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_LEAVE_TIME);
				m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
				return true;
			}
		case ASS_NO_CONTINUE_END://��Ϸ����
			{
				if (uDataSize!=sizeof(GameEndStruct))
					return FALSE;
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				KillGameTimer(0);
				//��Ϸ����
				GameFinishNotify finishnotify;
				::memset(&finishnotify,0,sizeof(finishnotify));

				CString strMoney;
				//�����������ʾ�ɼ�
				CString stime;
				stime.Format("%d",CTime::GetCurrentTime());
				int curtime=atoi(stime);
				CString str;
				m_MessageHandle.InsertNormalMessage(TEXT("������Ϸ�����"));
				m_MessageHandle.InsertNormalMessage(TEXT("������������"));
#ifdef FKDDZ
				str.Format(TEXT("�׷�:%d,����������X %d ��,ը�� X %d ��,δ���� X %d �� ��X %d ��"),
					pGameEnd->iCallScore,
					(int)pow(2.0,pGameEnd->iRobMul),(int)pow(2.0,pGameEnd->iBombMul),(int)pow(2.0,pGameEnd->iOutCardMul),(int)pow(2.0,pGameEnd->iTotalMul));
#endif	
				m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
				for (int i = 0; i < PLAY_COUNT; i ++)
				{
					if (m_pUserInfo[i]!=NULL)
					{		
						/*str.Format(TEXT("%s : %d "),
							m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]);*/
						if(m_pUserInfo[i]->GameUserInfo.iDoublePointTime>curtime && pGameEnd->iTurePoint[i]>0)
							{
								  SetIntToCharLjl(strMoney,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold);
								pGameEnd->iTurePoint[i]*=2;
								str.Format(TEXT("%s : ����:%I64d ��˫�����ֿ�Ч�������:%s"),
									m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],strMoney/*pGameEnd->iChangeMoney[i]*/);							
							}
							else
							{
								SetIntToCharLjl(strMoney,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold);
								str.Format(TEXT("%s : ����:%I64d ���:%s"),
									m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],strMoney/*pGameEnd->iChangeMoney[i]*/);								
							}
						m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i)/*viewstation*/,pGameEnd->iTurePoint[i]/*�ı�ķ���*/,pGameEnd->iChangeMoney[i]);
						
						m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
						//���ý�����ʾ��Ϣ
						wsprintf(finishnotify.name[i],TEXT("%s"),m_pUserInfo[i]->GameUserInfo.nickName);
						finishnotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
						finishnotify.iMoney[i]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
					}
				}

				//��������
				if(m_pGameInfo->bEnableSound)
				{
					if (pGameEnd->iTurePoint[GetMeUserInfo()->bDeskStation] > 0)
						VoiceOut(255,"GAME_WIN.wav");//SetPlaySound(SOUND_WIN);
					else
						VoiceOut(255,"GAME_LOST.wav");//SetPlaySound(SOUND_LOSE);

				}

					for (int i=0; i<4; i++)
				{
					::CopyMemory(finishnotify.iBombCard[i], pGameEnd->iBombCard[i], sizeof(pGameEnd->iBombCard[i]));
				}
				finishnotify.iBasePoint=pGameEnd->iTotalMul;
				finishnotify.iCallScore = pGameEnd->iCallScore;
				finishnotify.iUpGradePeople=pGameEnd->iUpGradeStation;
				finishnotify.iTax = pGameEnd->iTax;
				//				finishnotify.iGrade2=pGameEnd->iGrade2;
				finishnotify.iGameStyle = GetComType();
				m_PlayView.m_Result.SetMessage(finishnotify,ViewStation(pGameEnd->iUpGradeStation));
				//m_PlayView.m_Result.ShowWindow(SW_SHOW); //090330a3 JJ

				SetStationParameter(GS_WAIT_ARGEE);
				ResetGameStation(RS_GAME_END);

				//������� 090330a1 JJ
				if (pGameEnd->iTurePoint[pGameEnd->iUpGradeStation] > 0)
				{
					BeginAnimal(ANIMAL_DZWIN);
				} 
				else
				{
					BeginAnimal(ANIMAL_DZLOSE);
				}
				//end

				if(!m_bWatchMode)
				{
					//m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode); 090330a3 JJ
					//if(m_PlayView.m_btLeave.m_hWnd) 
					//{
					//	m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					//}
					//end

					//m_PlayView.m_btStartDisplayCards.ShowWindow(!m_bWatchMode);
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));

				}
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()!=NULL) 
				{
					m_PlayView.m_ThingMessage.DestroyWindow();
					m_PlayView.m_ThingMessage.m_pParent=NULL;
				}
				return TRUE;
			}
		case ASS_HAVE_THING:	//�û������뿪
			{
				HaveThingStruct * pThing=(HaveThingStruct *)pNetData;
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()==NULL) 
				{
					m_PlayView.m_ThingMessage.Create(IDD_THING_MESSAGE,this);
					m_PlayView.m_ThingMessage.m_pParent=this;
				}
				m_PlayView.m_ThingMessage.SetLeaveMessage(pThing->szMessage,m_pUserInfo[pThing->pos]->GameUserInfo.nickName);
				m_PlayView.m_ThingMessage.ShowWindow(SW_SHOW);
				return TRUE;
			}
		case ASS_LEFT_RESULT:	//�����뿪��� 
			{
				if (uDataSize!=sizeof(LeaveResultStruct)) return FALSE;
				LeaveResultStruct * pLeftResult=(LeaveResultStruct *)pNetData;
				CString strBuffer;
				if (!pLeftResult->bArgeeLeave) 
				{
					strBuffer.Format(TEXT("��� %s ��ͬ����Ϸ�����������ֽ����������뿪�߿����뿪��"),m_pUserInfo[pLeftResult->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(strBuffer.GetBuffer(MAX_PATH));
				}
				else
				{
					if(pLeftResult->bArgeeLeave&&GetMeUserInfo()->bDeskStation==pLeftResult->bDeskStation&&!m_bWatchMode)
						OnCancel();
				}
				return TRUE;
			}
		case ASS_AUTO:				//�й�
			{
				if (uDataSize!=sizeof(AutoStruct)) 
					return FALSE;
				AutoStruct *bAuto=(AutoStruct*)pNetData;
				m_PlayView.SetAuto(ViewStation(bAuto->bDeskStation),bAuto->bAuto);
				return true;
			}
		case ASS_MESSAGE:
			{
				if (uDataSize!=sizeof(MessageStruct))
				{
					//AfxMessageBox("435435");
					return FALSE;
				}
				MessageStruct *pMessage=(MessageStruct*)pNetData;
				m_MessageHandle.InsertSystemMessage(pMessage->Message);
				return true;
			}
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
//		m_PlayView.m_dlgDisTalk[i].ShowWindow(SW_HIDE);             //����������
		m_PlayView.SetPeopleMultiple(i,1);					//������ҼӰ�

	}
	m_bTimeOutCount = 0;
	m_iMeCardCount = 0;
	m_iNtPeople = -1;
	m_iNowOutPeople = -1;
	m_iBaseCount = 0;
	m_iFirstOutPeople = -1;
	//	m_iLastCardCount=0;
	m_iBeenPlayGame = 0;
	m_bShowLast = FALSE;
	::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	//��������
	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
	if(m_PlayView.m_btLeave.m_hWnd) 
	{
		m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
	}
	//m_PlayView.m_btStartDisplayCards.ShowWindow(SW_HIDE);
	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
	m_PlayView.m_btThing.EnableWindow(0);

	m_PlayView.m_btAddDouble.ShowWindow(0);
	m_PlayView.m_btAddDouble2.ShowWindow(0);
	m_PlayView.m_btRobNT.ShowWindow(0);
	m_PlayView.m_btRobNT2.ShowWindow(0);
	m_PlayView.m_btShowCard.ShowWindow(0);
	m_PlayView.m_btShowCard2.ShowWindow(0);

	m_PlayView.SetNTStation(-1);

	m_PlayView.m_MeCard.SetCard(NULL,NULL,0);
	m_PlayView.m_MeBackCard.SetCard(NULL,NULL,0);
	m_PlayView.m_GameNoteView.SetMessage(NULL);
	m_PlayView.SetCallScoreResult();
	//if (m_PlayView.m_HaveThing.GetSafeHwnd()) m_PlayView.m_HaveThing.DestroyWindow();
	//if (m_PlayView.m_ThingMessage.GetSafeHwnd()) m_PlayView.m_ThingMessage.DestroyWindow();
	//if(m_PlayView.m_GameNoteView.GetSafeHwnd()) m_PlayView.m_GameNoteView.DestroyWindow();
	if(m_hMciWnd1)
	{
		MCIWndStop(m_hMciWnd1);
		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
		MCIWndStop(m_hMciWnd2);
		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}	

	if(m_hMciWnd3)
	{
		MCIWndStop(m_hMciWnd3);
		MCIWndDestroy(m_hMciWnd3);
		m_hMciWnd3=NULL;
	}
	m_PlayView.SetGameMultiple(0);
	KillAllTimer();
	return __super::ResetGameFrame();
}

//��ʼ��ť����
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
#ifdef AI_MACHINE
	HWND forehwnd=::GetForegroundWindow();
	if(forehwnd == m_hWnd&&m_PlayView.m_Result.IsWindowVisible())
	{//��ͼ��Ļ
		//SaveScreen();
	}
#endif
	m_bIsStartDisplay = FALSE;//�������ƿ�ʼ
	OnControlHitBegin();
	return 0;
}

//���ƿ�ʼ��ť����
LRESULT	CClientGameDlg::OnHitBeginDisplayCards(WPARAM wparam, LPARAM lparam)
{
#ifdef AI_MACHINE
	HWND forehwnd=::GetForegroundWindow();
	if(forehwnd == m_hWnd&&m_PlayView.m_Result.IsWindowVisible())
	{//��ͼ��Ļ
		//SaveScreen();
	}
#endif
	m_bIsStartDisplay = TRUE;//�������ƿ�ʼ
	

	SetNoteFlag(0);
	m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
	//m_PlayView.m_btStartDisplayCards.ShowWindow(SW_HIDE);
	if(m_PlayView.m_btLeave.m_hWnd) 
	{
		m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
	}
	m_PlayView.SetOrientation(-1);

	//׼����������
	//if(m_pGameInfo->bEnableSound)
	//	SetSceneSound(SOUND_READY,m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bBoy);

	if (((GetStationParameter()==GS_WAIT_SETGAME)
		||(GetStationParameter()==GS_WAIT_NEXT)
		||(GetStationParameter()==GS_WAIT_ARGEE))
		&&!m_bWatchMode)
	{
		//�������ƿ�ʼ״̬�������
		ShowCardStruct show;
		memset(&show, 0, sizeof(show));
		show.bDeskStation = GetMeUserInfo()->bDeskStation;		
		show.iValue  =  m_bIsStartDisplay;	
		SendGameData(&show,sizeof(show),MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillAllTimer();
	ResetGameStation(RS_GAME_BEGIN);

	
	return 0;
}


bool CClientGameDlg::OnControlHitBegin()
{
	SetNoteFlag(0);
	m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
	//m_PlayView.m_btStartDisplayCards.ShowWindow(SW_HIDE);
	if(m_PlayView.m_btLeave.m_hWnd) 
	{
		m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
	}
	m_PlayView.SetOrientation(-1);

	//׼����������
	//if(m_pGameInfo->bEnableSound)
	//	SetSceneSound(SOUND_READY,m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bBoy);

	if (((GetStationParameter()==GS_WAIT_SETGAME)
		||(GetStationParameter()==GS_WAIT_NEXT)
		||(GetStationParameter()==GS_WAIT_ARGEE))
		&&!m_bWatchMode)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillAllTimer();
	ResetGameStation(RS_GAME_BEGIN);
	return true;
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_CALL_SCORE_TIME);
	KillTimer(ID_OUT_CARD);
	KillTimer(ID_LEAVE_TIME);
	KillTimer(ID_BEGIN_TIME);
	::memset(m_PlayView.m_uTime,0,sizeof(m_PlayView.m_uTime));
	return;
}

//������
void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_SHOW_LAST:	//�鿴��һ���˿�
		{
			m_bShowIngLast=FALSE;
			for (int i=0;i<PLAY_COUNT;i++)
			{
				if(m_iDeskCardCount[i] > 0)
				{
					m_PlayView.SetPass(ViewStation(i),0);
					m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);
				}
				else
				{
					if ( ( m_byteHitPass&(1<<i) ) == (1<<i) )
					{
						m_PlayView.SetPass(ViewStation(i),1);
					}
				}
			}
			m_PlayView.m_btLastTurn.ShowWindow(SW_SHOW);

			KillTimer(ID_SHOW_LAST);
			return ;
		}
	case ID_ANIMAL_RESULT: //��������(090329a1 JJ)
		{
			BOOL state = m_PlayView.m_AnimalResult.OnTimer();
			if (ER_END == state)
			{
				KillTimer(ID_ANIMAL_RESULT);
				//��ʾ�����
				m_PlayView.m_Result.SetPower(Glb().m_nPowerOfGold);
				m_PlayView.m_Result.ShowWindow(SW_SHOW);
				m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode); 
				if(m_PlayView.m_btLeave.m_hWnd) 
				{
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
				}
			}
			else if (ER_OK == state)
			{
				;					//ɶ�¶�����,����
			}
			else
			{
				KillTimer(ID_ANIMAL_RESULT);
			}
			m_PlayView.UpdateViewFace(NULL);
			return;
		}
	case ID_ANIMAL_BOMB0:
	case ID_ANIMAL_BOMB1:
	case ID_ANIMAL_BOMB2:
		{
			int nDesk = nIDEvent - ID_ANIMAL_BOMB0;
			BOOL state = m_PlayView.m_AnimalBomb[nDesk].OnTimer();
			/*TCHAR str[256] = {0};
			wsprintf(str, "%d  %d   %d", state, nDesk, m_PlayView.m_AnimalBomb[nDesk].IsOnShow());
			m_MessageHandle.InsertNormalMessage(str);*/
			if (ER_END == state)
			{
				KillTimer(ID_ANIMAL_BOMB0+nDesk);
				m_PlayView.m_UserCard[nDesk].ShowWindow(SW_SHOW);
			}
			else if (ER_OK == state)
			{
				;					//ɶ�¶�����,����
			}
			else
			{
				KillTimer(ID_ANIMAL_BOMB0+nDesk);
			}
			m_PlayView.UpdateViewFace(NULL);
			return;
		}
	case ID_ANIMAL_BIGBOMB0:
	case ID_ANIMAL_BIGBOMB1:
	case ID_ANIMAL_BIGBOMB2:
		{
			int nDesk = nIDEvent - ID_ANIMAL_BIGBOMB0;
			//BOOL state = m_PlayView.m_AnimalBomb[nDesk].OnTimer();
			BOOL state = m_PlayView.m_AnimalBigBomb.OnTimer();
			/*TCHAR str[256] = {0};
			wsprintf(str, "%d  %d   %d", state, nDesk, m_PlayView.m_AnimalBigBomb.IsOnShow());
			m_MessageHandle.InsertNormalMessage(str);*/
			if (ER_END == state)
			{
				KillTimer(ID_ANIMAL_BIGBOMB0+nDesk);
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					m_PlayView.m_UserCard[i].ShowWindow(SW_SHOW);	
				}
				m_PlayView.UpdateViewFace(NULL);
			}
			else if (ER_OK == state)
			{
				;					//ɶ�¶�����,����
			}
			else if (ER_UPDATE == state)
			{
				m_PlayView.UpdateViewFace(NULL);
			}
			else
			{
				//KillTimer(ID_ANIMAL_BIGBOMB0+nDesk);
				m_PlayView.UpdateViewFace(NULL);
			}	
			return;
		}
		//end
	default:
		break;
	}
	CLoveSendClassInExe::OnTimer(nIDEvent);
}
//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount<=0&&!m_bWatchMode)
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
				return true;
			}
			if (uTimeCount <= 5) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			m_PlayView.UpdateViewFace(NULL);
			return true;
		}
	case ID_LEAVE_TIME:
		{
			if (uTimeCount <= 0&&!m_bWatchMode)
			{
				//�뿪
				OnCancel();
				return true;
			}
			if (uTimeCount<=5) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_CALL_SCORE_TIME:	//�зּ�ʱ��
		{
			//����з�
			if(m_bCurrentOperationStation != 255)
			{
				if(m_iAIStation[m_bCurrentOperationStation] &&  !m_bWatchMode)
				{
					ReplaceCallScore(m_bCurrentOperationStation);
					return true;
				}
				if (m_pUserInfo[m_bCurrentOperationStation])
				{
					if(uTimeCount <= 0&&m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState == USER_CUT_GAME&&!m_bWatchMode)
					{	
						ReplaceCallScore(m_bCurrentOperationStation);
						return true;
					}
				}
			}

			if (uTimeCount <= 0 && m_bCurrentOperationStation == GetMeUserInfo()->bDeskStation  && !m_bWatchMode)
			{
				if(!m_bAutoCard)
					OnCallScore(0,0);
				else//�йܽ�3��
					OnCallScore(30,30);
				return true;
			}

			if (uTimeCount <= 5 && 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_ROB_NT_TIME://������
		{
			if(m_iAIStation[m_bCurrentOperationStation] && m_iNowOutPeople&&!m_bWatchMode)
			{
				ReplaceRobNT(m_bCurrentOperationStation);
				return true;
			}
			if(uTimeCount <= 0 && m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState == USER_CUT_GAME&&!m_bWatchMode)
			{	
				ReplaceRobNT(m_bCurrentOperationStation);
				return true;
			}

			if (uTimeCount <= 0 && m_bCurrentOperationStation == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
			{
				OnRobNT(0,0);
				return true;
			}
			if (uTimeCount <= 5 && 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_ADD_DOUBLE_TIME://�Ӱ�
		{
			//if(m_iAIStation[m_bCurrentOperationStation] && m_iNowOutPeople)
			//{
			//	SupersedeAddDouble(m_bCurrentOperationStation);
			//	return true;
			//}
			//if(uTimeCount<=0&&m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState == USER_CUT_GAME)
			//{	
			//	SupersedeAddDouble(m_bCurrentOperationStation);
			//	return true;
			//}

			if (uTimeCount <= 0 && 1 == ViewStation(bDeskStation)&&!m_bWatchMode)
			{
				m_PlayView.m_btAddDouble.ShowWindow(0);
				m_PlayView.m_btAddDouble2.ShowWindow(0);
				//OnAddDouble(0,0);
				return true;
			}

			if (uTimeCount <= 5 && 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_SHOW_CARD://����
		{
			if (uTimeCount <= 0 && m_bCurrentOperationStation == GetMeUserInfo()->bDeskStation && !m_bWatchMode)
			{
				OnShowCard(0,0);
				return true;
			}

			if (uTimeCount <= 5&& 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_OUT_CARD:	//����
		{
			int iViewStation=ViewStation(bDeskStation);

		
			if(m_iAIStation[m_iNowOutPeople]&&uTimeCount<=0&&!m_bWatchMode)
			{
				ReplaceOutCard(m_iNowOutPeople);
				return true;
			}
			if(m_pUserInfo[m_iNowOutPeople] == NULL)
				return true;
			//��ǰ������ҵ���
			if(uTimeCount<= 0 && m_iNowOutPeople>=0 && m_iNowOutPeople < PLAY_COUNT && m_pUserInfo[m_iNowOutPeople]->GameUserInfo.bUserState == USER_CUT_GAME&&!m_bWatchMode)
			{	
				ReplaceOutCard(m_iNowOutPeople);
				return true;
			}
			if (uTimeCount <= 0 && !m_bWatchMode)	//ʱ�䵽��
			{

				m_PlayView.m_SelectLaiZhi.ShowWindow(0);
				//�жϳ�ʱ
				if(!m_bAutoCard&&(m_iNowOutPeople == GetMeUserInfo()->bDeskStation) &&  !m_bWatchMode)
					m_bTimeOutCount++;
				/*if (!m_bAutoCard&&m_bTimeOutCount>2 && (m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
				{
				KillGameTimer(0);
				SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
				SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
				AFCMessage(TEXT("����3���Զ����ƣ��Զ��˳���Ϸ������ϵͳ�۳�15��"),m_pGameInfo->szGameName,MB_ICONQUESTION,this);
				PostMessage(WM_CLOSE,0,0);
				return false;
				}//m_bAutoCard==trueΪ�й�
				else*/ 
#ifndef AI_MACHINE
				if(!m_bAutoCard&&m_iNowOutPeople==GetMeUserInfo()->bDeskStation && !m_bWatchMode)
				{
					CString str;
					str.Format(TEXT("[��ʾ]���Ѿ���ʱ %d ����,��ʱ����,���Զ��й�."),m_bTimeOutCount);
									m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
					if(m_bTimeOutCount >= 3)
						OnAuto(0,0);
				}
#endif
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation)&&(m_bWatchMode == FALSE))
				{
					UserOutCard();
				}
				return true;
			}
			if ((uTimeCount<=6)&&(m_bWatchMode==FALSE)&&(iViewStation==1)) 
			{
				if(m_pGameInfo->bEnableSound)
					VoiceOut(255,"GAME_WARN.WAV");//SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	default:
		break;
	}
	return TRUE;
	//return __super::OnGameTimer(bDeskStation,uTimeID,uTimeCount);
}
//�����˿˰�ť����
LRESULT	CClientGameDlg::OnSelectNum(WPARAM wparam, LPARAM lparam)
{
//��������
	m_PlayView.m_SelectLaiZhi.ShowWindow(0);
	OutCardStruct OutCard;
	memset(&OutCard,0,sizeof(OutCard));
	OutCard.iCardCount=m_PlayView.m_MeCard.GetUpCard(OutCard.iCardList);
	OutCard.bIsReplace = true;
	KillAllTimer();
	m_iNowOutPeople=-1;
	::CopyMemory(OutCard.iReplaceCardList, m_Replace.ReplaceList[(int)wparam], sizeof(BYTE)*OutCard.iCardCount);
	
	if (m_Replace.ReplaceListShape[(int)wparam] == UG_BOMB)
	{
			::CopyMemory(OutCard.iReplaceCardList, OutCard.iCardList, sizeof(BYTE)*OutCard.iCardCount);
	}
	else if (m_Replace.ReplaceListShape[(int)wparam] != UG_BOMB && OutCard.iCardCount == 4)
	{
		    /*
		    BYTE tempUpCard[20] = {0};
		    BYTE iCount = m_PlayView.m_MeCard.GetUpCard(tempUpCard);
		    BYTE byteTempLaiZiValue = m_Logic.GetCardNum(m_PlayView.m_bLaiCardList[0]);//��ӵ��Ƶ�ֵ

		    int iTemp;//��ʱ�������±�

			BYTE byteTempArr[20] = {0};
			for (int i=0; i<4; i++)
			{
				BYTE byteValue = m_Logic.GetCardNum(OutCard.iReplaceCardList[i]);//��ʱ���Ƶ�ֵ
				for (int k=0; k<20; k++)
				{
					if (0 != tempUpCard[k])
					{
						if ((byteTempLaiZiValue == m_Logic.GetCardNum(tempUpCard[k])) && (byteValue == byteTempLaiZiValue))
						{
							OutCard.iReplaceCardList[i] = tempUpCard[k];
						}
					}
				}

				byteTempArr[m_Logic.GetCardNum(OutCard.iReplaceCardList[i])]++;  //��¼��ͬ�Ƶ�����м���
			}

			for (int i=0; i<20; i++)
			{
				if (byteTempArr[i] == 3)//����һ
				{
					for (int j=0; j<4; j++)
					{
						if (m_Logic.GetCardNum(OutCard.iReplaceCardList[j]) != i)
						{
							OutCard.iReplaceCardList[3] = OutCard.iReplaceCardList[j];
						}
						else
						{
							iTemp = j;
						}
					}
					OutCard.iReplaceCardList[0] = 48 - m_Logic.GetCardHuaKind(OutCard.iReplaceCardList[iTemp]) + OutCard.iReplaceCardList[iTemp];
					OutCard.iReplaceCardList[1] = OutCard.iReplaceCardList[0] - 16;
					OutCard.iReplaceCardList[2] = OutCard.iReplaceCardList[1] - 16;
				}
			}
			*/
			


	}

	if(!m_Logic.CanOutCard(OutCard.iReplaceCardList,OutCard.iCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0))
		OutCard.iCardCount  = 0;
	//OutCard.iReplaceCount = m_Replace.ReplaceList[(int)wparam]
	//��������
	SendGameData(&OutCard,sizeof(OutCard),
		MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	return 0;
}
//�����˿˰�ť����
LRESULT	CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	if (GetStationParameter()==GS_PLAY_GAME)	//����״̬
	{
			//��֤������Ϣ
			BYTE iUpCard[39];
			if (m_iNowOutPeople!=GetMeUserInfo()->bDeskStation) 
				return 0;
			int iCardCount=m_PlayView.m_MeCard.GetUpCard(iUpCard);

			//������Ŀ�ж�
			if (iCardCount==0)
			{
				m_PlayView.m_btOutCard.EnableWindow(FALSE);
				return 0;
			}
			//��֤�Ƿ���Գ��������˿�
			if (m_iBigOutPeople == GetMeUserInfo()->bDeskStation)	//��һ��������
			{
				if (m_Logic.GetReplaceCardCount(iUpCard,iCardCount) && m_Logic.GetReplaceCardCount(iUpCard,iCardCount) != iCardCount) //modified by Longan 12.12 2008
				{
					memset(&m_Replace,0,sizeof(m_Replace));
					if (m_Logic.GetCardReplaceShape(iUpCard,iCardCount,m_Replace) > 0)
					{
						m_PlayView.m_btOutCard.EnableWindow(true);
					}
					else
					{
						m_PlayView.m_btOutCard.EnableWindow(false);
					}

				}
				else
					m_PlayView.m_btOutCard.EnableWindow(m_Logic.GetCardShape(iUpCard,iCardCount)!=UG_ERROR_KIND);
			}
			else	//�������� 
			{
				if (m_Logic.GetReplaceCardCount(iUpCard,iCardCount) &&m_Logic.GetReplaceCardCount(iUpCard,iCardCount)!=iCardCount) //modified by Longan 12.12 2008
				{
						m_PlayView.m_btOutCard.EnableWindow(m_Logic.ReplaceCanOutCard(iUpCard,iCardCount,m_iBaseCard,
						   m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_Replace,m_iBaseCount == 0));
				}
				else
				{
					   m_PlayView.m_btOutCard.EnableWindow(m_Logic.CanOutCard(iUpCard,iCardCount,m_iBaseCard,
						   m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0));
				}
			}
	}

	return 0;
}



LRESULT	CClientGameDlg::OnRightHitCard(WPARAM wparam, LPARAM lparam)
{
	//	if(!m_PlayView.m_bRightPower)
	OnHitOutCard(0,0);
	//else
	//	OnHitCard(0,0);
	return 0;
}
LRESULT	CClientGameDlg::OnCue(WPARAM wparam, LPARAM lparam)
{
	BYTE CueCardList[45]={0},ResultCard[45];
	int iResultCardCount=0;
	//ȫ����������
	memset(CueCardList,0,sizeof(CueCardList));
	//��ȡ����������
	BYTE bUpCardList[54];
	int iUpCardCount;
	iUpCardCount = m_PlayView.m_MeCard.GetUpCard(bUpCardList);
	if(iUpCardCount == 0)//��һ�ε���ʾ
	{
		//��ʾ
		m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,ResultCard,	
			iResultCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation,TRUE);
	}
	else//��n+1�ε��
	{
		//�ϴε�����ʾ
		if(m_Logic.CanOutCard(bUpCardList,iUpCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation))
		{
			m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,bUpCardList,iUpCardCount,ResultCard,	
				iResultCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation,TRUE);
		}
		//û�б������Ƹ����,�ع�
		if(iResultCardCount == 0)
		{
			m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,ResultCard,	
				iResultCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation,TRUE);
		}
	}
	//��ʾ��������0
	if(iResultCardCount > 0)
		SetCueCardList(CueCardList,m_iMeCardList,m_iMeCardCount,ResultCard,iResultCardCount);
	/*else
		SetPlaySound(SOUND_INVALIDATE);*/
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,CueCardList,m_iMeCardCount);


	OnHitCard(0,0);
	//add by ljl ��ֻʣ��һ������Ƶ�ʱ����ʾ�������ж�m_btOutCardS�Ƿ���ã����Ŀ��߼��ˡ�
	if(iResultCardCount <=0 || !m_PlayView.m_btOutCard.IsWindowEnabled() )
		OnHitPassCard(0,0);
	return 0;
}
//��ʾ��
void CClientGameDlg::SetCueCardList(BYTE CueCardList[],BYTE iMeCardList[],int iMeCardCount,BYTE ResultCard[],int iResultCardCount)
{
	memset(CueCardList,0,sizeof(CueCardList));

	if(iResultCardCount>0)
	{
		for(int i=0;i<iResultCardCount;i++)
			for(int j=0;j<m_iMeCardCount;j++)
			{
				if(m_iMeCardList[j]==ResultCard[i]&&!CueCardList[j])
				{
					CueCardList[j]=1;
					break;
				}
			}
	}	
	return ;
}

//�������ư�ť����
LRESULT	CClientGameDlg::OnHitOutCard(WPARAM wparam, LPARAM lparam)
{
	//���ÿؼ�
	if (m_PlayView.m_btOutCard.IsWindowEnabled()&&m_PlayView.m_btOutCard.IsWindowVisible())
	{
		
		
		//��������
		OutCardStruct OutCard;
		memset(&OutCard,0,sizeof(OutCard));
		OutCard.iCardCount=m_PlayView.m_MeCard.GetUpCard(OutCard.iCardList);

		if(OutCard.iCardCount != 0)
		{	
			if (m_Logic.GetReplaceCardCount(OutCard.iCardList,OutCard.iCardCount) &&
				m_Logic.GetReplaceCardCount(OutCard.iCardList,OutCard.iCardCount) != OutCard.iCardCount)
			{
				if (m_Replace.ListCount > 1)
				{
					//����ѡ��Ҫ��������͵ĶԻ���
					if (m_bAutoCard)
					{
						m_PlayView.m_SelectLaiZhi.SetMessage(m_Replace,m_iBaseCount == 0, true);
					}
					else
					{
						m_PlayView.m_SelectLaiZhi.SetMessage(m_Replace,m_iBaseCount == 0);
						m_PlayView.m_SelectLaiZhi.ShowWindow(SW_SHOW);
					}					
				}
				else
				{
					KillAllTimer();
					m_iNowOutPeople=-1;
					OutCard.bIsReplace = true;
					::CopyMemory(OutCard.iReplaceCardList, m_Replace.ReplaceList[0], sizeof(BYTE)*OutCard.iCardCount);
					if (m_Replace.ReplaceListShape[0] == UG_BOMB)
					{
						::CopyMemory(OutCard.iReplaceCardList, OutCard.iCardList, sizeof(BYTE)*OutCard.iCardCount);
					}
					if(!m_Logic.CanOutCard(OutCard.iReplaceCardList,OutCard.iCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0))
						OutCard.iCardCount  = 0;
					m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
					m_PlayView.m_btPass.ShowWindow(SW_HIDE);
					m_PlayView.m_btCue.ShowWindow(SW_HIDE);
					m_PlayView.m_SelectLaiZhi.ShowWindow(SW_HIDE);
					//��������
					SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
						MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
				}
			}
			else
			{
				KillAllTimer();
				m_iNowOutPeople=-1;
				OutCard.bIsReplace = false;
				if(!m_Logic.CanOutCard(OutCard.iCardList,OutCard.iCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0))
					OutCard.iCardCount  = 0;
				//��������
				m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
				m_PlayView.m_btPass.ShowWindow(SW_HIDE);
				m_PlayView.m_btCue.ShowWindow(SW_HIDE);
				m_PlayView.m_SelectLaiZhi.ShowWindow(SW_HIDE);
				SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
					MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
			}
		}
		//����Ԥ��ʾ
		//m_PlayView.m_UserCard[2].SetCard(OutCard.iCardList,NULL,OutCard.iCardCount);
	}

	return 0;
}


//�������°�ť����
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	if (m_PlayView.m_HaveThing.GetSafeHwnd()==NULL) 
	{
		m_PlayView.m_HaveThing.Create(IDD_THING,this);
		m_PlayView.m_HaveThing.m_pParnet=this;
	}
	m_PlayView.m_HaveThing.ShowWindow(SW_SHOW);
	m_PlayView.m_HaveThing.SetFocus();
	return 0;
}

//�й�
LRESULT	CClientGameDlg::OnAuto(WPARAM wparam, LPARAM lparam)
{
	if(m_btAutoTime>2)											//һ����Ϸ����һȦ��,ֻ�����й�һ��
		return 0;
	m_btAutoTime+=1;
	if(!m_bWatchMode)
	{
		//m_bTimeOutCount=0;
		m_bAutoCard =!m_bAutoCard;
		if(!m_bAutoCard)
		{
			//m_PlayView.m_btAuto.LoadButtonBitmap(".\\image\\ddz\\Auto.bmp",false); 
			if(GetStationParameter()==GS_PLAY_GAME&&m_iNowOutPeople==GetMeUserInfo()->bDeskStation)
			{
				KillAllTimer();
				SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
			}
		}
		else
		{	
			//m_PlayView.m_btAuto.LoadButtonBitmap(".\\image\\ddz\\Auto_less.bmp",false);
			if(GetStationParameter()==GS_PLAY_GAME&&m_iNowOutPeople==GetMeUserInfo()->bDeskStation)
			{
				KillAllTimer();
				//SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
				UserOutCard();
			}
		}
	}
	AutoStruct bAuto;
	bAuto.bAuto=m_bAutoCard;
	SendGameData(&bAuto,sizeof(bAuto),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	return 0;
}


//�����뿪������
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
		m_PlayView.m_btThing.EnableWindow(FALSE);
		HaveThingStruct HaveThing;
		HaveThing.pos=0;
		::lstrcpy(HaveThing.szMessage,(char *)wparam);
		SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	return 0;
}

//�û������뿪
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	LeaveResultStruct Leave;
	BYTE result;
	result=(BYTE)wparam;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	if(result < 2)
	{
		Leave.bArgeeLeave=result;
		SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	return 0;
}

//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)//��Ϸ��ʼ
{
	//��������
	m_bTimeOutCount=0;//��ʱ����

	m_iNtPeople=-1;
	m_iNowOutPeople=-1;
	m_iBaseCount=0;
	m_iFirstOutPeople=-1;

	m_bShowLast=FALSE;
	m_bShowIngLast=FALSE;
	m_bCurrentOperationStation = 255;//��ǰ�з���
	//��������
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));
	 //��Ϸ������Ҫ���ô���״̬
	::memset(m_iAIStation,0,sizeof(m_iAIStation)); 
	m_PlayView.m_bIsFakeBomb = FALSE;//�Ƿ�Ϊ��ը

	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
	m_PlayView.m_btCue.ShowWindow(SW_HIDE);
	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
	m_PlayView.SetNTStation();
	m_ResultCallScore=-1;			//��ʼ��
	m_PlayView.m_btAuto.EnableWindow(false);
	m_PlayView.m_btThing.EnableWindow(false);
	m_PlayView.m_btSortCard.EnableWindow(false);
	m_PlayView.m_btAddDouble.ShowWindow(0);
	m_PlayView.m_btAddDouble2.ShowWindow(0);
	m_PlayView.m_btRobNT.ShowWindow(0);
	m_PlayView.m_btRobNT2.ShowWindow(0);
	m_PlayView.m_btShowCard.ShowWindow(0);
	m_PlayView.m_btShowCard2.ShowWindow(0);
	m_PlayView.m_btLastTurn.EnableWindow(0);
	HideButton();
	m_bAutoCard=false;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);

		m_PlayView.SetPass(i,0);


		m_PlayView.SetAuto(i,false);

		m_PlayView.SetAwardPoint(i,0);

		m_PlayView.SetCallScoreValue(i,-1);		//�з�����
	}
	switch (iGameStation)
	{
	case RS_GAME_END:
		{
			for(int i = 0; i < PLAY_COUNT; i ++)
			{	
				m_PlayView.m_HandCard[i].SetCard(m_Card[i],NULL,m_CardCount[i]);
				m_PlayView.m_HandCard[i].SetShowCard(true);

				if(i == GetMeUserInfo()->bDeskStation)
				{
					m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
				}
			}
			break;
		}
	case RS_ALL_VIEW:
		m_PlayView.m_Result.ShowWindow(SW_HIDE);

	case RS_GAME_CUT:
		{
			for(int i = 0;i < PLAY_COUNT; i ++)
			{

				m_PlayView.m_HandCard[i].SetShowCard(FALSE);//�����ò���ʾ��״̬
				m_PlayView.m_HandCard[i].SetCard(NULL,NULL,0);
				m_CardCount[i] = 0;
				m_PlayView.SetPeopleMultiple(i,1);					//������ҼӰ�
			}	
			CRect rect;
			m_PlayView.GetWindowRect(rect);
			m_PlayView.SetNTStation(-1);
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
			m_PlayView.m_MeBackCard.ShowWindow(0);
			::memset(m_iBackCard,0,sizeof(m_iBackCard));

			//�����ǰ�û�����Ϣ
			m_PlayView.m_MeBackCard.SetCard(NULL,NULL,0);
			m_PlayView.m_MeCard.SetCard(NULL,NULL,0);
			m_Logic.SetLai(255);
			m_PlayView.SetLai(255);
			m_iMeCardCount = 0;
			::memset(m_iAIStation,0,sizeof(m_iAIStation));
			break;
		}
	case RS_GAME_BEGIN:
		{
			m_PlayView.m_Result.ShowWindow(SW_HIDE);
			m_PlayView.SetCallScoreResult(0);					//�з�����
			//�����Ƿ����½�����
			m_PlayView.m_bFirstGame = false;		//090310a1 JJ 
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				m_PlayView.m_HandCard[i].SetShowCard(FALSE);//�����ò���ʾ��״̬
				m_PlayView.m_HandCard[i].SetCard(NULL,NULL,0);
				m_CardCount[i] = 0;
				m_PlayView.SetPeopleMultiple(i,1);					//������ҼӰ�
			}	
			CRect rect;
			m_PlayView.GetWindowRect(rect);
			m_PlayView.SetNTStation(-1);
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
			m_Logic.SetLai(255);
			m_PlayView.SetLai(255);
			m_PlayView.m_MeBackCard.ShowWindow(0);
			::memset(m_iBackCard,0,sizeof(m_iBackCard));

			//�����ǰ�û�����Ϣ
			m_PlayView.m_MeBackCard.SetCard(NULL,NULL,0);
			m_PlayView.m_MeCard.SetCard(NULL,NULL,0);
			m_iMeCardCount=0;
			m_PlayView.SetGameMultiple(1);
			break;
		}
	}
	return;
}

void CClientGameDlg::OnGameSetting()
{
#ifdef SUPER_VERSION
	AfxSetResourceHandle(GetModuleHandle("DllForGame.dll"));

	CCardSet CardSetDlg(this);
	if (IDOK == CardSetDlg.DoModal())
	{
		return ;
	}

	AfxSetResourceHandle(NULL);
#endif
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	CGameSet GameSetDlg(this);
	GameSetDlg.m_bSound=m_pGameInfo->bEnableSound;
	GameSetDlg.m_bShowUser=m_pGameInfo->bShowUserInfo;	
	GameSetDlg.m_bEnableWatch=m_pGameInfo->bEnableWatch;
	GameSetDlg.m_bRightPower = GetProfileInt(TEXT("RightPower"),FALSE);//˳ʱ�����

	int reval =  GameSetDlg.DoModal();

	if (reval == IDOK)
	{
		//�������
		bool bSendWatch=((m_bWatchMode==false)&&(m_pGameInfo->bEnableWatch!=GameSetDlg.m_bEnableWatch));
		m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
		m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
		m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
		WriteProfileInt(TEXT("RightPower"),GameSetDlg.m_bRightPower?1:0);
		m_bRightPower = GameSetDlg.m_bRightPower;
		//��������
		if (bSendWatch==true)
		{
			MSG_GM_WatchSet WatchSet;			
			memset(&WatchSet,0,sizeof(WatchSet));
			WatchSet.dwUserID=0;
			SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);

		}
	}
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{
	if (m_bWatchMode==true)
	{
		m_PlayView.m_MeCard.SetShowCard(m_bWatchOther?TRUE:FALSE);
		if (m_bWatchOther)
			m_MessageHandle.InsertNormalMessage(TEXT("����������Թ�����Ϸ"));
		else 
			m_MessageHandle.InsertNormalMessage(TEXT("��Ҳ��������Թ�����Ϸ"));
	}
}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	//��
	if(pUserItem == NULL)
		return false;
	//��������Ϸ�����뿪
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		OnCancel();
		return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	}

	//�������Ǳ����뿪��Ϸ��
	if(GetMeUserInfo()->bDeskStation != bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	}

	if (bWatchUser==false)//&&(bDeskStation==0))
	{
		if (GetStationParameter() > GS_WAIT_ARGEE)
		{
			this->SetStationParameter(GS_WAIT_ARGEE);
			ResetGameStation(RS_ALL_VIEW);			
			m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode && TY_MATCH_GAME != GetComType());
		}
	}

	//�Թ���Ҳ�뿪��Ϸ��
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser==TRUE)
	{
		OnCancel();
	}

	return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
}
BOOL CClientGameDlg::VoiceOut(BYTE bDeskStation, LPCSTR pszSound)
{

	CString strTmp = pszSound;



	if ( bDeskStation == 255 )
	{
		TCHAR fileName[100];
		ZeroMemory(fileName,sizeof(fileName));
		wsprintf(fileName,".//music//cardwav//%s",strTmp);
		::PlaySound(fileName,NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
		return true;
	}

	if(bDeskStation<0 ||bDeskStation>=PLAY_COUNT)
	{

		return FALSE;
	}



	TCHAR fileName[100];
	ZeroMemory(fileName,sizeof(fileName));






	if(m_pUserInfo[bDeskStation]->GameUserInfo.bBoy)
	{
		strTmp.Replace("human","MALE");
		wsprintf(fileName,".//music//man//%s",strTmp);



	}
	else
	{
		strTmp.Replace("human","FEMALE");
		wsprintf(fileName,".//music//women//%s",strTmp);
	}

	::PlaySound(fileName,NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
	//TCHAR asdasd[4];
	//wsprintf(asdasd,"%d",bDeskStation);
	//OutputDebugString(fileName);
	//OutputDebugString(asdasd);
	return TRUE;
}

//�˳�
void CClientGameDlg::OnCancel()
{
	TCHAR sz[200];
	CString strMoney;

	strMoney.Format("zhtout:%d,%d",m_iRunPublish * m_iRoomBasePoint,Glb().m_nPowerOfGold);
	OutputDebugString(strMoney);
	SetIntToCharLjl(strMoney,m_iRunPublish * m_iRoomBasePoint,Glb().m_nPowerOfGold);
	
	if(GetComType() == TY_MONEY_GAME)
		wsprintf(sz,TEXT("�����˳������ %d ����(��ҳ������ %s ���).�����Ҫ�˳���?"),m_iRunPublish,strMoney);
	else
		wsprintf(sz,TEXT("�����˳������ %d ����.�����Ҫ�˳���?"),m_iRunPublish);
	//��ѯ״̬
	if (!m_bWatchMode && GetStationParameter() >= GS_SEND_CARD&&GetStationParameter() <= GS_WAIT_NEXT)
	{
		if (IDOK!=AFCMessageBox(sz,m_pGameInfo->szGameName,AFC_YESNO))
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	//�����Ƿ����������
	m_PlayView.m_bFirstGame = true;
	AFCCloseFrame();

	__super::OnCancel();
}
#ifdef MY_TWO
//���Ż��㵽��ͼλ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	BYTE bViewStation = GetMeUserInfo()->bDeskStation ;//m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
	switch(bViewStation)
	{
	case 0: 
		{ 
			if(bDeskStation == 0)	
				return 1;
			if(bDeskStation == 1)
				return  0 ;
		
		}
	case 1:
		{
			if(bDeskStation == 0)	
				return 0 ;
			if(bDeskStation == 1)
				return 1;
		}

	default:
		break;
	}
	return 0;
}
#else

//���Ż��㵽��ͼλ��
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	BYTE bViewStation = GetMeUserInfo()->bDeskStation ;//m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
	switch(bViewStation)
	{
	case 0: 
		{ 
			if(bDeskStation == 0)	
				return 1;
			if(bDeskStation == 2)
				return  2 ;
			if(bDeskStation == 1)
				return  0;
		}
	case 1:
		{
			if(bDeskStation == 0)	
				return 2 ;
			if(bDeskStation == 1)
				return 1;
			if(bDeskStation == 2)
				return 0 ;
		}

	case 2: 
		{ 
			if(bDeskStation == 0)	
				return 0 ;

			if(bDeskStation == 1)
				return 2 ;

			if(bDeskStation == 2)
				return 1;
		}
	default:
		break;
	}
	return 0;
}
#endif

//�з�
void CClientGameDlg::ShowCallScoreBt(int CallScore,BOOL bExtVol)
{
	m_PlayView.m_btCallScore1.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScore2.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScore3.ShowWindow(SW_SHOW);

	m_PlayView.m_btCallScorePass.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.EnableWindow(1);

	m_PlayView.m_btCallScore1.EnableWindow(false);
	m_PlayView.m_btCallScore2.EnableWindow(false);
	m_PlayView.m_btCallScore3.EnableWindow(false);

	switch(CallScore)
	{
	case 30:
		break;
	case 20:
		m_PlayView.m_btCallScore3.EnableWindow(TRUE);
		break;
	case 10:
		m_PlayView.m_btCallScore2.EnableWindow(TRUE);
		m_PlayView.m_btCallScore3.EnableWindow(TRUE);
		break;
	default:
		m_PlayView.m_btCallScore1.EnableWindow(TRUE);
		m_PlayView.m_btCallScore2.EnableWindow(TRUE);
		m_PlayView.m_btCallScore3.EnableWindow(TRUE);
		break;
	}
	return;
}
//�з�
LRESULT	CClientGameDlg::OnCallScore(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_CALL_SCORE_TIME);
	HideButton();
	CallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.iValue = (int)wparam;								
	score.bDeskStation = GetMeUserInfo()->bDeskStation;			
	score.bCallScoreflag = false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	return 0;
}
//���ذ�ť
BOOL CClientGameDlg::HideButton()
{
	m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore2.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore3.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE);
	return true;
}
//����
LRESULT	CClientGameDlg::OnHitSortCard(WPARAM wparam, LPARAM lparam)
{
	//����ʽ
	m_bSortCard ++;
	m_bSortCard %= 2;
	m_Logic.SetSortCardStyle(m_bSortCard);
	m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	OnHitCard(0,0);								
	return 0;
}
//����
LRESULT	CClientGameDlg::OnHitPassCard(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_OUT_CARD);
	//���ÿؼ�
	if (m_PlayView.m_btPass.IsWindowVisible())
	{
		//��������
		KillAllTimer();
		m_iNowOutPeople=-1;
		m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
		m_PlayView.m_btPass.ShowWindow(SW_HIDE);
		m_PlayView.m_btCue.ShowWindow(SW_HIDE);
		//��������
		OutCardStruct OutCard;
		OutCard.iCardCount=0;
		SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
			MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	return 0;
}


//��������
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{
	TCHAR GameName[100];
	CString strBuffer;
	wsprintf(GameName,TEXT("���%s��Ϸ�汾���ɣ���Ҫ�������������°汾��?"),m_pGameInfo->szGameName);
	if(iVersion != m_iVersion || iVersion2 != m_iVersion2)
	{
		TCHAR sz[200];
		wsprintf(sz,TEXT("�汾��ͻ,��ǰ�������汾��%d-%d,��İ汾��%d-%d"),iVersion,iVersion2,m_iVersion,m_iVersion2);
		//		WriteStr(sz);
	}	
	if(iVersion != m_iVersion)
	{
		if (IDOK == AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO))
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	if(iVersion2 != m_iVersion2)
	{
		if (IDOK == AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO))
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}
	return TRUE;
}

//����
BOOL CClientGameDlg::UserOutCard()
{
//	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
//	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
//	m_PlayView.m_btCue.ShowWindow(SW_HIDE);
//
//	//��������
//	OutCardStruct OutCard;
//	if(m_iBaseCount == 0 || m_bAutoCard)//�йܳ���
//	{
//		m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,OutCard.iCardList,
//			OutCard.iCardCount,m_iBigOutPeople==GetMeUserInfo()->bDeskStation);
//	}
//	else
//		OutCard.iCardCount=0;
//
//#ifdef AI_MACHINE
//	m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,OutCard.iCardList,
//		OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
//#endif
//
//	m_iNowOutPeople=-1;
//	OutCard.bIsReplace = false;
//	SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
//		MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
//	OnHitCard(0,0);
//	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	//add by ljl �йܳ��������⣬������ʾ���ڵ��ó�����Ϣ��
	
	KillAllTimer();
	OutCardStruct OutCard;
	OutCard.iCardCount = 0; 
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	SendMessage(IDM_CUE,0,0);

	if ( m_PlayView.m_btOutCard.IsWindowEnabled() )
	{
		SendMessage(IDM_OUT_CARD,1,1);
	}
	else
	{
		SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
			MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}

	return TRUE;
}

//������ҿ�ʼ
BOOL CClientGameDlg::ReplaceBegin(BYTE bDeskStation)
{
	if (!m_bWatchMode)
	{
		ReplaceAgreeStruct agree;
		agree.bDeskStation = bDeskStation;
		SendGameData(&agree,sizeof(agree),MDM_GM_GAME_NOTIFY,ASS_REPLACE_GM_AGREE_GAME,0);
	}
	return TRUE;
}

//������ҳ���
BOOL CClientGameDlg::ReplaceOutCard(BYTE bDeskStation)
{
//  	if (m_pUserInfo[bDeskStation])
// 		return TRUE;
	BYTE bDesk = ViewStation(bDeskStation);
	if(m_CardCount[bDesk] <= 0)
		return true;

	//��������
	ReplaceOutCardStruct OutCard;
	OutCard.bDeskStation = bDeskStation;
	OutCard.bIsReplace   = false;
	m_Logic.AutoOutCard(m_Card[bDesk],m_CardCount[bDesk],
		m_iBaseCard,m_iBaseCount,
		OutCard.iCardList,OutCard.iCardCount,
		m_iBigOutPeople == bDeskStation);
	//��ͨ��,�ǵ�һ����Ҵ������
	if(m_iFirstOutPeople != bDeskStation)
	{
		OutCard.iCardCount = 0;
	}

	SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
		MDM_GM_GAME_NOTIFY,ASS_REPLACE_OUT_CARD,0);
	return TRUE;
}

//������ҽз�
BOOL CClientGameDlg::ReplaceCallScore(BYTE bDeskStation)
{
	ReplaceCallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.CallScore = 0;						//����з�Ϊ0��,������
	score.bDeskStation = bDeskStation;	//�зֵ��˵�λ��
	score.CallScoreFlag=false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_REPLACE_CALL_SCORE,0);
	return true;
}

//��ʾ�û�����
void CClientGameDlg::ShowUserTalk(MSG_GR_RS_NormalTalk *pNormalTalk)
{
	//TCHAR szMsg[1000];
	////��������
	//UserItemStruct * pSendUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(pNormalTalk->dwSendID);
	//if (pSendUserItem==NULL) 
	//	return ;
	//if(pSendUserItem->GameUserInfo.bUserState==USER_WATCH_GAME)
	//	return ;
	//lstrcpy(szMsg,pNormalTalk->szMessage);	
	//m_PlayView.DisplayTalkMsg(ViewStation(pSendUserItem->GameUserInfo.bDeskStation),pNormalTalk->szMessage); //��ʾ������Ϣ	

	//m_InputMessage.SetFocus();
	//m_InputMessage.SetEditSel(m_InputMessage.GetWindowTextLength(),m_InputMessage.GetWindowTextLength());

	//return ;
}

//���������ϱ��
BOOL CClientGameDlg::SetNoteFlag(int iFlag)
{
	m_PlayView.m_GameNoteView.SetGameStateFlag(iFlag);
	CRect rect,clientrect;
	m_PlayView.GetClientRect(&rect);
	m_PlayView.m_GameNoteView.GetClientRect(&clientrect);
	int width = clientrect.right - rect.left ;
	int height = clientrect.bottom - rect.top ;
	int x = (rect.right - rect.left )/2 - width /2;
	int y = (rect.bottom - rect.top )/2 - height /2;

	m_PlayView.m_GameNoteView.MoveWindow(x,y,width,height);
	m_PlayView.m_GameNoteView.SetMessage(TEXT("ok"));
	return TRUE;
}

//��Ҷ���,���ж��Ƿ�Ϊ��ǰ������
BOOL CClientGameDlg::UserCut(BYTE bDeskStation)
{
	m_iAIStation[bDeskStation] = 1;
	//��ǰ������Ϊ��ǰ�з���
	if(m_bCurrentOperationStation == bDeskStation)
	{
		ReplaceCallScore(bDeskStation);
		return true;
	}

	//��ǰ������Ϊ��ǰ������
	if(m_iNowOutPeople == bDeskStation)
	{
		ReplaceOutCard(bDeskStation);
		return true;
	}
	return true;
}

//������
LRESULT	CClientGameDlg::OnRobNT(WPARAM wparam, LPARAM lparam)
{
	m_PlayView.m_btRobNT.ShowWindow(0);

	m_PlayView.m_btRobNT2.ShowWindow(0);

	RobNTStruct robnt;
	memset(&robnt,0,sizeof(robnt));
	robnt.bDeskStation = GetMeUserInfo()->bDeskStation;		
	robnt.iValue  = (int)wparam;	
	SendGameData(&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);
	return 0;
}

//��������
BOOL CClientGameDlg::ReplaceRobNT(BYTE bDeskStation)
{
	RobNTStruct robnt;
	memset(&robnt,0,sizeof(robnt));
	robnt.bDeskStation = bDeskStation;		
	robnt.iValue  = 0;//(int)wparam;	
	SendGameData(&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_REPLACE_ROB_NT,0);
	return true;
}
//�Ӱ�
LRESULT	CClientGameDlg::OnAddDouble(WPARAM wparam, LPARAM lparam)
{
	m_PlayView.m_btAddDouble.ShowWindow(0);
	m_PlayView.m_btAddDouble2.ShowWindow(0);

	AddDoubleStruct AddDouble;
	memset(&AddDouble,0,sizeof(AddDouble));
	AddDouble.bDeskStation = GetMeUserInfo()->bDeskStation;		
	AddDouble.iValue  = (int)wparam;	
	SendGameData(&AddDouble,sizeof(AddDouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);
	return 0;
}
//����
LRESULT	CClientGameDlg::OnShowCard(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_SHOW_CARD);
	m_PlayView.m_btShowCard.ShowWindow(0);
	m_PlayView.m_btShowCard2.ShowWindow(0);

	ShowCardStruct show;
	memset(&show,0,sizeof(show));
	show.bDeskStation = GetMeUserInfo()->bDeskStation;		
	show.iValue  = (int)wparam;	
	SendGameData(&show,sizeof(show),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD,0);
	return 0;
}
//����Ӱ�
BOOL CClientGameDlg::ReplaceAddDouble(BYTE bDeskStation)
{
	AddDoubleStruct AddDouble;
	memset(&AddDouble,0,sizeof(AddDouble));
	AddDouble.bDeskStation = bDeskStation;		
	AddDouble.iValue  = 0;//(int)wparam;	
	SendGameData(&AddDouble,sizeof(AddDouble),MDM_GM_GAME_NOTIFY,ASS_REPLACE_ADD_DOUBLE,0);
	return true;
}

//�������ְ�ť����
LRESULT	CClientGameDlg::OnLastTurn(WPARAM wparam, LPARAM lparam)
{
	////====ע����������д��벻֪����ʲô
	//if ((m_bShowIngLast)||(!m_bShowLast)||(m_iLastCardCount==0))//||(m_iNowOutPeople==m_iFirstOutPeople))
	//	return 0;
	m_bShowIngLast=TRUE;
	m_PlayView.m_btLastTurn.ShowWindow(SW_HIDE);

	SetTimer(ID_SHOW_LAST,3000,NULL);

	for (int i = 0;i < PLAY_COUNT; i ++)
	{	
		if(m_iLastCardCount[i] == 0)
		{
			m_PlayView.m_UserCard[ViewStation(i)].SetCard(NULL,NULL,0);
			//m_PlayView.SetPass(ViewStation(i),1);
		}
		else
		{
			m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_iLastOutCard[i],NULL,m_iLastCardCount[i]);
		}
	}
	return 0;
}

//��������(animalNameֵ��UpgradeMessage.h)
LRESULT	CClientGameDlg::BeginAnimal(DWORD animalName, BYTE card[]/* = NULL*/, int count/* = 0*/)
{
	int nElapse = 0;
	int nDesk = 0;
	EndAnimal(animalName);
	switch(animalName)
	{
	case ANIMAL_DZWIN:
		nElapse = 100;
		m_PlayView.m_AnimalResult.Create(&m_PlayView.m_ImgAnimalDZWin, 11);
		m_PlayView.m_AnimalResult.Begin(4000, nElapse, 0, 0);
		SetTimer(ID_ANIMAL_RESULT, nElapse, NULL);
		break;
	case ANIMAL_DZLOSE:
		nElapse = 200;
		m_PlayView.m_AnimalResult.Create(&m_PlayView.m_ImgAnimalDZLose, 4);
		m_PlayView.m_AnimalResult.Begin(4000, nElapse, 0, 0);
		SetTimer(ID_ANIMAL_RESULT, nElapse, NULL);
		break;
	case ANIMAL_SBOMB0:
	case ANIMAL_SBOMB1:
	case ANIMAL_SBOMB2:
		nElapse = 100;
		nDesk = animalName-ANIMAL_SBOMB0;
		m_PlayView.m_AnimalBomb[nDesk].Create(&m_PlayView.m_ImgAnimalSmallBomb, 10);
		m_PlayView.m_AnimalBomb[nDesk].Begin(1000, nElapse, m_PlayView.m_PtSBomb[nDesk].x, m_PlayView.m_PtSBomb[nDesk].y);
		SetTimer(ID_ANIMAL_BOMB0+nDesk, nElapse, NULL);
		break;
	case ANIMAL_BBOMB0:
	case ANIMAL_BBOMB1:
	case ANIMAL_BBOMB2:
		{
			nElapse = 100;
			nDesk = animalName-ANIMAL_BBOMB0;
			//if (!m_PlayView.m_AnimalBigBomb.IsValid())
			{
				AnimalStruct aniStruct[2] = {0};
				//����
				aniStruct[0].nFrame = 6;
				aniStruct[0].pImage = &m_PlayView.m_ImgAnimalBBFire;
				//����
				aniStruct[1].nFrame = 3;
				aniStruct[1].pImage = &m_PlayView.m_ImgAnimalBBStar;

				m_PlayView.m_AnimalBigBomb.Create(aniStruct, 2);
				//��
				m_PlayView.m_AnimalBigBomb.m_CardAnimal.Create(&m_PlayView.m_ImgAnimalBBCardNormal, &m_PlayView.m_ImgAnimalBBCardBack);
			}
			AnimalShowAttriStruct asaStruct[2] = {0};
			int nBeginTime[2] = {0};
			int nEndTime[2] = {0};
			//����
			asaStruct[0].nElapse = 100;
			asaStruct[0].nTime = 600;
			asaStruct[0].xPos = 0;
			asaStruct[0].yPos = 0;
			nBeginTime[0] = 0;
			nEndTime[0] = 600;
			//����
			asaStruct[1].nElapse = 300;
			asaStruct[1].nTime = 900;
			asaStruct[1].xPos = 0;
			asaStruct[1].yPos = 0;
			nBeginTime[1] = 600;
			nEndTime[1] = 1500;

			m_PlayView.m_AnimalBigBomb.Begin(asaStruct, nBeginTime, nEndTime, 2);
			m_PlayView.m_AnimalBigBomb.BeginCard(600, 1600, CPoint(0,0), CPoint(0,0), 1, card, count);
			SetTimer(ID_ANIMAL_BIGBOMB0+nDesk, nElapse, NULL);
		}
		break;
	default:
		break;
	}
	return ER_OK;
}


//���ݴ��붯���������������ܵ�ס�Ķ���
LRESULT CClientGameDlg::EndAnimal(DWORD animalName)
{
	int nDesk = -1;
	if (animalName >= ANIMAL_SBOMB0 && animalName <= ANIMAL_SBOMB0+PLAY_COUNT)
	{
		nDesk = animalName - ANIMAL_SBOMB0;
		m_PlayView.m_AnimalBomb[nDesk].End();
	}
	else if (animalName == ANIMAL_DZLOSE || animalName == ANIMAL_DZWIN)
	{
		nDesk = 1;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			m_PlayView.m_AnimalBomb[i].End();
		}
		m_PlayView.m_AnimalBigBomb.End();
	}
	else if (animalName >= ANIMAL_BBOMB0 && animalName <= ANIMAL_BBOMB0+PLAY_COUNT)
	{
		nDesk = animalName - ANIMAL_BBOMB0;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			m_PlayView.m_AnimalBomb[i].End();
		}
	}
	if (nDesk != -1)
	{
		m_PlayView.UpdateViewFace(NULL);
		return TRUE;
	}
	return FALSE;
}

void CClientGameDlg::SetIntToCharLjl(CString  &str, __int64 iMoney, int iSet)
{
	CString strTmpe("0");
	str.Format("%d",iMoney);
	for ( int i=0; i<iSet; i++  )
	{
		str+= strTmpe;
	}
}

//Ϊ��Ƶģ�����ش˺���
#ifdef VIDEO
bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}
	
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
				//
//				TCHAR sz[200];
			//	sprintf(sz,"��Ƶ��ʼ���ɹ�!");
			///	Writelog(sz,1);
				///������Ƶ������
				
				pPlayVideo->Connect();
			
			}
			else
			{
				pPlayVideo->Video_Release();
			/*	sprintf(sz,"��Ƶ��ʼ��ʧ��!");
				Writelog(sz,1);*/
				return 0;
			}


		} 
	}
 	return TRUE;
}



bool CClientGameDlg::AFCCloseFrame()
{
	 ///�ͷ���Ƶ��Դ 
	if( NULL != CPlayVideo::GetPlayVideo())
		CPlayVideo::GetPlayVideo()->Video_Release(); 
	return __super::AFCCloseFrame();
}
#endif
