#include "StdAfx.h"
#include "UserPower.h"
#include "GameMainManage.h"

#include "GameRoomMessage.h"

#include "writelog.h"
#include "commonuse.h"

#include "AFCException.h"

#include "AutoAllotDesk.h"

// ��Ϊ��̬��Ա������ʹ�����ڽ��̽���ʱ��ʽ����CBcfFile::ClearMap()�������ͷţ�����MFC�ᱨ�ڴ�й©
//MANAGE_EXT_CLASS std::map<CString, CBcfFileNameItem*> CBcfFile::mapFile;
//MANAGE_EXT_CLASS std::map<CString, FileItem*> CUnZipRes::mapFile;
//unsigned char CUnZipRes::m_bPassWord[MAX_PASSWORD_LENGTH];


//#include "ItemDataDBMessage.h"
//�궨��
#define IDT_WRITE_SERVER_INFO						7					//���·�������Ϣ��ʱ��
#define IDT_CHECK_DATA_CONNECT						2					//����������Ӷ�ʱ��
#define IDT_UPDATE_ROOM_PEOPLE						3					//ǿ�Ƹ��·����û���������

#define IDT_QUEUE_GAME								4					/// �Ŷӻ���ʱ��
#define IDT_CONTEST_GAMEOVER						5
#define IDT_LEASE_TIMEOVER							6					///���޷���

#define IDT_CONTEST_KICK						1000

#define VIP_CHECK_TIME								60000				//1���Ӽ��һ��
#define EXPERCISE_MONEY						5000000						//���ѵ������Ǯ��

//extern void DebugPrintf(const char *p, ...);
/// ���������Ϣ
//static void DebugPrintf(const char *p, ...)
//{
//	char szFilename[256];
//	sprintf( szFilename, "E:\\bzw\\DebugInfo\\WServer.txt");
//	FILE *fp = fopen( szFilename, "a" );
//	if (NULL == fp)
//	{
//		return;
//	}
//	va_list arg;
//	va_start( arg, p );
//	fprintf(fp,"[%s]--", CTime::GetCurrentTime().Format("%H:%M:%S"));
//	vfprintf(fp, p, arg );
//	fprintf(fp,"\n");
//	fclose(fp);
//}
/// ��ӡ������Ϣ


static char *G_GetAppPath()
{
	static char s_Path[MAX_PATH];
	bool s_bIsReady = false;
	if (!s_bIsReady)
	{
		ZeroMemory(s_Path,sizeof(s_Path));
		DWORD dwLength=GetModuleFileNameA(GetModuleHandle(NULL), s_Path, sizeof(s_Path));
		char *p = strrchr(s_Path, '\\');
		if(p != NULL)	*p = '\0';
		s_bIsReady = true;
	}
	return s_Path;
}
static void DebugPrintf(const char *p, ...)
{
	char szTimeStr[32];
	char szDateStr[32];
	char szFilename[256];
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	GetTimeFormatA( LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &sysTime, "HH:mm:ss", szTimeStr, 32);
	GetDateFormatA( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, "yyyy-MM-dd", szDateStr, 32);
	wsprintf( szFilename, TEXT("%s\\Log\\%s_GameMainManage.txt"), G_GetAppPath(), szDateStr);
	va_list arg;
	va_start( arg, p );
	FILE *fp = NULL;
	fp = fopen(szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	fprintf(fp,"[%s %s]--", szDateStr, szTimeStr);
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}

static void DebugPrintf1(int roomid, const char *p, ...)
{
	char szTimeStr[32];
	char szDateStr[32];
	char szFilename[256];
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	GetTimeFormatA( LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &sysTime, "HH:mm:ss", szTimeStr, 32);
	GetDateFormatA( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, "yyyy-MM-dd", szDateStr, 32);
	wsprintf( szFilename, TEXT("%s\\Log\\%s_%d_GameMainManage.txt"), G_GetAppPath(), szDateStr, roomid);
	va_list arg;
	va_start( arg, p );
	FILE *fp = NULL;
	fp = fopen(szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	fprintf(fp,"[%s %s]--", szDateStr, szTimeStr);
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}

/*******************************************************************************************************/


//ע���û���½
//���캯��
CGameMainManage::CGameMainManage(void)
{
	m_bStopLogon=false;
	m_pDesk=NULL;
	m_uDeskCount=0;
	m_uMatchUserCount=0;
	m_pMatchUser=NULL;
	m_pMatchInfo=NULL;
	m_pDeskArray=NULL;
	m_szMsgRoomLogon[0]=0;
	m_szMsgGameLogon[0]=0;
//	m_ItemDate = NULL;
	m_PropService=NULL;
	m_pBankService = NULL;
	m_lVIPID = -1;  ///< ���÷��������ID
	m_uVIPIndex = 0;
	memset(m_szVIPPassWord,0,sizeof(m_szVIPPassWord));
	
	CBcfFile f(CBcfFile::GetAppPath() + "Function.bcf");
	CString strValue = f.GetKeyVal("TaxRate","Rate","100");
	m_nRate = atoi(strValue);
	strValue = f.GetKeyVal("PresentCoinPerDay","Available","0");
	m_bPresentCoin = (atoi(strValue)!=0);

	m_pIAutoAllotDesk = NULL;
}

//��������	
CGameMainManage::~CGameMainManage(void)
{
	m_uDeskCount=0;
	SafeDeleteArray(m_pDesk);
	SafeDeleteArray(m_pMatchUser);
	SafeDeleteArray(m_pMatchInfo);
	SafeDeleteArray(m_pDeskArray);
	//if(m_ItemDate) 
	//	delete m_ItemDate;
	if(m_PropService)
		delete m_PropService;
	if (m_pBankService)
		delete m_pBankService;
	m_pGameUserInfoList.clear();
}

//���ݹ���ģ���ʼ��
bool CGameMainManage::OnInit(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)
{
	try
	{
		DebugPrintf1(m_InitData.uRoomID, "���gOnInit()");
		//��ʼ���û�����Ϸ����
		m_uMatchUserCount=0;
		m_szMsgRoomLogon[0]=0;
		m_szMsgGameLogon[0]=0;
		m_RoomLimited.RemoveAll();
		m_GameLimited.RemoveAll();
		lstrcpy(m_szMsgRoomLogon,TEXT("��ӭ�㣬ף������������ÿ��ģ����ִ����￪ʼ��"));
		lstrcpy(m_szMsgGameLogon,TEXT("��ӭ�㣬ף������������ÿ��ģ����ִ����￪ʼ��"));

		//m_ItemDate=new CAFCItemData((CWnd*)this);

		m_PropService=new CPropService((CWnd*)this);
		m_pBankService = new CBankService((CWnd*)this);

		if (m_UserManage.Init(pInitData->uMaxPeople,pInitData->uMaxPeople,pInitData->uVipReseverCount)==false) return false;
		if (InitGameDesk(pInitData->uDeskCount, pInitData->uDeskType)==false) return false;//�ټ���lym

		for(int i=0; m_InitData.uVirtualUser,i<(m_InitData.uVirtualUser/m_KernelData.uDeskPeople); i++) ///< ����W������������������������ӡ�
		{
			CGameDesk * pDesk = *(m_pDesk + i);
			pDesk->VirtualLockDesk();
		}


		CString _NameID;
		_NameID.Format("%d", pKernelData->uNameID);
		CBcfFile fConfig(CBcfFile::GetAppPath()+"SpecialRule.bcf");
		//pKernelData->uMinDeskPeople = pKernelData->uDeskPeople;
		pKernelData->uMinDeskPeople = fConfig.GetKeyVal("MinDeskPeople", _NameID, (int)pKernelData->uDeskPeople);

		CBcfFile f(CBcfFile::GetAppPath()+"SpecialRule.bcf");
		m_iQueueTime = f.GetKeyVal("Queuer","QueuerTime", 10);

		if (IsQueueGameRoom())
		{
			m_pIAutoAllotDesk = CreateAutoAllotDesk(m_KernelData.uDeskPeople, m_InitData.uDeskCount);
			m_pQueueUserSitBuffer = new BYTE[(pKernelData->uDeskPeople+1)*sizeof(int)];
		}

		return true;
	}
	catch (CAFCException * pException)	{ pException->Delete();TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);	}
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	return false;
}

//���ݹ���ģ��ж��
bool CGameMainManage::OnUnInit()
{
	DebugPrintf1(m_InitData.uRoomID, "���gOnUnInit()");
		/// ������Զ������ķ��䣬���ⴴ���Ķ�����Ҫ�ͷ�
	if (IsQueueGameRoom())
	{
		SafeDeleteArray(m_pQueueUserSitBuffer);
		DestroyAutoAllotDesk(m_pIAutoAllotDesk);
	}

	for(int i=0;i<m_UserManage.GetOnLineUserCount();i++)
	{
		CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(i);
		if (pUserInfo!=NULL) 
		{
			CleanUserInfo(pUserInfo);
		}

	}
	
	//��������
	m_uDeskCount=0;
	m_uMatchUserCount=0;
	m_szMsgRoomLogon[0]=0;
	m_szMsgGameLogon[0]=0;
	SafeDeleteArray(m_pDesk);
	SafeDeleteArray(m_pMatchInfo);
	SafeDeleteArray(m_pDeskArray);
	SafeDeleteArray(m_pMatchUser);
	m_UserManage.UnInit();
	m_RoomLimited.RemoveAll();
	m_GameLimited.RemoveAll();

	return true;
}

//���ݹ���ģ������
bool CGameMainManage::OnStart()
{

	HANDLE hDatabase=m_SQLDataManage.hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_RoomStartProcess");
	if(hRecord<(HANDLE)0x1000)
		return false;

	sqlSPAddPara(hDatabase,hRecord,"@RoomID",m_InitData.uRoomID);
	sqlSPAddPara(hDatabase,hRecord,"@LockTable",m_InitData.szLockTable,sizeof(m_InitData.szLockTable));

	sqlSPExec(hDatabase,hRecord,true);

	if(!sqlEndOfFile(hDatabase,hRecord))
	{
		sqlGetValue(hDatabase,hRecord,"RoomWelcome",m_szMsgRoomLogon,sizeof(m_szMsgRoomLogon));
		sqlGetValue(hDatabase,hRecord,"RallWelcome",m_szMsgGameLogon,sizeof(m_szMsgGameLogon));
	}
	sqlCloseRecord(hDatabase,hRecord);

	DebugPrintf1(m_InitData.uRoomID, "���gOnStart()");
	SetTimer(IDT_WRITE_SERVER_INFO,60000L);
	SetTimer(IDT_CHECK_DATA_CONNECT,60000L);

	//���Ŷӻ����䣬�����Ŷӻ���ʱ��
	if (IsQueueGameRoom())
	{
		SetTimer(IDT_QUEUE_GAME, m_iQueueTime * 1000);
	}

	if (m_InitData.dwRoomRule & GRR_CONTEST)
	{
		SetTimer(IDT_CONTEST_GAMEOVER, 10000);
	}

	if (m_InitData.iLeaseID != 0)
	{
		SetTimer(IDT_LEASE_TIMEOVER, 10000);
	}

	return true;
}

HANDLE CGameMainManage::sqlSPSetNameEx(HANDLE &hHandle,LPCTSTR szSPName,bool bReturnValue)
{

	bool bIsDBNative = false;;
	if(hHandle == m_SQLDataManage.hDBNative)
	{
		bIsDBNative = true;
	}

	if (bIsDBNative)
	{
		hHandle = m_SQLDataManage.hDBNative;
	}
	else
	{
		hHandle = m_SQLDataManage.hDBLogon;
	}

	if (!sqlExec(hHandle,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_SQLDataManage.SQLConnectReset();

	}

	return  sqlSPSetName(hHandle,szSPName,bReturnValue);

}

//���ݹ���ģ��ر�
bool CGameMainManage::OnStop()
{
	//ɾ����ʱ��
	//KillTimer(IDT_RE_WRITE_DATA);

	DebugPrintf1(m_InitData.uRoomID, "���gOnStop()");
	KillTimer(IDT_WRITE_SERVER_INFO);
	KillTimer(IDT_CHECK_DATA_CONNECT);
	//KillTimer(ID_CHECK_VIP_TIMER);
	KillTimer(IDT_QUEUE_GAME);
	return true;
}

/// GM��Ϣ����ģ��
bool CGameMainManage::OnGMSocketMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if (pNetHead->bAssistantID == ASS_GM_MESSAGE_PRINTLOG) //��ӡ�����־
	{
		GM_MSG_PrintLog_t* pPrintLogCmd = (GM_MSG_PrintLog_t*)pData;
		if (m_InitData.uRoomID == pPrintLogCmd->uRoomID)
		{
			PrintUsersLog();
		}
		return true;
	}

	if(pNetHead->bAssistantID != ASS_GM_MESSAGE_OPERATE_ACT)
	{
		return true;
	}
	//Ч������
	if ((NULL == pData)||(uSize!=sizeof(GM_OperateByID_t))) return false;///��֤�ṹ�Ƿ�һ��

	/// ��֤������Ȩ��
	CGameUserInfo * pOperator=m_UserManage.GetOnLineUserInfo(uIndex);
	if ((NULL == pOperator) || (pOperator->m_UserData.bGameMaster==0))
	{
		return false;
	}

	//����
	GM_OperateByID_t * pOperation=(GM_OperateByID_t *)pData;
	//DebugPrintf("����[%d]���[%d]ʱ��[%d]", pOperation->iOperation, pOperation->iUserID, pOperation->iWorkMinutes);

	/// �ҵ��û�
	CGameUserInfo *pUserInfo = this->m_UserManage.FindOnLineUser(pOperation->iUserID);
	if (NULL == pUserInfo)
	{
		pUserInfo = this->m_UserManage.FindNetCutUser(pOperation->iUserID);
		if (NULL == pUserInfo)
		{
			return true;
		}
	}
	/// ��������ڴ�ֱ�ӷ���
	if (pOperation->iWorkMinutes == 0)
	{
		pUserInfo->m_UserData.userInfoEx2 = 0;
		return true;
	}
	switch(pOperation->iOperation)
	{
	case GM_ACT_AWORD:		//1	///< �������������浽�����У���ʵʱд���û�Ǯ���������Ϣ���͵�Z������
		break;
	case GM_ACT_NO_TALK:	//2	///< ���ԣ���������������ǰ��ĳ�����У�GM�����߿ͻ�����ͬʱ���������ڵ�W��������������Ϣ
		{
			/// ִ�н���
			CTime tm = CTime::GetCurrentTime();
			tm += pOperation->iWorkMinutes*60;
			CString strTime;
			strTime.Format("%d", tm);
			pUserInfo->m_UserData.userInfoEx2 = atoi(strTime);
			break;
		}
	case GM_ACT_KICK_OUT:	//3	///< �߳����䣬��������������ǰ��ĳ�����У�GM�����߿ͻ�����ͬʱ���������ڵ�W��������������Ϣ
	case GM_ACT_BANDON_ID:	//4	///< ��ֹ��¼������ţ���������������ǰ��ĳ�����У�GM�����߿ͻ���ͬʱ���������ڵ�W��������������Ϣ
		{
			bool bAlreadyCut = false;
			CGameUserInfo *pUserInfo = this->m_UserManage.FindOnLineUser(pOperation->iUserID);
			if (NULL == pUserInfo)
			{
				bAlreadyCut = true;
				pUserInfo = this->m_UserManage.FindNetCutUser(pOperation->iUserID);
				if (NULL == pUserInfo)
				{
					break;
				}
			}
			//m_TCPSocket.SendData(pUserInfo->m_uSocketIndex, MDM_GP_PLAYER_2_GM, ASS_GR_MSG_BE_BANDID, 0, pUserInfo->m_dwHandleID);
			/// һ�㲻�����ǽ����������Ϊ���߳����ŵģ������ܵ�¼��W������
			if (pOperation->iWorkMinutes>0)
			{
				/// �ж��Ƿ��Ѿ�����Ϸ��
				if (pUserInfo->m_UserData.bDeskNO != 255)
				{
					CGameDesk *pDesk = *(m_pDesk+pUserInfo->m_UserData.bDeskNO);
					// ��������ֶΣ����ڱ�־λ�����λ��1����ʱ��ֵ���������ô�����
					pUserInfo->m_UserData.userInfoEx2 |= 0x80000000;
					//if ((pUserInfo->m_UserData.bUserState == USER_PLAY_GAME) || (pUserInfo->m_UserData.bUserState == USER_CUT_GAME))
					if(pDesk->IsPlayingByGameStation())
					{
						/// ������������Ϸ�����Ѿ����ߣ�������Ҷ��ߣ�����Ϸ�������Զ�����������
						if ((pUserInfo->m_UserData.bUserState == USER_PLAY_GAME) || (pUserInfo->m_UserData.bUserState == USER_CUT_GAME))
						{
							if (!bAlreadyCut)
							{
								m_UserManage.RegNetCutUser(pUserInfo);
							}
							pDesk->UserNetCut(pUserInfo->m_UserData.bDeskStation, pUserInfo);
						}
						else
						{
							pDesk->WatchUserLeftDesk(pUserInfo->m_UserData.bDeskStation,pUserInfo);
							//�����û�����
							CleanUserInfo(pUserInfo);
							m_UserManage.FreeUser(pUserInfo, false);
							m_TCPSocket.CloseSocket(pUserInfo->m_uSocketIndex, 0);
						}
					}
					else
					{
						pDesk->UserLeftDesk(pUserInfo->m_UserData.bDeskStation,pUserInfo);
						//�����û�����
						CleanUserInfo(pUserInfo);
						m_UserManage.FreeUser(pUserInfo, false);
						m_TCPSocket.CloseSocket(pUserInfo->m_uSocketIndex, 0);
					}
				}
				else
				{
					//�����û�����
					CleanUserInfo(pUserInfo);
					m_UserManage.FreeUser(pUserInfo, false);
					m_TCPSocket.CloseSocket(pUserInfo->m_uSocketIndex, 0);
				}
			}
			break;
		}
	default:
		break;
	}
	return true;
}

//SOCKET ���ݶ�ȡ
bool CGameMainManage::OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	/// {{Added by zxd 20100722 GM��Ϣ�������
	if (MDM_GAMEMASTER_MESSAGE == pNetHead->bMainID)
	{
		return OnGMSocketMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
	}
	/// Added by zxd 20100722}}
	try
	{
	switch (pNetHead->bMainID)
	{
		//�źż���
	case MDM_GR_NETSIGNAL:
		{
			m_TCPSocket.SendData(uIndex,pData,uSize,MDM_GR_NETSIGNAL,0,0,dwHandleID);
			return true;
		}
	case MDM_GR_LOGON:			//�����½
		{
			return OnUserLogonMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
	case MDM_GR_USER_ACTION:	//�û�����
		{
			return OnUserActionMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
	case MDM_GR_ROOM:			//������Ϣ
		{
			return OnRoomMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
	case MDM_GR_MANAGE:			//������Ϣ
		{
			return OnManageMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
	case MDM_GR_MONEY:	//Ǯ����
		{
			return OnAboutMoney(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
	case MDM_GM_GAME_FRAME:		//�����Ϣ
	case MDM_GM_GAME_NOTIFY:	//��Ϸ��Ϣ
		{
			//Ч���û�
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
			{
				DebugPrintf("CGameMainManage::OnSocketRead()δ�ҵ��������[uIndex=%d][%d��]", uIndex, __LINE__);
				return false;
			}
			if ((pUserInfo->m_UserData.bDeskNO>=m_uDeskCount)||(pUserInfo->m_UserData.bDeskStation>=m_KernelData.uDeskPeople))
			{
				DebugPrintf("CGameMainManage::OnSocketRead()����[%d]����λ��[%d]�Ƿ�[%d��]",
					pUserInfo->m_UserData.bDeskNO,
					pUserInfo->m_UserData.bDeskStation,
					__LINE__);
				return true;
			}

			//�������� 
			bool bWatchUser=(pUserInfo->m_UserData.bUserState==USER_WATCH_GAME);
			CGameDesk * pGameDesk=*(m_pDesk+pUserInfo->m_UserData.bDeskNO);
			bool bRet = false;
			TCHAR szFunctionName[16];
			if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
			{
				_tcscpy(szFunctionName, "Notify");
				bRet = pGameDesk->HandleNotifyMessage(pUserInfo->m_UserData.bDeskStation,pNetHead,pData,uSize,uIndex,bWatchUser);
			}
			else if (pNetHead->bMainID==MDM_GM_GAME_FRAME)
			{
				_tcscpy(szFunctionName, "Frame");
				bRet = pGameDesk->HandleFrameMessage(pUserInfo->m_UserData.bDeskStation,pNetHead,pData,uSize,uIndex,bWatchUser);
			}
			if (!bRet)
			{
				DebugPrintf("CGameMainManage::OnSocketRead()��pGameDesk->Handle%sMessage()����ֵΪfalse[bMainID=%d, bAssID=%d][uIndex=%d][%d��]",
					szFunctionName, pNetHead->bMainID, pNetHead->bAssistantID, uIndex, __LINE__);
			}

			return bRet;		
		}
	case MDM_GR_MESSAGE:	//����ϵͳ��Ϣ
		{
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pData;
			if ((uSize<=(sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)))||(uSize>sizeof(MSG_GR_RS_NormalTalk))) return false;
			if ((pNormalTalk->iLength+1)!=(uSize+sizeof(pNormalTalk->szMessage)-sizeof(MSG_GR_RS_NormalTalk))) return false;

			MSG_GA_S_Message Message;
			memset(&Message,0,sizeof(Message));

			//��������
			Message.bFontSize=0;
			Message.bCloseFace=0;
			Message.bShowStation=pNormalTalk->szMessage[0]==64?2:1;
			if (m_TalkFilter.IsAllowableTalk(pNormalTalk->szMessage,lstrlen(pNormalTalk->szMessage)))
			{
				lstrcpy(Message.szMessage,pNormalTalk->szMessage);
			}
			else
			{
				const TCHAR* pTemTalk = m_TalkFilter.GetReplaceContent();
				strcpy(Message.szMessage,pTemTalk);

			}

			if (pNetHead->bAssistantID==ASS_GR_SYSTEM_MESSAGE)
				m_TCPSocket.SendDataBatch(&Message,sizeof(Message),MDM_GR_MESSAGE,ASS_GR_SYSTEM_MESSAGE,0);
			return true;
		}
	case MDM_GM_MESSAGE:	//����ϵͳ��Ϣ
		{
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pData;
			if ((uSize<=(sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)))||(uSize>sizeof(MSG_GR_RS_NormalTalk))) return false;
			if ((pNormalTalk->iLength+1)!=(uSize+sizeof(pNormalTalk->szMessage)-sizeof(MSG_GR_RS_NormalTalk))) return false;

			MSG_GA_S_Message Message;
			memset(&Message,0,sizeof(Message));

			//��������
			Message.bFontSize=0;
			Message.bCloseFace=0;
			Message.bShowStation=1;
			if (m_TalkFilter.IsAllowableTalk(pNormalTalk->szMessage,lstrlen(pNormalTalk->szMessage)))
			{
				lstrcpy(Message.szMessage,pNormalTalk->szMessage);
			}
			else
			{
				const TCHAR* pTemTalk = m_TalkFilter.GetReplaceContent();
				strcpy(Message.szMessage,pTemTalk);
			}

			if (pNetHead->bAssistantID==ASS_GM_SYSTEM_MESSAGE)
				m_TCPSocket.SendDataBatch(&Message,sizeof(Message),MDM_GM_MESSAGE,ASS_GM_SYSTEM_MESSAGE,0);

			return true;
		}
	case MDM_GR_POINT://�����
		{
			return OnPointMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
		//�����ӹ���2008.3.5
		//case MDM_GR_BROADCAST_MESSAGE://С����
		//	{
		//		return OnBroadcastMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		//	}
		//case MDM_GR_UPDATE_HOLD_ITEM_DATA://���µ���
		//	{
		//		return OnUpdateUserHoldItemData(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		//	}
	case MDM_GR_FRIEND_MANAGE://����
		{
			return OnFriendManageMsg(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}

	case MDM_GR_QUEUE_MSG://�Ŷ�
		{
			return OnQueueMsg(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		}
	//case MDM_GR_ITEM_DATA_MSG://���������Ϣ
	//	{
	//		if(m_ItemDate!=NULL)
	//			return m_ItemDate->OnItemDataMsg(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
	//		return true;
	//	}
	//case  MDM_GR_VIP_MSG://vip����
	//	{
	//		if(m_ItemDate!=NULL)
	//			return m_ItemDate->OnVipMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
	//	}
	//vip����Ҳ����PROP��
	case MDM_GR_PROP:
		{
            // PengJiLin, 2010-9-13, ���˿����˹���
            if(ASS_PROP_NEW_KICKUSER == pNetHead->bAssistantID)
            {
                return OnNewKickUserProp(pNetHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
            }

			if(m_PropService)
			{				
				return m_PropService->OnNetMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
			}
			return true;
		}
	case MDM_BANK:
		{
			if (m_pBankService)
			{
				return m_pBankService->OnNetMessage(pNetHead, pData, uSize, uAccessIP, uIndex, dwHandleID);
			}
		}
	}
	}
	catch (...)
	{
		DebugPrintf("GameMainManage OnSocketRead�쳣[bMainID = %d][%d��]", pNetHead->bMainID, __LINE__);
		return false;
	}
	return false;
}



//������½������½ÿ�����䣩

//�û���������
bool CGameMainManage::OnUserActionMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	TRACE("USER ACTION MESSAGE:%d with %d\n",pNetHead->bMainID,pNetHead->bAssistantID);

	if(pNetHead->bMainID!=MDM_GR_USER_ACTION)
		return false;
	
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_USER_HIT_BEGIN:
		{
			MSG_GM_WatchSet UserAgreeID;
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if(pUserInfo!=NULL)
			{
				pUserInfo->m_UserData.bUserState=USER_DESK_AGREE;
				UserAgreeID.dwUserID = pUserInfo->m_UserData.dwUserID;
				m_TCPSocket.SendDataBatch(&UserAgreeID,sizeof(MSG_GM_WatchSet),MDM_GR_USER_ACTION,ASS_GR_USER_HIT_BEGIN,0);
				return true;
			}
			else
				return false;
		}
	case ASS_GR_SET_TABLE_BASEPOINT:
		{
			if(uSize!=sizeof(MSG_GR_S_SetDeskBasePoint)) return false;
			MSG_GR_S_SetDeskBasePoint* setBasePoint=(MSG_GR_S_SetDeskBasePoint*)pData;

			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if(pUserInfo->m_UserData.bDeskNO!=255)
			{
				CGameDesk * pNewDesk=*(m_pDesk+pUserInfo->m_UserData.bDeskNO);
				if(pNewDesk->GetTableOwnerID()==setBasePoint->dwUserID)//����
				{
					setBasePoint->bDeskOwner=1;
					pNewDesk->SetTableBasePoint(setBasePoint->iDeskBasePoint);
				}
			}else 
				setBasePoint->bDeskOwner=0;
			if(pUserInfo==NULL)
			{
				//����ʧ����Ϣ
				m_TCPSocket.SendData(uIndex,MDM_GR_USER_ACTION,ASS_GR_SET_TABLE_BASEPOINT,0,dwHandleID);
				return false;
			}
			else 
			{
				pUserInfo->m_UserData.uDeskBasePoint=setBasePoint->iDeskBasePoint;
				m_TCPSocket.SendDataBatch(setBasePoint,uSize,MDM_GR_USER_ACTION,ASS_GR_SET_TABLE_BASEPOINT,1);
			}
			return true;
		}
	case ASS_GR_USER_UP:		//�û�����
	case ASS_GR_WATCH_UP:		//�Թ�����
		{
			//Ч�����ݺϷ���
			if (uSize!=0) return false;
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;

			//�ж��Ƿ�����뿪
			if (pUserInfo->m_UserData.bDeskNO!=255)
			{

				if (IsUserPlayGame(pUserInfo)==true) 
				{
					m_TCPSocket.SendData(uIndex,MDM_GR_USER_ACTION,ASS_GR_SIT_ERROR,ERR_GR_CAN_NOT_LEFT,dwHandleID);
					return true;
				}
				UserLeftDesk(pUserInfo);

			}
			return true;
		}
	case ASS_GR_USER_SIT:		//�û�������Ϸ
	case ASS_GR_WATCH_SIT:		//�û��Թ���Ϸ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_UserSit)) return false;
			MSG_GR_S_UserSit * pUserSitInfo=(MSG_GR_S_UserSit *)pData;
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;
			if ((pUserSitInfo->bDeskIndex>=m_uDeskCount)||(pUserSitInfo->bDeskStation>=m_KernelData.uDeskPeople)) return false;

			//�����Ƿ���������Ϣ
			if ((pUserInfo->m_UserData.bDeskNO==pUserSitInfo->bDeskIndex)&&
				(pUserInfo->m_UserData.bDeskStation==pUserSitInfo->bDeskStation)) return true;

			//�����û��뿪ԭλ��
			if (pUserInfo->m_UserData.bDeskNO!=255)
			{
				if (IsUserPlayGame(pUserInfo)==true)
				{
					if(m_InitData.uComType==TY_MATCH_GAME) return true;
					m_TCPSocket.SendData(uIndex,MDM_GR_USER_ACTION,ASS_GR_SIT_ERROR,ERR_GR_CAN_NOT_LEFT,dwHandleID);
					return true;
				}
				UserLeftDesk(pUserInfo);
			}

			//�����û�����λ��
			BYTE bErrorCode=ERR_GR_ERROR_UNKNOW;
			CGameDesk * pNewDesk=*(m_pDesk+pUserSitInfo->bDeskIndex);
			/// {{ added by zxd 20100902 ��������������ˣ���������
			if (pNewDesk->IsVirtualLock())
			{
				m_TCPSocket.SendData(uIndex,MDM_GR_USER_ACTION,ASS_GR_SIT_ERROR,ERR_GR_ALREAD_USER,dwHandleID);
				return true;
			}
			/// added by zxd 20100902 ��������������ˣ���������}}
			if (pNetHead->bAssistantID==ASS_GR_USER_SIT) 
				bErrorCode=pNewDesk->UserSitDesk(pUserSitInfo,pUserInfo);
			else 
				bErrorCode=pNewDesk->WatchUserSitDesk(pUserSitInfo,pUserInfo);

			if (bErrorCode!=ERR_GR_SIT_SUCCESS)	
				m_TCPSocket.SendData(uIndex,MDM_GR_USER_ACTION,ASS_GR_SIT_ERROR,bErrorCode,dwHandleID);

			return true;
		}
	case ASS_GR_JOIN_QUEUE:	///<���û������Ŷӻ�
		{
			///�����Ŷӻ�����
			if (!IsQueueGameRoom())
			{
				return true;
			}

			if (uSize!=0) 
			{
				return false;
			}

			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) 
			{
				return false;
			}

		//	if (pUserInfo->m_UserData.bUserState == USER_LOOK_STATE)//û����������
			{
				m_pGameUserInfoList.remove(pUserInfo);		//ɾ�������Ŷӵ���ң���ֹͬһ�û��ŶӶ��
				m_pGameUserInfoList.push_back(pUserInfo);
			}

			AutoAllotDeskAddUser(pUserInfo);

			return true;
		}
	case ASS_GR_QUIT_QUEUE:///<�����뿪�Ŷӻ�
		{
			///�����Ŷӻ�����
			if (!IsQueueGameRoom())
			{
				return true;
			}

			if (uSize!=0) 
			{
				return false;
			}

			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) 
			{
				return false;
			}

			//int dwUserID = pUserInfo->m_UserData.dwUserID;

			m_pGameUserInfoList.remove(pUserInfo);	//ɾ�������Ŷӵ����

			AutoAllotDeskDeleteUser(pUserInfo);

			return true;
		}
	}
	return false;
}

//�ҳ������������
/*CGameDesk * CGameMainManage::findBestDesk()
{
TCHAR sz[50];

int maxPeople=0,maxPeopleDesk=0;
for(int i=0;i<m_uDeskCount;i++)
{
//wsprintf(sz,"%d",maxPeopleDesk);

CGameDesk * pNewDesk=*(m_pDesk+i);
int uSitCount=0;
for (BYTE j=0;j<pNewDesk->m_bMaxPeople;j++)
{
if (pNewDesk->m_pUserInfo[j]==NULL) 
continue;
else 
{
BYTE temp=pNewDesk->m_pUserInfo[j]->m_UserData.bUserState;
if ((temp==USER_LOOK_STATE)||(temp==USER_SITTING)||(temp==USER_ARGEE)) uSitCount++;
}
}
if(uSitCount>maxPeople&&uSitCount!=pNewDesk->m_bMaxPeople)
{
maxPeople=uSitCount;
maxPeopleDesk=i;
}
}

return *(m_pDesk+maxPeopleDesk);
}*/

//Ǯ���˴���
bool CGameMainManage::OnAboutMoney(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	ASSERT(pNetHead->bMainID==MDM_GR_MONEY);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_PAY_MONEY:
		{
			//��Ǯ����ֱ�������ݿ���ۣ������ϵ�Ǯ

			//Ч������
			if(uSize!=sizeof(MSG_GR_S_Pay)) return false;
			MSG_GR_S_Pay* pPayMoney=(MSG_GR_S_Pay*)pData;
			//��������
			CGameUserInfo* pUserInfo = m_UserManage.FindOnLineUser(pPayMoney->UserID);
			if(pUserInfo==NULL)
				return false;

			if(pPayMoney->uCount + pUserInfo->m_UserData.i64Money < 0)//���ϵ�Ǯ����
			{
				m_TCPSocket.SendData(uIndex,MDM_GR_MONEY,ASS_GR_PAY_MONEY,0,dwHandleID);//
				return true;
			}

			pUserInfo->m_ChangePoint.dwMoney +=  pPayMoney->uCount;
			pUserInfo->m_UserData.i64Money +=  pPayMoney->uCount;
			//�ཿ�Ǯ
			DL_GR_I_UserLeft m_UserInfo;
			memset(&m_UserInfo,0,sizeof(m_UserInfo));
			m_UserInfo.dwUserID = pPayMoney->UserID;
			m_UserInfo.dwChangeMoney = pPayMoney->uCount;
			m_SQLDataManage.PushLine(&m_UserInfo.DataBaseHead,sizeof(m_UserInfo),DTK_GR_UPDATE_GAME_RESULT,0,0);

			m_TCPSocket.SendDataBatch(pPayMoney,uSize,MDM_GR_MONEY,ASS_GR_PAY_MONEY,1);//�ɹ�
			return true;
		}
	case ASS_GR_REFLASH_MONEY:
		{
			//Ч������
			if(uSize!=sizeof(MSG_GR_S_RefalshMoney)) return false;
			MSG_GR_S_RefalshMoney* pReflashMoney=(MSG_GR_S_RefalshMoney*)pData;
			if(pNetHead->bHandleCode==0)//ͬ�����
			{
				if(m_InitData.uComType!=TY_MONEY_GAME) return false;

				//��������
				CGameUserInfo * pUserInfo=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserInfo==NULL) return false;
				else
				{
					pUserInfo->m_UserData.i64Money = pReflashMoney->i64Money;
					if(pUserInfo->m_UserData.i64Money<0) 
					{
						pUserInfo->m_UserData.i64Money=0; 
						pReflashMoney->i64Money=0;
					}

				}
				m_TCPSocket.SendDataBatch(pReflashMoney,uSize,MDM_GR_ROOM,ASS_GR_USER_POINT,10);
				return true;
			}else if(pNetHead->bHandleCode==1)//ͬ������
			{
				//��������
				CGameUserInfo * pUserInfo=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserInfo==NULL) return false;
				else
				{
					pUserInfo->m_UserData.dwPoint = pReflashMoney->i64Money;
				}
				m_TCPSocket.SendDataBatch(pReflashMoney,uSize,MDM_GR_ROOM,ASS_GR_USER_POINT,11);
				return true;
			}
		}
	case ASS_GR_OPEN_WALLET_INGAME:		///< ��Ϸ�д����н��� 
		{
			//Ч������
			//if(uSize!=sizeof(MSG_GR_OpenBank))
			//	return false;
			MSG_GR_OpenBank * pOpenBank=(MSG_GR_OpenBank *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;

			//��ȡ���ݿ�
			DL_GR_I_UnRegisterUser OpenWallet;
			memset(&OpenWallet,0,sizeof(OpenWallet));
			OpenWallet.dwUserID = pUserInfo->m_UserData.dwUserID;
			//lstrcpy(OpenWallet.szMD5Pass,pOpenBank->szMD5Pass);
			m_SQLDataManage.PushLine(&OpenWallet.DataBaseHead,sizeof(OpenWallet),DTK_GR_DEMAND_MONEY_IN_GAME,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_OPEN_WALLET:			///< �ڴ�����Ǯ��
		{
			MSG_GR_OpenBank * pOpenBank=(MSG_GR_OpenBank *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;

			//��ȡ���ݿ�
			DL_GR_I_UnRegisterUser OpenWallet;
			memset(&OpenWallet,0,sizeof(OpenWallet));
			OpenWallet.dwUserID = pUserInfo->m_UserData.dwUserID;
			//lstrcpy(OpenWallet.szMD5Pass,pOpenBank->szMD5Pass);
			m_SQLDataManage.PushLine(&OpenWallet.DataBaseHead,sizeof(OpenWallet),DTK_GR_DEMAND_MONEY,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_CHECK_OUT:			//ȡ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_CheckMoneyWithPwd_t)) return false;
			MSG_GR_S_CheckMoneyWithPwd_t* pCheckOut=(MSG_GR_S_CheckMoneyWithPwd_t *)pData;

			//��������
			CGameUserInfo* pTargetUserInfo=m_UserManage.FindOnLineUser(pCheckOut->UserID);

			//��ȫ��֤�����ڻ����˹���һ��uIndex�����Ի����˲����밲ȫ��֤
			//Add by JianGuankun 2012.2.1
			if (pTargetUserInfo == NULL)
			{
				return false;
			}

			if (!pTargetUserInfo->m_UserData.isVirtual)
			{
				CGameUserInfo* pUserInfo = m_UserManage.GetOnLineUserInfo(uIndex);
				if (pUserInfo == NULL)
				{
					return false;
				}

				if (pCheckOut->UserID != pUserInfo->m_UserData.dwUserID)
				{
					return false;
				}
			}

			//end add

			//if (pTargetUserInfo==NULL||pCheckOut->UserID!=pUserInfo->m_UserData.dwUserID) return false;

			/// ������������Ϸ�У��ǰ�������Ϸ��ׯ��(����)������ȡǮ add by lxl 2010-12-6
			if (pTargetUserInfo->m_UserData.bUserState == USER_PLAY_GAME 
				|| pTargetUserInfo->m_UserData.bUserState == USER_CUT_GAME
				|| ((m_InitData.dwRoomRule & GRR_GAME_U3D) && IsUserPlayGame(pTargetUserInfo))
				)
			{
				if(pTargetUserInfo->m_UserData.bDeskNO != 255)
				{
					CGameDesk *pDesk = *(m_pDesk+pTargetUserInfo->m_UserData.bDeskNO);

					if(!pDesk->IsBJLGameType())
					{		
						m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_OUT, ASS_GR_CHECKOUT_ERROR_PLAYING, dwHandleID);
						return true;
					}

					CBcfFile f(CBcfFile::GetAppPath() + "Function.bcf");
					int drawMoneyInGame = f.GetKeyVal("DrawMoneyInGame","Available",0);
					if(drawMoneyInGame)
					{
						if(pDesk->IsZhuangjia(pTargetUserInfo->m_UserData.dwUserID))
						{
							m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_OUT, ASS_GR_CHECKOUT_ERROR_PLAYING, dwHandleID);
							return true;
						}
					}

				}
			}


			//д�����ݿ�
			DL_GR_I_CheckOutMoneyWithPwd_t CheckOut;
			memset(&CheckOut,0,sizeof(CheckOut));
			CheckOut.dwUserID=pCheckOut->UserID;
			CheckOut.i64Count=pCheckOut->i64Count;
			CopyMemory(CheckOut.szMD5Pass, pCheckOut->szMD5Pass, sizeof(CheckOut.szMD5Pass));
			m_SQLDataManage.PushLine(&CheckOut.DataBaseHead,sizeof(CheckOut),DTK_GR_CHECKOUT_MONEY,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_CHECK_OUT_INGAME:	// ��Ϸ��ȡ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_CheckMoneyWithPwd_t)) return false;
			MSG_GR_S_CheckMoneyWithPwd_t * pCheckOut=(MSG_GR_S_CheckMoneyWithPwd_t *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;
			CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pCheckOut->UserID);
			if (pTargetUserInfo==NULL||pCheckOut->UserID!=pUserInfo->m_UserData.dwUserID) return false;

			/// ������������Ϸ�У����Ƿǰ�������Ϸ������ȡǮ add by lxl 2010-12-6
			if (pUserInfo->m_UserData.bUserState == USER_PLAY_GAME 
				|| pUserInfo->m_UserData.bUserState == USER_CUT_GAME
				|| ((m_InitData.dwRoomRule & GRR_GAME_U3D) && IsUserPlayGame(pTargetUserInfo))
				)
			{
				if(pUserInfo->m_UserData.bDeskNO != 255)
				{
					CGameDesk *pDesk = *(m_pDesk+pUserInfo->m_UserData.bDeskNO);
					if(!pDesk->IsBJLGameType())
					{		
						m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_OUT_INGAME, ASS_GR_CHECKOUT_ERROR_PLAYING, dwHandleID);
						return true;
					}

					CBcfFile f(CBcfFile::GetAppPath() + "Function.bcf");
					int drawMoneyInGame = f.GetKeyVal("DrawMoneyInGame","Available",0);
					if(drawMoneyInGame)
					{
						if(pDesk->IsZhuangjia(pUserInfo->m_UserData.dwUserID))
						{
							m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_OUT_INGAME, ASS_GR_CHECKOUT_ERROR_PLAYING, dwHandleID);
							return true;
						}
					}
				}
			}


			//д�����ݿ�
			DL_GR_I_CheckOutMoneyWithPwd_t CheckOut;
			memset(&CheckOut,0,sizeof(CheckOut));
			CheckOut.dwUserID=pCheckOut->UserID;
			CheckOut.i64Count=pCheckOut->i64Count;
			CopyMemory(CheckOut.szMD5Pass, pCheckOut->szMD5Pass, sizeof(CheckOut.szMD5Pass));
			m_SQLDataManage.PushLine(&CheckOut.DataBaseHead,sizeof(CheckOut),DTK_GR_CHECKOUT_MONEY_INGAME,uIndex,dwHandleID);
			return true;

		}
	case ASS_GR_CHECK_IN:			//��Ǯ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_CheckMoney)) return false;
			MSG_GR_S_CheckMoney * pCheckIn=(MSG_GR_S_CheckMoney *)pData;

			//��������
			CGameUserInfo* pTargetUserInfo=m_UserManage.FindOnLineUser(pCheckIn->UserID);

			//��ȫ��֤�����ڻ����˹���һ��uIndex�����Ի����˲����밲ȫ��֤
			//Add by JianGuankun 2012.2.1
			if (pTargetUserInfo == NULL)
			{
				return false;
			}
			
			if (!pTargetUserInfo->m_UserData.isVirtual)
			{
				CGameUserInfo* pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
				if (pUserInfo == NULL)
				{
					return false;
				}
				if (pCheckIn->UserID != pUserInfo->m_UserData.dwUserID)
				{
					return false;
				}
			}
			//end add

			/// ������������Ϸ�У����ô�Ǯ
			/// todo

			if (pTargetUserInfo->m_UserData.bUserState == USER_PLAY_GAME
				|| pTargetUserInfo->m_UserData.bUserState == USER_CUT_GAME
				|| ((m_InitData.dwRoomRule & GRR_GAME_U3D) && IsUserPlayGame(pTargetUserInfo))
				)
			{
				m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_IN, ASS_GR_CHECKIN_ERROR_PLAYING, dwHandleID);
				return true;
			}

			/// �ټ�������Ϸ����������ׯ�ң����ô�Ǯ add by lxl 2010-11-5
			//�ж��Ƿ�����Ϸ���ӣ�ֻ������Ϸ�в��п�����ׯ��
			if(pTargetUserInfo->m_UserData.bDeskNO != 255)
			{
				CGameDesk* pDesk = *(m_pDesk + pTargetUserInfo->m_UserData.bDeskNO);
				if(pDesk->IsZhuangjia(pTargetUserInfo->m_UserData.dwUserID))
				{
					m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_IN_INGAME, ASS_GR_CHECKIN_ZHUANGJIA, dwHandleID);
					return true;
				}
			}

			//д�����ݿ�
			DL_GR_I_CheckOutMoney CheckIn;
			memset(&CheckIn,0,sizeof(CheckIn));
			CheckIn.dwUserID=pCheckIn->UserID;
			CheckIn.i64Count= pCheckIn->i64Count;
			m_SQLDataManage.PushLine(&CheckIn.DataBaseHead,sizeof(CheckIn),DTK_GR_CHECKIN_MONEY,uIndex,dwHandleID);
			return true;

		}
	case ASS_GR_CHECK_IN_INGAME:	//��Ϸ�д�Ǯ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_CheckMoney)) return false;
			MSG_GR_S_CheckMoney * pCheckIn=(MSG_GR_S_CheckMoney *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;
			CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pCheckIn->UserID);
			if (pTargetUserInfo==NULL||pCheckIn->UserID!=pUserInfo->m_UserData.dwUserID) return false;
			/// ������������Ϸ�У����ô�Ǯ
			/// todo
			if (pUserInfo->m_UserData.bUserState == USER_PLAY_GAME
				|| pUserInfo->m_UserData.bUserState == USER_CUT_GAME
				|| ((m_InitData.dwRoomRule & GRR_GAME_U3D) && IsUserPlayGame(pTargetUserInfo))
				)
			{
				m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_IN_INGAME, ASS_GR_CHECKIN_ERROR_PLAYING, dwHandleID);
				return true;
			}

			/// �ټ�������Ϸ����������ׯ�ң����ô�Ǯ add by lxl 2010-11-5
			//�ж��Ƿ�����Ϸ���ӣ�ֻ������Ϸ�в��п�����ׯ��
			if(pUserInfo->m_UserData.bDeskNO != 255)
			{
				CGameDesk *pDesk = *(m_pDesk+pUserInfo->m_UserData.bDeskNO);
				if(pDesk->IsZhuangjia(pUserInfo->m_UserData.dwUserID))
				{
					m_TCPSocket.SendData(uIndex, MDM_GR_MONEY, ASS_GR_CHECK_IN_INGAME, ASS_GR_CHECKIN_ZHUANGJIA, dwHandleID);
					return true;
				}
			}

			//д�����ݿ�
			DL_GR_I_CheckOutMoney CheckIn;
			memset(&CheckIn,0,sizeof(CheckIn));
			CheckIn.dwUserID=pCheckIn->UserID;
			CheckIn.i64Count=pCheckIn->i64Count;
			m_SQLDataManage.PushLine(&CheckIn.DataBaseHead,sizeof(CheckIn),DTK_GR_CHECKIN_MONEY_INGAME,uIndex,dwHandleID);
			return true;

		}
	case ASS_GR_TRANSFER_MONEY:			//ת��
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_TransferMoney)) return false;
			MSG_GR_S_TransferMoney * pTransferMoney=(MSG_GR_S_TransferMoney *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
				return false;
			if(pTransferMoney->UserID != pUserInfo->m_UserData.dwUserID)
				return false;

			/// ����Լ�ת���Լ�
			if (pTransferMoney->UserID == pTransferMoney->destUserID)
			{
				return false;
			}

			//д�����ݿ�
			DL_GR_I_TransferMoney TransferMoney;
			memset(&TransferMoney,0,sizeof(TransferMoney));
			::memcpy(&TransferMoney.stTransfer,pTransferMoney,sizeof(MSG_GR_S_TransferMoney));
			//Ӧ����˰
			CString stime;
			stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
			int curtime=atoi(stime);                           //ת��Ϊint

			//if(pUserInfo->m_UserData.iVipTime>curtime)//vip����˰
			//	TransferMoney.stTransfer.uTax = 0;
			//else
			//	TransferMoney.stTransfer.uTax = TransferMoney.stTransfer.uMoney/10;

			TransferMoney.stTransfer.bTranTax = 1; ///< Ĭ������¶�ÿ��ת�˿�˰

			CBcfFile fConfig(CBcfFile::GetAppPath()+"Function.bcf");
			CString strBuffer = fConfig.GetKeyVal("SpecificID","Available","0");
			if (atoi(strBuffer)) ///< ָ���˺�ID
			{
				strBuffer = fConfig.GetKeyVal("SpecificID","NormalID","60000000,69999999");
				int reValue = TransferIDCompare(TransferMoney.stTransfer.UserID,TransferMoney.stTransfer.destUserID,
					atol(strBuffer.Left(strBuffer.Find(",")+1)),
					atol(strBuffer.Right(strBuffer.GetLength()-strBuffer.Find(",")-1)));

				switch (reValue)
				{
				case 0:
					return false; ///< ����ת��

				case 1:
					///< ����ת�ˣ���˰
					TransferMoney.stTransfer.bTranTax = 1;
					break;

				case 2:
					///< ����ת�ˣ�����˰
					TransferMoney.stTransfer.bTranTax = 2;
					break;

				case 3:
					///< ɢ����ת��
					TransferMoney.stTransfer.bTranTax = 3;
					break;

				default :
					break;
				}
			}

            // PengJiLin, 2010-8-23, ��ͨת�˻���ȫת���ܣ�1=ȫת
            TransferMoney.stTransfer.uHandleCode = pNetHead->bHandleCode;

			m_SQLDataManage.PushLine(&TransferMoney.DataBaseHead,sizeof(TransferMoney),DTK_GR_TRANSFER_MONEY,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_TRANSFER_MONEY_INGAME:	//��Ϸ��ת��
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_TransferMoney)) return false;
			MSG_GR_S_TransferMoney * pTransferMoney=(MSG_GR_S_TransferMoney *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
				return false;
			if(pTransferMoney->UserID != pUserInfo->m_UserData.dwUserID)
				return false;

			/// ����Լ�ת���Լ�
			if (pTransferMoney->UserID == pTransferMoney->destUserID)
			{
				return false;
			}

			//д�����ݿ�
			DL_GR_I_TransferMoney TransferMoney;
			memset(&TransferMoney,0,sizeof(TransferMoney));
			::memcpy(&TransferMoney.stTransfer,pTransferMoney,sizeof(MSG_GR_S_TransferMoney));
			//Ӧ����˰
			CString stime;
			stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
			int curtime=atoi(stime);                           //ת��Ϊint

			//if(pUserInfo->m_UserData.iVipTime>curtime)//vip����˰
			//	TransferMoney.stTransfer.uTax = 0;
			//else
			//	TransferMoney.stTransfer.uTax = TransferMoney.stTransfer.uMoney/10;

			TransferMoney.stTransfer.bTranTax = 1; ///< Ĭ������¶�ÿ��ת�˿�˰

			CBcfFile fConfig(CBcfFile::GetAppPath()+"Function.bcf");
			CString strBuffer = fConfig.GetKeyVal("SpecificID","Available","0");
			if (atoi(strBuffer)) ///< ָ���˺�ID
			{
				strBuffer = fConfig.GetKeyVal("SpecificID","NormalID","60000000,69999999");
				int reValue = TransferIDCompare(TransferMoney.stTransfer.UserID,TransferMoney.stTransfer.destUserID,
					atol(strBuffer.Left(strBuffer.Find(",")+1)),
					atol(strBuffer.Right(strBuffer.GetLength()-strBuffer.Find(",")-1)));

				switch (reValue)
				{
				case 0:
					return false; ///< ����ת��

				case 1:
					///< ����ת�ˣ���˰
					TransferMoney.stTransfer.bTranTax = 1;
					break;

				case 2:
					///< ����ת�ˣ�����˰
					TransferMoney.stTransfer.bTranTax = 2;
					break;

				case 3:
					///< ɢ����ת��
					TransferMoney.stTransfer.bTranTax = 3;
					break;

				default :
					break;
				}
			}

			m_SQLDataManage.PushLine(&TransferMoney.DataBaseHead,sizeof(TransferMoney),DTK_GR_TRANSFER_MONEY_INGAME,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_CHANGE_PASSWD:
		{
			if (uSize != sizeof(MSG_GR_S_ChangePasswd_t))
			{
				 return false;
			}
			MSG_GR_S_ChangePasswd_t *pChangePasswd = (MSG_GR_S_ChangePasswd_t *)pData;

			/// �������ݣ��ҳ��û��������͵��ô洢���̵�ָ�������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
				return false;
			if(pChangePasswd->UserID != pUserInfo->m_UserData.dwUserID)
				return false;
			/// 
			DL_GR_I_ChangePasswd_t changePasswd;
			changePasswd.stChangePasswd.UserID = pUserInfo->m_UserData.dwUserID;
			_tcscpy(changePasswd.stChangePasswd.szMD5PassNew, pChangePasswd->szMD5PassNew);
			_tcscpy(changePasswd.stChangePasswd.szMD5PassOld, pChangePasswd->szMD5PassOld);
			m_SQLDataManage.PushLine(&changePasswd.DataBaseHead,sizeof(changePasswd),DTK_GR_CHANGE_PASSWD,uIndex,dwHandleID);

			return true;
		}
	case ASS_GR_TRANSFER_RECORD:
		{
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
			{
				return false;
			}
			MSG_GR_S_TransferRecord_t *pTransferRecord = (MSG_GR_S_TransferRecord_t *)pData;
			/// ����û��������Լ����û�������ʧ��
			if (pUserInfo->m_UserData.dwUserID != pTransferRecord->dwUserID)
			{
				return false;
			}
			DL_GR_I_TransferRecord_t transferRecord;
			transferRecord.stTransfer.dwUserID = pUserInfo->m_UserData.dwUserID;
			m_SQLDataManage.PushLine(&transferRecord.DataBaseHead,sizeof(transferRecord),DTK_GR_TRANSFER_RECORD,uIndex,dwHandleID);
			/// "SP_GetTransferRecord"
			break;
		}
	case ASS_GR_TRANSFER_RECORD_INGAME:
		{
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
			{
				return false;
			}
			MSG_GR_S_TransferRecord_t *pTransferRecord = (MSG_GR_S_TransferRecord_t *)pData;
			/// ����û��������Լ����û�������ʧ��
			if (pUserInfo->m_UserData.dwUserID != pTransferRecord->dwUserID)
			{
				return false;
			}
			DL_GR_I_TransferRecord_t transferRecord;
			transferRecord.stTransfer.dwUserID = pUserInfo->m_UserData.dwUserID;
			m_SQLDataManage.PushLine(&transferRecord.DataBaseHead,sizeof(transferRecord),DTK_GR_TRANSFER_RECORD_INGAME,uIndex,dwHandleID);
			/// "SP_GetTransferRecord"
			break;
		}
	}
	return true;
}

/// ת���û�ID�Ƚ�
/// @param UserID ����Լ���ID��UserIDDestĿ��ID,ɢ��ID NomalIDFrom��ʼ��ɢ��ID NomalIDEnd����
///@return  0����ת�ˣ�1 ����ת��,��˰��2 ����ת��,����˰.3 ����ת�˶�Ϊ��ָ�������ת�˽��
int CGameMainManage::TransferIDCompare(long UserID,long UserIDDest,long NomalIDFrom,long NomalIDEnd)
{
	char BufID[12]     = {0};
	char BufIDDest[12] = {0};

	BOOL specificID     = TRUE;  ///< �Ƿ�ָ��ID��21111100 21111000���Լ���ID��
	BOOL specificIDDest = TRUE;  ///< �Ƿ�ָ��ID��21111100 21111000���Է���ID��
	BOOL sameNumID      = TRUE;  ///< �Ƿ�����ID��11111 444444���Լ���ID��
	BOOL sameNumIDDest  = TRUE;  ///< �Ƿ�����ID��11111 444444���Է���ID��
	BOOL sameID         = TRUE;  ///< ����ID�Ƿ���ͬ
	int CntZero         = 0;   ///< 0�ĸ���
	int CntZeroDest     = 0;   ///< 0�ĸ���
	int Len             = 0;   ///< ID����
	int LenDest         = 0;   ///< ID����
	int CntSame         = 0;   ///< �Լ�ID���Է�ID��ͬ�ĸ���

	if ((UserID>NomalIDFrom && UserID<NomalIDEnd) && (UserIDDest>NomalIDFrom && UserIDDest<NomalIDEnd))///< ɢ��
	{
		return 3;
	}

	if (UserID<NomalIDFrom && UserIDDest<NomalIDFrom)
	{
		sprintf(BufID,"%ld",UserID);
		sprintf(BufIDDest,"%ld",UserIDDest);

		for (int i=0; 0!=BufID[i] || 0!=BufIDDest[i]; i++)
		{
			if (0!=BufID[i])
			{
				if (specificID)
				{
					if ('0' == BufID[i])
					{
						CntZero++;
					}
					else if ('0' != BufID[i] && 0 != CntZero)
					{
						specificID = FALSE; ///< ����ָ��ID(���ǿͻ�ָ��ID)
					}

					if (i > 0 && BufID[0] != BufID[i] && sameNumID) ///< �Ƿ�����ID
					{
						sameNumID = FALSE;
					}
				}
				Len++;

				if (BufID[i] == BufIDDest[i] && sameID)
				{
					CntSame++;
				}
				else
				{
					sameID = FALSE;
				}
			}

			if (0!=BufIDDest[i])
			{
				if (specificIDDest)
				{
					if ('0' == BufIDDest[i])
					{
						CntZeroDest++;
					}
					else if ('0'!=BufIDDest[i] && 0!=CntZeroDest)
					{
						specificIDDest = FALSE; ///< ����ָ��ID(���ǿͻ�ָ��ID)
					}

					if (i > 0 && BufIDDest[0] != BufIDDest[i] && sameNumIDDest) ///< �Ƿ�����ID
					{
						sameNumIDDest = FALSE;
					}
				}
				LenDest++;
			}
		}

		char BufFromID[12] = {0};
		int  LenFromID     = 0;

		sprintf(BufFromID,"%ld",NomalIDFrom);
		LenFromID = strlen(BufFromID);
		if (LenFromID != LenDest) ///< ���Ȳ�������ָ��ID
		{
			specificIDDest = FALSE;
		}

		if (LenFromID != Len) ///< ���Ȳ�������ָ��ID
		{
			specificID = FALSE;
		}

		if (sameNumID && sameNumIDDest) ///< ����ID��������
		{
		//	if (Len>=LenDest && LenDest>4)
			if (Len>4 && LenDest>4) ///< ��������ID������໥ת��--�߻� ��
			{
				return 2;
			}
		}
		else if (sameNumID && Len>4 && !sameNumIDDest && specificIDDest) ///< �Լ�ID�����ţ��Է�ID��ָ���˺�
		{
			if (CntZeroDest < Len-1)
			{
				if (2 == Len-CntZeroDest)
				{
					if ('4' == BufIDDest[LenFromID-CntZeroDest-1])
					{
						return 0;
					}
				}

				if (CntZeroDest>0 || '4'==BufIDDest[LenFromID-1])
					return 2;
				else 
					return 1; ///< ����ת�˿�˰
			}
			else
			{
				return 0;
			}
		}
		else if (sameNumIDDest && LenDest>4 && !sameNumID && specificID) ///< �Է�������ID���Լ���ָ���˺�
		{
			if (CntZero < LenDest-1)
			{
				if (2 == LenDest-CntZero)
				{
					if ('4' == BufID[LenFromID-CntZero-1]) ///< ���Ǹ�ְ��������IDͬ�ȼ�
					{
						return 0;
					}
				}

				if (CntZero>0 || '4'==BufID[LenFromID-1])
					return 2;
				else
					return 2;
			}
			else ///< �Է�����С�����ļ��𣬲���ת�ˡ�
			{
				return 0;
			}
		}

		if (specificID && specificIDDest)
		{
			for (int i=Len-1; i>=CntSame; i--)
			{
				if ((BufID[i] != BufIDDest[i]) && CntSame == i)
				{
					if ('0' == BufID[i])///< ��ְ
					{
						if ('4' == BufIDDest[i]) ///< ��ְ��ְת��
						{
							return 2;
						}
						else ///< ���¼�
						{
							if (CntZeroDest > 0) ///< �ڼ�����ת��
								return 2;
							else
								return 1;
						}
					}
					else if ('4' == BufID[i]) ///< ��ְ
					{
						if ('0' == BufIDDest[i]) ///< ��ְ����ְת�ˣ�����˰
						{
							return 2;
						}
						else ///< ��ְ���¼�
						{
							if (CntZeroDest > 0) ///< �ڼ�����ת��
								return 2;
							else
								return 1;
						}
					}
					else
					{
						if ('0' == BufIDDest[i] || '4' == BufIDDest[i]) ///< ���ϼ�ת��
						{
							if (CntZero > 0) ///< �ڼ�����ת��
								return 2;
							else
								return 2; ///< �����ת�˲���˰ 2010 5.7 �޸�����;
						}
						else ///< ƽ��
						{
							if (Len-1 - CntSame) ///< ������ƽ��ת��
							{
								return 2;
							}
							else
							{
								return 0;
							}
						}
					}
				}
				else if (BufID[i] != BufIDDest[i] && CntSame != i)///< ����ͬһ�ϼ�
				{
					return 0;
				}
				else if (BufID[i]==BufIDDest[i] && '0'!=BufID[i])///< ����ͬһ�ϼ�
				{
					return 0;
				}
			}
		}
		else if (!specificID && !specificIDDest && specificID && specificIDDest)
		{
			return 3; ///< ���������ر�ID��ת�ˡ�
		}
		else ///< ����ת�ˡ�
		{
			return 0;
		}
	}

	///< ������ɢ��������֮��ת��
	int RetUserID;
	int RetUserIDDest;
	RetUserID = GetIDInformation(UserID,NomalIDFrom,NomalIDEnd);
	RetUserIDDest = GetIDInformation(UserIDDest,NomalIDFrom,NomalIDEnd);

	if (-1==RetUserID && RetUserIDDest<-1) ///< ɢ��������ת��
	{
		return 2; ///< ����˰
	}
	else if (RetUserID<-1 && -1==RetUserIDDest) ///< ���Ÿ�ɢ��ת��
	{
		return 1; ///< ��˰
	}

	return 0;
}

/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
/// @return 0��10��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
int CGameMainManage::GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd)
{
	char BufID[12]     = {0};
	int CntZero        = 0;   ///< 0�ĸ���
	int len            = 0;
	BOOL sameNumID     = TRUE;  ///< �Ƿ�����ID��11111 444444���Լ���ID��

	if (UserID>NomalIDFrom && UserID<NomalIDEnd) ///< ɢ��
	{
		return -1;
	}

	if (UserID < NomalIDFrom)
	{
		sprintf(BufID,"%ld",UserID);

		for (int i=0; 0!=BufID[i]; i++)
		{
			if ('0'==BufID[i])
			{
				CntZero++;
			}
			else if ('0'!=BufID[i] && 0!=CntZero) ///< ��Ч�˺�
			{
				return -2;
			}
			
			if (i>0 && BufID[0]!=BufID[i] && sameNumID)
			{
				sameNumID = FALSE;
			}
			len++;
		}

		if (sameNumID) ///< ������ID
		{
			if (len >= 5)
				return -len;
			else
				return -2; ///< ��Ч�˺�
		}

		char BufIDFrom[12] = {0};
		sprintf(BufIDFrom,"%ld",NomalIDFrom);
		if (strlen(BufIDFrom) == len)
		{
			if ('4' == BufID[len-CntZero-1]) ///< ��ְ
			{
				return CntZero+1; ///< 4�ڵ�����CntZero+1λ
			}
			return CntZero;
		}
		else
			return -2; ///< ��Ч�˺�
	}

	return -2; ///< ��Ч�˺�
}

/// ��ȡĳ����Ϸ����ָ��
const CGameDesk* CGameMainManage::GetGameDesk(BYTE bDeskNo)
{
	if (bDeskNo >= m_uDeskCount || m_uDeskCount < 0)
	{
		return NULL;
	}

	return *(m_pDesk + bDeskNo);
}

/// ��ӡ����������ҵ���Ϣ
/// Added By JianGuankun 2011.12.5
void CGameMainManage::PrintUsersLog()
{
	CString strPath = CBcfFile::GetAppPath();
	//f.open("");

	TCHAR szPathName[MAX_PATH];
	TCHAR szPath[MAX_PATH];
	TCHAR szName[MAX_PATH];
	SYSTEMTIME t;

	memset(szPathName,NULL,MAX_PATH);
	::GetCurrentDirectory(MAX_PATH,szPath);
	::strcat(szPath,"\\wlogs");
	::CreateDirectory(szPath,NULL);

	::GetLocalTime(&t);

	//�γ���־ID
	wsprintf(szName,TEXT("%03d_%02d%02d_%02d%02d%02d"),m_InitData.uRoomID,
		t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond);


	wsprintf(szPathName,TEXT("%s\\%s.log"),szPath,szName);

	CFile f;
	if (!f.Open(szPathName,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate))
	{
		return;
	}

	//�����־ͷ
	CString strOutput;
	strOutput.Format("��־: ����ID = %d ʱ�� = %02d.%02d %02d:%02d:%02d"
		,m_InitData.uRoomID,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond);
	Writeline(&f,strOutput);

	Writeline(&f,"===============================================================================================================");

	Writeline(&f,"");

	strOutput.Format("������������%d",m_UserManage.m_uOnLineCount);
	Writeline(&f,strOutput);

	Writeline(&f,"");

	CAFCPtrArray<CGameUserInfo>* pUserList = &m_UserManage.m_OnLineUserInfo;

	strOutput.Format("--��������б�--------------------------------------------------------------------------------------------------------",
		pUserList->GetPermitCount());
	Writeline(&f,strOutput);

	Writeline(&f,"");

	int uFindCount = 0;

	for (int i = 0; i < pUserList->GetPermitCount(); ++i)
	{
		CGameUserInfo* pUserInfo = pUserList->GetArrayItem(i);
		if (pUserInfo)
		{
			strOutput.Format("  ID:%-9d Name:%-20s NickName:%-30s Access = %1d Network = (%05d,%05d) Station = (%03d,%03d,%03d)",
				pUserInfo->m_UserData.dwUserID,pUserInfo->m_UserData.szName,pUserInfo->m_UserData.nickName,pUserInfo->m_bAccess,
				pUserInfo->m_uSocketIndex,pUserInfo->m_dwHandleID,
				pUserInfo->m_UserData.bDeskNO,pUserInfo->m_UserData.bDeskStation,pUserInfo->m_UserData.bUserState);
			Writeline(&f,strOutput);
			if (pUserInfo->IsAccess())
			{
				uFindCount++;
			}
		}
	}

	Writeline(&f,"");
	strOutput.Format("--(��%d��)--",uFindCount);
	Writeline(&f,strOutput);
	Writeline(&f,"");

	pUserList = &m_UserManage.m_NetCutUserInfo;

	strOutput.Format("--��������б�--------------------------------------------------------------------------------------------------------",
		pUserList->GetPermitCount());
	Writeline(&f,strOutput);

	Writeline(&f,"");

	uFindCount = 0;

	for (int i = 0; i < pUserList->GetPermitCount(); ++i)
	{
		CGameUserInfo* pUserInfo = pUserList->GetArrayItem(i);
		if (pUserInfo)
		{
			strOutput.Format("  ID:%-9d Name:%-20s NickName:%-30s Access = %1d Network = (%05d,%05d) Station = (%03d,%03d,%03d)",
				pUserInfo->m_UserData.dwUserID,pUserInfo->m_UserData.szName,pUserInfo->m_UserData.nickName,pUserInfo->m_bAccess,
				pUserInfo->m_uSocketIndex,pUserInfo->m_dwHandleID,
				pUserInfo->m_UserData.bDeskNO,pUserInfo->m_UserData.bDeskStation,pUserInfo->m_UserData.bUserState);
			Writeline(&f,strOutput);
			if (pUserInfo->IsAccess())
			{
				uFindCount++;
			}
		}
	}

	Writeline(&f,"");
	strOutput.Format("--(��%d��)--",uFindCount);
	Writeline(&f,strOutput);

	Writeline(&f,"");
	Writeline(&f,"ע�ͣ�ID,Name,NickName,Access,Network = (SocketIndex,HandleCode),Station = (DeskNO,DeskStation,UserState)");
	Writeline(&f,"");

	f.Close();

	return;
}

//д��־
bool CGameMainManage::Writeline(CFile* p,CString str)
{
	if (p == NULL)
	{
		return false;
	}

	p->Write(str.GetBuffer(),str.GetLength());
	p->Write(_TEXT("\r\n"),2);

	return true;
}


//������Ϣ����
bool CGameMainManage::OnManageMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	ASSERT(pNetHead->bMainID==MDM_GR_MANAGE);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_ALONE_DESK://����
		{	
			if(uSize!=sizeof(MSG_GR_SR_MatchDelete)) return false;
			MSG_GR_SR_MatchDelete *aloneDesk=(MSG_GR_SR_MatchDelete*) pData;
			CGameDesk * pDesk=*(m_pDesk+aloneDesk->bDeskIndex);
			//if(aloneDesk->bDeskStation==1) pDesk->LockDesk();  
			if(aloneDesk->bDeskStation==1) pDesk->VirtualLockDesk();//yjj 090325

			//wushuqun 2009.6.6
			//��ʱ��������
			if (pDesk)
			{
				DL_GR_I_ALone_Data aloneDeskData;
				memset(& aloneDeskData,0,sizeof(aloneDeskData));
				//����������
				aloneDeskData.bDeskIndex = pDesk->m_bDeskIndex;
				//������ʶ
				aloneDeskData.bDeskStation = aloneDesk->bDeskStation;
				//��������������
				aloneDeskData.bDeskPeople = pDesk->m_bMaxPeople;
				//����ID��
				aloneDeskData.uRoomID = pDesk->m_pDataManage->m_InitData.uRoomID;
				//д���ݿ��¼
				m_SQLDataManage.PushLine(&aloneDeskData.DataBaseHead,sizeof(aloneDeskData),DTR_GR_ALONEDESK_INTIME,uIndex,dwHandleID);
			}
			m_TCPSocket.SendDataBatch(NULL,0,MDM_GR_DESK,aloneDesk->bDeskIndex,aloneDesk->bDeskStation);
			return true;

		}
	case ASS_GR_DEALONE_DESK://
		{
			if(uSize!=sizeof(MSG_GR_SR_MatchDelete)) return false;
			MSG_GR_SR_MatchDelete *aloneDesk=(MSG_GR_SR_MatchDelete*) pData;
			CGameDesk * pDesk=*(m_pDesk+aloneDesk->bDeskIndex);
			//if(aloneDesk->bDeskStation==2) pDesk->UnlockDesk();  
			if(aloneDesk->bDeskStation==2) pDesk->UnVirtualLockDesk();//yjj 090325

			//wushuqun 2009.6.6
			//��ʱ���������
			if (pDesk)
			{
				DL_GR_I_ALone_Data aloneDeskData;
				memset(& aloneDeskData,0,sizeof(aloneDeskData));
				//����������
				aloneDeskData.bDeskIndex = pDesk->m_bDeskIndex;
				//������ʶ
				aloneDeskData.bDeskStation = aloneDesk->bDeskStation;
				//��������������,�����ʱ������Ϊ��
				aloneDeskData.bDeskPeople =  pDesk->m_bMaxPeople;

				aloneDeskData.bDeskPeople = -aloneDeskData.bDeskPeople;
				//����ID��
				aloneDeskData.uRoomID = pDesk->m_pDataManage->m_InitData.uRoomID;
				//д���ݿ��¼
				m_SQLDataManage.PushLine(&aloneDeskData.DataBaseHead,sizeof(aloneDeskData),DTR_GR_ALONEDESK_INTIME,uIndex,dwHandleID);
			}

			m_TCPSocket.SendDataBatch(NULL,0,MDM_GR_DESK,aloneDesk->bDeskIndex,aloneDesk->bDeskStation);
			return true;
		}
	case ASS_GR_WARN_MSG:			//������Ϣ
		{
			//Ч������
			MSG_GR_SR_Warning * pWarningUser=(MSG_GR_SR_Warning *)pData;
			if ((uSize<=(sizeof(MSG_GR_SR_Warning)-sizeof(pWarningUser->szWarnMsg)))||(uSize>sizeof(MSG_GR_SR_Warning))) return false;
			//if ((pWarningUser->uLength+1)!=(uSize+sizeof(pWarningUser->szWarnMsg)-sizeof(MSG_GR_SR_Warning))) return false;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if ((pUserInfo==NULL)||(CUserPower::CanSendWarning(pUserInfo->m_dwMasterPower)==false)) return false;
			CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pWarningUser->dwTargetID);
			if (pTargetUserInfo==NULL) return true;
			pWarningUser->szWarnMsg[pWarningUser->uLength]=0;

			//��������
			m_TCPSocket.SendData(pTargetUserInfo->m_uSocketIndex,pWarningUser,uSize,MDM_GR_MANAGE,ASS_GR_WARN_MSG,0,pTargetUserInfo->m_dwHandleID);

			//д�����ݿ�
			DL_GR_I_Warn_User_Record WarningUserRecord;
			memset(&WarningUserRecord,0,sizeof(WarningUserRecord));
			WarningUserRecord.dwGMID=pUserInfo->m_UserData.dwUserID;
			WarningUserRecord.dwTargetID=pWarningUser->dwTargetID;
			lstrcpy(WarningUserRecord.szWarnMsg,pWarningUser->szWarnMsg);
			m_SQLDataManage.PushLine(&WarningUserRecord.DataBaseHead,sizeof(WarningUserRecord),DTK_GR_GM_WARNING_USER_RECORD,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_CUT_USER:			//���û�����//��ȫ����û���Ϸ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_CutUser)) return false;
			MSG_GR_S_CutUser * pCutUser=(MSG_GR_S_CutUser *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if ((pUserInfo==NULL)||(CUserPower::CanCutUser(pUserInfo->m_dwMasterPower)==false)) return false;
			CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pCutUser->dwTargetID);
			if (pTargetUserInfo==NULL) return true;

			//��������
			if (CUserPower::CanHideAction(pUserInfo->m_dwMasterPower)==true)
				pCutUser->dwUserID=0L;
			else 
				pCutUser->dwUserID=pUserInfo->m_UserData.dwUserID;
			if(pNetHead->bHandleCode==0)//���û�����
				m_TCPSocket.SendData(pTargetUserInfo->m_uSocketIndex,MDM_GR_MANAGE,ASS_GR_CUT_USER,0,pTargetUserInfo->m_dwHandleID);
			else if(pNetHead->bHandleCode==1)//��ȫ����û���Ϸ
				m_TCPSocket.SendData(pTargetUserInfo->m_uSocketIndex,MDM_GR_MANAGE,ASS_GR_CUT_USER,1,pTargetUserInfo->m_dwHandleID);

			//д�����ݿ�
			DL_GR_I_Kick_User_Record DT_KickUserRecord;
			memset(&DT_KickUserRecord,0,sizeof(DT_KickUserRecord));
			lstrcpy(DT_KickUserRecord.szTargetUserName,pTargetUserInfo->m_UserData.szName);
			DT_KickUserRecord.GMID=pUserInfo->m_UserData.dwUserID;
			m_SQLDataManage.PushLine(&DT_KickUserRecord.DataBaseHead,sizeof(DT_KickUserRecord),DTK_GR_GM_KICK_USER_RECORD,uIndex,dwHandleID);
			return true;
		}
	case ASS_GR_POWER_SET://����Ȩ��
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_SR_GamePowerSet)) return false;
			MSG_GR_SR_GamePowerSet * pPowerSet=(MSG_GR_SR_GamePowerSet *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if ((pUserInfo==NULL)||(pUserInfo->m_dwMasterPower==0L)) return false;
			CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pPowerSet->dwUserID);
			if (pTargetUserInfo!=NULL)
			{
				//����Ȩ��
				CUserPower::SetCanRoomTalk(pTargetUserInfo->m_dwGamePower,CUserPower::CanRoomTalk(pPowerSet->dwGamePowerID));
				CUserPower::SetCanGameTalk(pTargetUserInfo->m_dwGamePower,CUserPower::CanGameTalk(pPowerSet->dwGamePowerID));
				CUserPower::SetCanSendMessage(pTargetUserInfo->m_dwGamePower,CUserPower::CanSendMessage(pPowerSet->dwGamePowerID));
				CUserPower::SetCanWatchGame(pTargetUserInfo->m_dwGamePower,CUserPower::CanWatchGame(pPowerSet->dwGamePowerID));
				CUserPower::SetCanPlayGame(pTargetUserInfo->m_dwGamePower,CUserPower::CanPlayGame(pPowerSet->dwGamePowerID));
				CUserPower::SetCanLimiteIP(pTargetUserInfo->m_dwGamePower,CUserPower::CanLimiteIP(pPowerSet->dwGamePowerID));

				//��������
				pPowerSet->dwGamePowerID=pTargetUserInfo->m_dwGamePower;
				pPowerSet->dwMasterPowerID=pTargetUserInfo->m_dwMasterPower;
				m_TCPSocket.SendData(pTargetUserInfo->m_uSocketIndex,pPowerSet,sizeof(MSG_GR_SR_GamePowerSet),MDM_GR_MANAGE,ASS_GR_POWER_SET,
					ASS_GR_GAME_POWER_SET_RETURN,pTargetUserInfo->m_dwHandleID);

				//д�����ݿ�
				DL_GR_I_PowerSet DT_PowerSet;
				memset(&DT_PowerSet,0,sizeof(DT_PowerSet));
				DT_PowerSet.dwGMID=pUserInfo->m_UserData.dwUserID;
				DT_PowerSet.dwGamePower=pTargetUserInfo->m_dwGamePower;
				DT_PowerSet.dwMasterPower=pTargetUserInfo->m_dwMasterPower;
				DT_PowerSet.dwUserID=pPowerSet->dwUserID;
				DT_PowerSet.uLockTime=pPowerSet->uLockTime;
				DT_PowerSet.bBindKind = pPowerSet->bBindKind;
				m_SQLDataManage.PushLine(&DT_PowerSet.DataBaseHead,sizeof(DT_PowerSet),DTK_GR_GAME_POWER,uIndex,dwHandleID);
			}
			return true;
		}
	case ASS_GR_CALL_GM://����GM
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_Call_GM)) return false;
			MSG_GR_Call_GM * pcallGM=(MSG_GR_Call_GM *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;
			DL_GR_I_CallGM dl_callGM;
			dl_callGM.dwUserID=pUserInfo->m_UserData.dwUserID;
			lstrcpy(dl_callGM.szName,pUserInfo->m_UserData.szName);
			dl_callGM.uDeskNO=pUserInfo->m_UserData.bDeskNO;
			dl_callGM.uDeskStation=pUserInfo->m_UserData.bDeskStation;
			dl_callGM.bCallReason=pcallGM->bCallReason;
			dl_callGM.szMsg[pcallGM->uLength]=0;
			lstrcpy(dl_callGM.szMsg,pcallGM->szMsg);
			m_SQLDataManage.PushLine(&dl_callGM.DataBaseHead,sizeof(dl_callGM),DTK_GR_CALL_GM,uIndex,dwHandleID);
			return true;
		}
		//case ASS_GR_OPT_USER://�����Ӻ��ѻ���ӻ�������
		//	{
		//		//Ч������
		//		if (uSize!=sizeof(MSG_GR_S_User)) return false;
		//		MSG_GR_S_User * pOptUser=(MSG_GR_S_User *)pData;

		//		//��������
		//		CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
		//		if (pUserInfo==NULL) 
		//			return false;
		//		CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pOptUser->dwTargetID);
		//		if(pTargetUserInfo == NULL)
		//			return true;
		//		//�������ݶ���
		//		DL_GR_I_User_Opt UserOpt;
		//		UserOpt.dwTargetID = pOptUser->dwTargetID;
		//		UserOpt.uType = pOptUser->uType;

		//		m_SQLDataManage.PushLine(&UserOpt.DataBaseHead,sizeof(UserOpt),DTK_GR_USER_OPT,uIndex,dwHandleID);

		//		return true;
		//	}
	}
	return false;
}
//����д���
bool CGameMainManage::OnPointMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_OPEN_POINT:
		{
			DL_GR_I_OpenPoint op;
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
				return false;			
			op.dwUserID = pUserInfo->m_UserData.dwUserID;
			m_SQLDataManage.PushLine(&op.DataBaseHead,sizeof(op),DTK_GR_OPEN_POINT,uIndex,dwHandleID);
		}
		return true;
	case ASS_GR_GET_POINT:
		{
			DL_GR_I_GetPoint getpoint;
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL)
				return false;	
			if(uSize != sizeof(MSG_GR_S_GetPoint))
				return false;
			MSG_GR_S_GetPoint *pGetPoint = (MSG_GR_S_GetPoint *)pData;
			getpoint.dwUserID = pUserInfo->m_UserData.dwUserID;
			getpoint.dwsrcPoint = pUserInfo->m_UserData.dwPoint;
			getpoint.dwPoint = pGetPoint->dwPoint;
			m_SQLDataManage.PushLine(&getpoint.DataBaseHead,sizeof(getpoint),DTK_GR_GET_POINT,uIndex,dwHandleID);
		}
		return true;
	}
	return false;
}


////С����
//bool CGameMainManage::OnBroadcastMessage(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
//{
//	CGameUserInfo * pUserInfo = m_UserManage.GetOnLineUserInfo(uIndex);
//	if (pUserInfo == NULL) 
//		return false;
//	return true;
//}



//���Ѵ���
bool CGameMainManage::OnFriendManageMsg(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	CGameUserInfo * pSenderUserInfo = m_UserManage.GetOnLineUserInfo(uIndex);
	if (pSenderUserInfo == NULL) 
		return false;
	//if (sizeof(tagIM_Msg) != uSize)
	//{
	//	return false;
	//}

	return true;
}



//�ŶӴ���
bool CGameMainManage::OnQueueMsg(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	ASSERT(pNetHead->bMainID == MDM_GR_QUEUE_MSG);
	switch(pNetHead->bAssistantID)//�Ŷ���Ϣ����
	{
	case ASS_GR_ADD_QUEUE://�����Ŷ�
		MSG_GR_Queue stQueue;
		memset(&stQueue,0,sizeof(stQueue));
		m_TCPSocket.SendData(uIndex,&stQueue,sizeof(stQueue),MDM_GR_QUEUE_MSG,ASS_GR_ADD_QUEUE_RESULT,0,0);
		break;
	case ASS_GR_AGREE_PLAN_SIT://ͬ������(ϵͳ������λ)
		break;
	case ASS_GR_REFUSE_PLAN_SIT://��ͬ����ĳλ����
		break;
	}
	return true;
}

bool CGameMainManage::IsContestGameOver()
{
	if (m_InitData.dwRoomRule & GRR_CONTEST)
	{
		CTime _t = CTime::GetCurrentTime();
		if (_t.GetTime() > m_InitData.i64TimeEnd) return true;
	}
	return false;
}

bool CGameMainManage::IsLeaseTimeOver()
{
	if (m_InitData.iLeaseID != 0)
	{
		CTime _t = CTime::GetCurrentTime();
		if (_t.GetTime() > m_InitData.i64LeaseTimeEnd) return true;
	}
	return false;
}

///�Ŷӻ�����
void CGameMainManage::queuerGame()
{
    static int* pArray = NULL;


	if (IsContestGameOver()) return;

	if (IsLeaseTimeOver()) return;

    // PengJiLin, 2010-5-20
    if(m_pGameUserInfoList.size() < m_uQueuerMaxPeople) return;    
    if(NULL == pArray) pArray = new int [m_uQueuerMaxPeople];

	int iDeskIndex = -1;						//��������
	int iDeskStation = 0;						//��λ����
	int iADeskPlayerNum = 0;					//�����е�����
	bool isGameing = false;						//�����Ƿ�ʼ��Ϸ��
	//bool isADeskGameBegin = false;				//�Ƿ��������Ŷӳɹ�������ʼ��Ϸ��
	BYTE bErrorCode=ERR_GR_ERROR_UNKNOW;
	
	CGameUserInfo* pGameUserInfo = NULL;
	CGameDesk* pGameDesk = NULL;
	
	list<CGameUserInfo*> tempList;				//����ԭ�����Ŷ�list
	list<CGameUserInfo*> sitdownList;			//�����Ѿ����µ����
	list<CGameUserInfo*>::iterator iter1;


	//{���� rongqiufen 2010.06.29
	//�Ƿ��ų��ɹ�
	bool hasSucceed = false;

	int iDeskPlayerNum = (*m_pDesk)->m_bMaxPeople;
	if(m_pGameUserInfoList.size() >= m_uQueuerPeople + iDeskPlayerNum)
	{
		srand(unsigned(time(0)));
		///ÿ֧������������Ȩ0-49
		for (iter1 = m_pGameUserInfoList.begin(); iter1 != m_pGameUserInfoList.end(); iter1++)
		{
			if ((*iter1)->m_UserData.bDeskNO == 255) 
			{
				if ((*iter1)->GetWeigth() < MAXLONG - 50) 
				{
					int r = rand() % 50;
					(*iter1)->m_SelectWeight += r;
				}
			}
		}
		{
			///��Ȩֵ��������Ȩֵ�ߵĻ����Ƚ��д���
			m_pGameUserInfoList.sort(compare());
        //���� rongqiufen}

			//CString s1, s2;
			//s1 = "";
			//s1 = "";
			//for (iter1 = m_pGameUserInfoList.begin(); iter1 != m_pGameUserInfoList.end(); ++iter1)
			//{
			//	s2.Format("(%s, %d)", (*iter1)->m_UserData.szName, (*iter1)->GetWeigth());
			//	s1 += " | " + s2;
			//}
			//ouf<<s1<<endl;

			 // PengJiLin, 2010-5-20, �������ֻʹ��һ�Σ��ڶ��λ��ߵ����λ������������
			 // ͬʱ���ⲻ��һ��������ӵ�����
			BOOL bUseOnce = FALSE;
			//����û�п�ʼ��Ϸ������
			while (m_pGameUserInfoList.size() >= m_uQueuerPeople + iDeskPlayerNum && (1 + iDeskIndex) < m_uDeskCount)
			{
				iADeskPlayerNum = 0;
				isGameing = true;
				pGameDesk = NULL;

				++iDeskIndex;
				pGameDesk=*(m_pDesk+iDeskIndex);
				
				if (NULL != pGameDesk)
				{
					isGameing = pGameDesk->IsPlayingByGameStation();
					if (!isGameing)
					{
						//isADeskGameBegin = false;
						//��������ж�����
						iADeskPlayerNum = pGameDesk->GetDeskPlayerNum();

						/// ��������δ�������������˺�û����
						if (iADeskPlayerNum < pGameDesk->m_bMaxPeople)
						{
							int iQueueSitDownNum = 0;	//һ���������Ŷ����µ�����					
							tempList = m_pGameUserInfoList;

							// PengJiLin, 2010-5-20
							UINT iCount = 0;

							//һ���Ŷ�һ������
							for (iter1 = tempList.begin(); iter1 != tempList.end(); ++iter1)
							{
								pGameUserInfo = *iter1;						
								iDeskStation = pGameDesk->GetDeskNoPlayerStation();		//��ȡ���ڵĿ���λ����

								if (-1 == iDeskStation)//������
								{
									break;
								}

								if (NULL != pGameUserInfo)
								{
									if (pGameUserInfo->m_UserData.bDeskNO == 255)//û����������
									{
										//�ж��������Ƿ���ip��ͬ�����
										if (pGameDesk->IsDeskIpSame(pGameUserInfo))
										{
											continue;
										}

										//// PengJiLin, 2010-5-20, �����޳���λ��
										//if((pArray[iCount] == -1) && (FALSE == bUseOnce))
										//{
										//	++iCount;
										//	continue;
										//}

										++iQueueSitDownNum;
										pGameUserInfo->m_UserData.bUserState = USER_ARGEE; //USER_SITTING;
										pGameUserInfo->m_UserData.bDeskNO = iDeskIndex;
										pGameUserInfo->m_UserData.bDeskStation=iDeskStation;								
										pGameDesk->SetDeskUserInfo(pGameUserInfo, iDeskStation);

										//���Ŷӻ���ɾ���Ѿ�������λ�����
										m_pGameUserInfoList.remove(pGameUserInfo);
										//�����Ѿ����µ����
										sitdownList.push_back(pGameUserInfo);
			
										if (iQueueSitDownNum == pGameDesk->m_bMaxPeople)
										{
											break;//������
										}
									}
								}
							}//end of for

							// PengJiLin, 2010-5-20
							bUseOnce = TRUE;
							
							//���������ˣ��������ˡ������˵����
							if (0 < iQueueSitDownNum)
							{
								//�Ƿ��������
								if (pGameDesk->CanBeginGame())
								{
									//pGameDesk->GameBegin(0);
									//CString s;
									//s.Format("%d, %d", IsUserPlayGame(pGameDesk->m_pUserInfo[0]), pGameDesk->m_pUserInfo[0]->m_UserData.bUserState);
									//AfxMessageBox(s);
									//��һ�������Ŷӳɹ�����ʼ��Ϸ�ˣ�����client�����¡���Ϸ��ʼ��Ϣ����sitdownList��ȡ�����Ϣ
									//���ͷ���������Ϣ
									for (iter1 = sitdownList.begin(); iter1 != sitdownList.end(); ++iter1)
									{
										pGameUserInfo = NULL;
										pGameUserInfo = *iter1;
										if (NULL != pGameUserInfo)
										{
											hasSucceed = true;
											pGameUserInfo->m_SelectWeight = 0;
											MSG_GR_R_UserSit UserSit;
											memset(&UserSit, 0, sizeof(UserSit));
											UserSit.dwUserID = pGameUserInfo->m_UserData.dwUserID;
											UserSit.bDeskIndex = pGameUserInfo->m_UserData.bDeskNO;
											UserSit.bDeskStation = pGameUserInfo->m_UserData.bDeskStation;
											UserSit.bUserState = USER_SITTING;
											m_TCPSocket.SendDataBatch(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_QUEUE_USER_SIT,ERR_GR_SIT_SUCCESS);
										}
									}							
									//end of ��client����Ϣ
									//pGameDesk->GameBegin(0);
								}
								else//�����ŶӺ��ܿ�ʼ��Ϸ����ԭ֮ǰ���õ�һЩ��Ϣ
								{
									for (iter1 = sitdownList.begin(); iter1 != sitdownList.end(); ++iter1)
									{
										pGameUserInfo = NULL;
										pGameUserInfo = *iter1;
										if (NULL != pGameUserInfo)
										{
											pGameDesk->SetDeskUserInfo(NULL, pGameUserInfo->m_UserData.bDeskStation);//�����λ�ϵ������Ϣ
											pGameUserInfo->m_UserData.bDeskNO=255;
											pGameUserInfo->m_UserData.bDeskStation=255;
											pGameUserInfo->m_UserData.bUserState=USER_LOOK_STATE;
										}
									}						
									m_pGameUserInfoList.swap(tempList);//�Ŷ�ʧ�ܻ�ԭ��list
								}

								tempList.clear();
								sitdownList.clear();
							}
						}//end of if (iADeskPlayerNum < pGameDesk->m_bMaxPeople)
					}//end of if (!isGameing)
				}//end of if (NULL != pGameDesk)			
			}// end of while (m_pGameUserInfoList.size() > 0 && (1 + iDeskIndex) < m_uDeskCount)
		}
	

		//{���� rongqiufen 2010.06.29
		///�����ŶӲ��ɹ��Ľ��м�Ȩ
		if (hasSucceed)
		{
			for (iter1 = m_pGameUserInfoList.begin(); iter1 != m_pGameUserInfoList.end(); iter1++)
			{
				if ((*iter1)->m_UserData.bDeskNO == 255) 
				{
					if ((*iter1)->GetWeigth() < MAXLONG - 50) 
						(*iter1)->m_SelectWeight += 30;
				}
			}
		}
       //���� rongqiufen}

		//s1 = "---";
		//for (iter1 = m_pGameUserInfoList.begin(); iter1 != m_pGameUserInfoList.end(); ++iter1)
		//{
		//	s2.Format("%s, %d", (*iter1)->m_UserData.szName, (*iter1)->GetWeigth());
		//	s1 += " | " + s2;
		//}
		//ouf<<s1<<endl;
	}

/*

	
    // PengJiLin, 2010-5-20, ���Ŷӻ�λ������޳�
    memset(pArray, 0, m_uQueuerMaxPeople);
    if(m_uQueuerMaxPeople > m_KernelData.uDeskPeople)
    {
        int iCount = m_uQueuerMaxPeople - m_KernelData.uDeskPeople;
        for(int i = 0; i < iCount; ++i)
        {
            UINT uIndex = (UINT)rand() % m_uQueuerMaxPeople;

            if(pArray[uIndex] == 0) pArray[uIndex] = -1;
            else
            {
                for(int i = 0; i < m_uQueuerMaxPeople; ++i)
                {
                    if(pArray[i] == 0)
                    {
                        pArray[i] = -1;
                        break;
                    }
                }
            }
        }
    }

    // PengJiLin, 2010-5-20, �������ֻʹ��һ�Σ��ڶ��λ��ߵ����λ������������
    // ͬʱ���ⲻ��һ��������ӵ�����
    BOOL bUseOnce = FALSE;

	//����û�п�ʼ��Ϸ������
	while (m_pGameUserInfoList.size() > 0 && (1 + iDeskIndex) < m_uDeskCount)
	{
		iADeskPlayerNum = 0;
		isGameing = true;
		pGameDesk = NULL;

		++iDeskIndex;
		pGameDesk=*(m_pDesk+iDeskIndex);
		
		if (NULL != pGameDesk)
		{
			isGameing = pGameDesk->IsPlayingByGameStation();
			if (!isGameing)
			{
				//isADeskGameBegin = false;
				//��������ж�����
				iADeskPlayerNum = pGameDesk->GetDeskPlayerNum();

				/// ��������δ�������������˺�û����
				if (iADeskPlayerNum < pGameDesk->m_bMaxPeople)
				{
					int iQueueSitDownNum = 0;	//һ���������Ŷ����µ�����					
					tempList = m_pGameUserInfoList;

                    // PengJiLin, 2010-5-20
                    UINT iCount = 0;

					//һ���Ŷ�һ������
					for (iter1 = tempList.begin(); iter1 != tempList.end(); ++iter1)
					{
						pGameUserInfo = *iter1;						
						iDeskStation = pGameDesk->GetDeskNoPlayerStation();		//��ȡ���ڵĿ���λ����

						if (-1 == iDeskStation)//������
						{
							break;
						}

						if (NULL != pGameUserInfo)
						{
							if (pGameUserInfo->m_UserData.bDeskNO == 255)//û����������
							{
								//�ж��������Ƿ���ip��ͬ�����
								if (pGameDesk->IsDeskIpSame(pGameUserInfo))
								{
									continue;
								}

                                // PengJiLin, 2010-5-20, �����޳���λ��
                                if((pArray[iCount] == -1) && (FALSE == bUseOnce))
                                {
                                    ++iCount;
                                    continue;
                                }

								++iQueueSitDownNum;
								pGameUserInfo->m_UserData.bUserState = USER_ARGEE; //USER_SITTING;
								pGameUserInfo->m_UserData.bDeskNO = iDeskIndex;
								pGameUserInfo->m_UserData.bDeskStation=iDeskStation;								
								pGameDesk->SetDeskUserInfo(pGameUserInfo, iDeskStation);

								//���Ŷӻ���ɾ���Ѿ�������λ�����
								m_pGameUserInfoList.remove(pGameUserInfo);
								//�����Ѿ����µ����
								sitdownList.push_back(pGameUserInfo);
	
								if (iQueueSitDownNum == pGameDesk->m_bMaxPeople)
								{
									break;//������
								}
							}
						}
					}//end of for

                    // PengJiLin, 2010-5-20
                    bUseOnce = TRUE;
					
					//���������ˣ��������ˡ������˵����
					if (0 < iQueueSitDownNum)
					{
						//�Ƿ��������
						if (pGameDesk->CanBeginGame())
						{
							//��һ�������Ŷӳɹ�����ʼ��Ϸ�ˣ�����client�����¡���Ϸ��ʼ��Ϣ����sitdownList��ȡ�����Ϣ
							//���ͷ���������Ϣ
							for (iter1 = sitdownList.begin(); iter1 != sitdownList.end(); ++iter1)
							{
								pGameUserInfo = NULL;
								pGameUserInfo = *iter1;
								if (NULL != pGameUserInfo)
								{
									MSG_GR_R_UserSit UserSit;
									memset(&UserSit, 0, sizeof(UserSit));
									UserSit.dwUserID=pGameUserInfo->m_UserData.dwUserID;
									UserSit.bDeskIndex=pGameUserInfo->m_UserData.bDeskNO;
									UserSit.bDeskStation=pGameUserInfo->m_UserData.bDeskStation;
									UserSit.bUserState=USER_SITTING;
									m_TCPSocket.SendDataBatch(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_QUEUE_USER_SIT,ERR_GR_SIT_SUCCESS);
								}
							}							
							//end of ��client����Ϣ
							//pGameDesk->GameBegin(0);
						}
						else//�����ŶӺ��ܿ�ʼ��Ϸ����ԭ֮ǰ���õ�һЩ��Ϣ
						{
							for (iter1 = sitdownList.begin(); iter1 != sitdownList.end(); ++iter1)
							{
								pGameUserInfo = NULL;
								pGameUserInfo = *iter1;
								if (NULL != pGameUserInfo)
								{
									pGameDesk->SetDeskUserInfo(NULL, pGameUserInfo->m_UserData.bDeskStation);//�����λ�ϵ������Ϣ
									pGameUserInfo->m_UserData.bDeskNO=255;
									pGameUserInfo->m_UserData.bDeskStation=255;
									pGameUserInfo->m_UserData.bUserState=USER_LOOK_STATE;
								}
							}						
							m_pGameUserInfoList.swap(tempList);//�Ŷ�ʧ�ܻ�ԭ��list
						}

						tempList.clear();
						sitdownList.clear();
					}
				}//end of if (iADeskPlayerNum < pGameDesk->m_bMaxPeople)
			}//end of if (!isGameing)
		}//end of if (NULL != pGameDesk)			
	}// end of while (m_pGameUserInfoList.size() > 0 && (1 + iDeskIndex) < m_uDeskCount)
*/	
	return;
}

//�û����䴦��
bool CGameMainManage::OnRoomMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	ASSERT(pNetHead->bMainID==MDM_GR_ROOM);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_NORMAL_TALK:	//��ͨ����
		{
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pData;
			/*if(!coreProcess(pNormalTalk->szMessage))
				return false;*/
			if(m_InitData.dwRoomRule& GRR_FORBID_ROOM_TALK)//��ֹ��������
				return false;
			//Ч������ 
			//MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pData;
			if ((uSize<=(sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)))||(uSize>sizeof(MSG_GR_RS_NormalTalk))) return false;
			if ((pNormalTalk->iLength+1)!=(uSize+sizeof(pNormalTalk->szMessage)-sizeof(MSG_GR_RS_NormalTalk))) return false;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;

			//{{ Added by zxd 20100806
			/// �������ʱ����Ч���򲻷���
			CString strTime;
			strTime.Format("%d",CTime::GetCurrentTime());
			int curTime=atoi(strTime);
			if (pUserInfo->m_UserData.userInfoEx2 > curTime)
			{
				return true;
			}
			// Added by zxd 20100806}}

			//Ⱥ������
			pNormalTalk->szMessage[pNormalTalk->iLength]=0;
			pNormalTalk->dwSendID=pUserInfo->m_UserData.dwUserID;
			
			if (!m_TalkFilter.IsAllowableTalk(pNormalTalk->szMessage,lstrlen(pNormalTalk->szMessage)))
			{
				const TCHAR* pTemTalk = m_TalkFilter.GetReplaceContent();
				strcpy(pNormalTalk->szMessage,pTemTalk);
				pNormalTalk->iLength = strlen(pTemTalk) + 1;
				pNormalTalk->szMessage[pNormalTalk->iLength]=0;

				//AfxMessageBox(pNormalTalk->szMessage);
			}
			m_TCPSocket.SendDataBatch(pNormalTalk,sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)+pNormalTalk->iLength+1,MDM_GR_ROOM,ASS_GR_NORMAL_TALK,0);

			return true;
		}
	case ASS_GR_SHORT_MSG:		//����Ϣ����
		{
			//Ч������
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pData;
			if ((uSize<=(sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)))||(uSize>sizeof(MSG_GR_RS_NormalTalk))) return false;
			if ((pNormalTalk->iLength+1)!=(uSize+sizeof(pNormalTalk->szMessage)-sizeof(MSG_GR_RS_NormalTalk))) return false;

			//��������
			CGameUserInfo * pSendUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			CGameUserInfo * pRecvUserInfo=m_UserManage.FindOnLineUser(pNormalTalk->dwTargetID);
			if ((pSendUserInfo==NULL)||(pRecvUserInfo==NULL)) return true;
			pNormalTalk->szMessage[pNormalTalk->iLength]=0;
			pNormalTalk->dwSendID=pSendUserInfo->m_UserData.dwUserID;

			if (!m_TalkFilter.IsAllowableTalk(pNormalTalk->szMessage,lstrlen(pNormalTalk->szMessage)))
			{
				const TCHAR* pTemTalk = m_TalkFilter.GetReplaceContent();
				strcpy(pNormalTalk->szMessage,pTemTalk);
				pNormalTalk->iLength = strlen(pTemTalk) + 1;
				pNormalTalk->szMessage[pNormalTalk->iLength]=0;

				//AfxMessageBox(pNormalTalk->szMessage);
				//return true;
			}

			//��������
			m_TCPSocket.SendData(pSendUserInfo->m_uSocketIndex,pNormalTalk,sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)+pNormalTalk->iLength+1,MDM_GR_ROOM,ASS_GR_SHORT_MSG,0,pSendUserInfo->m_dwHandleID);
			m_TCPSocket.SendData(pRecvUserInfo->m_uSocketIndex,pNormalTalk,sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)+pNormalTalk->iLength+1,MDM_GR_ROOM,ASS_GR_SHORT_MSG,0,pRecvUserInfo->m_dwHandleID);

			return true;
		}
	case ASS_GR_ROOM_SET:		//��������
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_UserSet)) return false;
			MSG_GR_S_UserSet * pUserSetInfo=(MSG_GR_S_UserSet *)pData;

			//��������
			CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			if (pUserInfo==NULL) return false;
			pUserSetInfo->m_Rule.szPass[sizeof(pUserSetInfo->m_Rule.szPass)/sizeof(pUserSetInfo->m_Rule.szPass[0])-1]=0;
			pUserInfo->m_Rule=pUserSetInfo->m_Rule;

            // PengJiLin, 2010-9-14, �ȽϹ����еĻ��ֱȽ��Ƿ��Ϊ��ұȽ�
//             CBcfFile fConfig(CBcfFile::GetAppPath()+"Function.bcf");
//             int iGold = fConfig.GetKeyVal("CommFunc","UseGoldValue",0);
//             if(iGold > 0 && pUserInfo->m_Rule.bLimitPoint != 0)
//             {
//                 pUserInfo->m_Rule.bLimitPoint |= 0x80;
//             }

			return true;
		}
	case ASS_GR_INVITEUSER:		//�����û�
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_SR_InviteUser)) return false;
			MSG_GR_SR_InviteUser * pInviteUser=(MSG_GR_SR_InviteUser *)pData;

			//��������
			CGameUserInfo * pSendUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
			CGameUserInfo * pRecvUserInfo=m_UserManage.FindOnLineUser(pInviteUser->dwTargetID);
			if ((pSendUserInfo==NULL)||(pRecvUserInfo==NULL)) return true;
			if (pSendUserInfo->m_UserData.bDeskNO==255) return true;
			pNetHead->bHandleCode=pSendUserInfo->m_UserData.bDeskNO;
			pInviteUser->dwUserID=pSendUserInfo->m_UserData.dwUserID;

			//��������
			m_TCPSocket.SendData(pRecvUserInfo->m_uSocketIndex,pInviteUser,uSize,MDM_GR_ROOM,ASS_GR_INVITEUSER,
				pNetHead->bHandleCode,pRecvUserInfo->m_dwHandleID);

			return true;
		}
	case ASS_GR_ROOM_PASSWORD_SET:
		{
			BOOL bRecord = FALSE;
			char bufID[12] = {0};
			CGameUserInfo * pSendUserInfo = m_UserManage.GetOnLineUserInfo(uIndex);

			CBcfFile fConfig(CBcfFile::GetAppPath()+"Function.bcf");
			CString strBuffer = fConfig.GetKeyVal("SpecificID","Available","");
			if (atoi(strBuffer)) ///< ָ���˺�ID
			{
				if (0 ==_tcslen(m_szVIPPassWord)) ///< VIP��������Ϊ��ʼ����
				{
					strBuffer = fConfig.GetKeyVal("SpecificID","NormalID","60000000,69999999");

					int iVIP = GetIDInformation(pSendUserInfo->m_UserData.dwUserID,
						atol(strBuffer.Left(strBuffer.Find(",")+1)),
						atol(strBuffer.Right(strBuffer.GetLength()-strBuffer.Find(",")-1)));

					if ((iVIP>0 && iVIP<7) || (iVIP>-9 && iVIP<-4))
					{
						if (uSize>50)
							break;
						
						m_lVIPID = pSendUserInfo->m_UserData.dwUserID;
						
						bRecord = TRUE;
					}
				}
				else if (pSendUserInfo->m_UserData.dwUserID == m_lVIPID)
				{
					bRecord = TRUE;
				}

				///< �������ݿ�,��¼���������޸�.
				if (bRecord)
				{
					wsprintf(bufID,"%ld",m_lVIPID);
					memcpy(m_szVIPPassWord,pData,uSize);
					m_szVIPPassWord[uSize] = 0;
					m_TCPSocket.SendDataBatch(bufID,strlen(bufID),MDM_GR_ROOM,ASS_GR_ROOM_PASSWORD_SET,dwHandleID);

					DL_GR_RoomPWChange pRoomPW;
					pRoomPW.dwUserID = pSendUserInfo->m_UserData.dwUserID;
					pRoomPW.dwRoomID = m_InitData.uRoomID;
					m_SQLDataManage.PushLine(&pRoomPW.DataBaseHead,sizeof(DL_GR_RoomPWChange),DTK_GR_ROOM_PW_CHANGE,uIndex,dwHandleID);
				}
			}
			
			return true;
		}
    case ASS_GR_GET_NICKNAME_ONID:  // PengJiLin, 2010-8-23, ����ID��ȡ�ǳ�
        {
            // Ч������
            if (uSize!=sizeof(MSG_GR_S_GetNickNameOnID_t)) return false;
            MSG_GR_S_GetNickNameOnID_t *pGetNickNameOnID =(MSG_GR_S_GetNickNameOnID_t *)pData;

            // �������ݿ�
            DL_GR_I_GetNickNameOnID stGetNickNameOnID;
            stGetNickNameOnID.stGetNickNameOnID.iUserID = pGetNickNameOnID->iUserID;
            m_SQLDataManage.PushLine(&stGetNickNameOnID.DataBaseHead,sizeof(stGetNickNameOnID),DTK_GR_GETNICKNAME_ONID,uIndex,dwHandleID);

            return true;
        }
    break;
    case ASS_GR_GET_NICKNAME_ONID_INGAME:  // lxl, 2010-11-17, ����ID��ȡ�ǳ�
        {
            // Ч������
			OutputDebugString("��Ϸ�л�ȡID�ǳ�");
            if (uSize!=sizeof(MSG_GR_S_GetNickNameOnID_t)) return false;
            MSG_GR_S_GetNickNameOnID_t *pGetNickNameOnID =(MSG_GR_S_GetNickNameOnID_t *)pData;

            // �������ݿ�
            DL_GR_I_GetNickNameOnID stGetNickNameOnID;
            stGetNickNameOnID.stGetNickNameOnID.iUserID = pGetNickNameOnID->iUserID;
            m_SQLDataManage.PushLine(&stGetNickNameOnID.DataBaseHead,sizeof(stGetNickNameOnID),DTK_GR_GETNICKNAME_ONID_INGAME,uIndex,dwHandleID);

            return true;
        }
    break;
    case ASS_GR_OWNER_T_ONE_LEFT_ROOM:      // PengJiLin, 2010-8-26, ����������뿪����
        {
            //Ч������
            if (uSize!=sizeof(MSG_GR_S_CutUser)) return false;
            MSG_GR_S_CutUser * pCutUser=(MSG_GR_S_CutUser *)pData;

            CGameUserInfo * pTargetUserInfo=m_UserManage.FindOnLineUser(pCutUser->dwTargetID);
            if (pTargetUserInfo==NULL) return true;

            char bufID[12] = {0};
            wsprintf(bufID,"%ld",pCutUser->dwTargetID);
            m_TCPSocket.SendData(pTargetUserInfo->m_uSocketIndex, bufID, sizeof(bufID),
                                MDM_GR_ROOM,ASS_GR_OWNER_T_ONE_LEFT_ROOM,0,pTargetUserInfo->m_dwHandleID);

            return true;
        }
        break;

	case ASS_GR_AVATAR_LOGO_CHANGE: //JianGuankun 2012.6.28,�����ĳ���logo����ı�
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_AvatarChange)) return false;
			MSG_GR_AvatarChange* pAvatarChange = (MSG_GR_AvatarChange*)pData;

			CGameUserInfo * pTargetUserInfo = m_UserManage.FindOnLineUser(pAvatarChange->dwUserID);
			if (pTargetUserInfo == NULL) return true;

			pTargetUserInfo->GetUserData(NULL)->bLogoID = pAvatarChange->nNewLogoID;

			m_TCPSocket.SendDataBatch(pAvatarChange,sizeof(MSG_GR_AvatarChange),MDM_GR_ROOM,ASS_GR_AVATAR_LOGO_CHANGE,0);

			return true;
		}
		break;
	}
	return false;
}

//�û���½���� 
bool CGameMainManage::OnUserLogonMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	ASSERT(pNetHead->bMainID==MDM_GR_LOGON);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_IS_VIPROOM: ///< ƽ̨�ͻ��˷��Ͳ�ѯ�Ƿ�VIP����
		{
			if (m_InitData.bVIPRoom) ///< �����뷿��
			{
				if (0 == m_UserManage.GetGameRoomCount()) ///< �����ڷ���
				{
					m_lVIPID = -1;
				memset(m_szVIPPassWord,0,sizeof(m_szVIPPassWord));
				m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIP_NO_PW,dwHandleID);
			}
				else ///< �����ڷ���
				{
					if (0 != strlen(m_szVIPPassWord)) ///< �����Ѿ���������
					{
						CGameUserInfo * pGameUserInfo = m_UserManage.FindOnLineUser(m_lVIPID);
						if (NULL == pGameUserInfo) ///< ����������˲��ڷ���
						{
							if (atol((char*)pData) == m_lVIPID) ///< �ǵ�һ�����÷���������û�������Ҫ��������.
							{
								if (!m_bIsVIPLogon)
								{
									m_bIsVIPLogon = TRUE;
									m_uVIPIndex = uIndex;
									m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIP_NO_PW,dwHandleID);
								}
								else ///< �Ѿ��и��û�������ID���޸ķ��������ID�ˡ�
								{
									m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIP_PW,dwHandleID);
								}
							}
							else ///< ��������������ˣ���Ҫ�������롣
							{
								m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIP_PW,dwHandleID);
							}
						}
						else ///< ������������ڷ��䣬��������Ҫ�������롣
						{
							m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIP_PW,dwHandleID);
						}
					}
					else ///< ����û����������,����Ҫ��������
					{
						m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIP_NO_PW,dwHandleID);
					}
				}
			}
			else ///< �������뷿��,����Ҫ�������롣
			{
				m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_NO_VIP,dwHandleID);
			}

			return true;
		}

	case ASS_GR_VIPROOM_PW: ///< ƽ̨�ͻ��˷���VIP��������
		{
			if (uSize < 50)
			{
				TCHAR Password[50] = {0};
				memcpy(Password,pData,uSize);
				
				if (0 == lstrcmp(m_szVIPPassWord,Password))
				{
					m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,ASS_GR_VIPROOM_PW_RIGHT,dwHandleID);
					return true;
				}
			}

			m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_USER_PASS_ERROR,dwHandleID);
			return true;
		}

	case ASS_GR_LOGON_BY_ID:		//ͨ�� ID ��½
		{
			//Ч������
			if (uSize!=sizeof(MSG_GR_S_RoomLogon)) return false;
			MSG_GR_S_RoomLogon * pLogonInfo=(MSG_GR_S_RoomLogon *)pData;
			if (pLogonInfo->uNameID!=m_KernelData.uNameID) return false;
			pLogonInfo->szMD5Pass[sizeof(pLogonInfo->szMD5Pass)/sizeof(pLogonInfo->szMD5Pass[0])-1]=0;
			m_bStopLogon=m_InitData.uStopLogon;
			//�жϵ�½״̬
			if (m_bStopLogon)
			{
				SendRoomMessage(uIndex,dwHandleID,TEXT("��Ϸ�����Ѿ���ͣ���룡"),SHS_MESSAGE_BOX|SHS_SYSTEM_SHOW,0,TRUE);
				m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_STOP_LOGON,dwHandleID);
				return true;
			}

			//���������û�
			CGameUserInfo * pGameUserInfo=m_UserManage.FindOnLineUser(pLogonInfo->dwUserID);
			if (pGameUserInfo!=NULL)
			{
				m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_ACCOUNTS_IN_USE,dwHandleID);
				return true;
			}

			if (m_InitData.bVIPRoom &&			 ///< �����뷿��
				0!=strlen(m_szVIPPassWord) &&	 ///< �Ѿ���������
				NULL==pGameUserInfo &&			 ///< ����������˲��ڷ���
				m_bIsVIPLogon &&
				m_uVIPIndex==uIndex &&			 ///< ����������˵��׽�������
				m_lVIPID!=pLogonInfo->dwUserID)  ///< ���˵�ID����VIP ID
			{
				m_bIsVIPLogon = FALSE;
				m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_STOP_LOGON,dwHandleID);
				return 0;
			}
			else if (m_InitData.bVIPRoom &&
				0!=strlen(m_szVIPPassWord) &&
				NULL==pGameUserInfo &&
				m_bIsVIPLogon &&
				m_uVIPIndex==uIndex &&
				m_lVIPID==pLogonInfo->dwUserID)
			{
				m_bIsVIPLogon = FALSE;
			}

			//���Ҷ����û�
			CGameUserInfo * pLostUserInfo=m_UserManage.FindNetCutUser(pLogonInfo->dwUserID);
			if (pLostUserInfo!=NULL)
			{
				//Ч���û�����
				if (lstrcmp(pLostUserInfo->m_szMD5Pass,pLogonInfo->szMD5Pass)!=0) 
				{
					m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_USER_PASS_ERROR,dwHandleID);
					return true;
				}
				if ((pLostUserInfo->m_UserData.userInfoEx2 & 0x80000000) > 0)
				{
					m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_STOP_LOGON,dwHandleID);
					return true;
				}

				CTime _t = CTime::GetCurrentTime();
				if (m_InitData.dwRoomRule & GRR_CONTEST && (pLostUserInfo->m_UserData.timeLeft + m_InitData.iTimeout * 60 < _t.GetTime()))
				{
					m_TCPSocket.SendData(uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,ERR_GR_CONTEST_TIMEOUT,dwHandleID);
					return true;
				}

				//�����û�����
				pLostUserInfo->m_UserData.bUserState=USER_PLAY_GAME;
				pGameUserInfo=m_UserManage.ActiveUser(&pLostUserInfo->m_UserData,uIndex,dwHandleID,pLostUserInfo->m_dwGamePower,
					pLostUserInfo->m_dwMasterPower,pLostUserInfo->m_dwAccessLogID);

				//д���û�������Ϣ
				pGameUserInfo->SetMD5Pass(pLogonInfo->szMD5Pass);
				pGameUserInfo->m_ChangePoint=pLostUserInfo->m_ChangePoint;
				pGameUserInfo->SetUserScrPoint(pLostUserInfo->m_dwScrPoint);

				//���ͷ�����Ϣ
				if (SendRoomInfo(pGameUserInfo,uIndex,dwHandleID,NULL)==false)
					return false;

				//����״̬����
				ASSERT(pGameUserInfo->m_UserData.bDeskNO!=255);
				if (pGameUserInfo->m_UserData.bDeskNO!=255)
				{
					CGameDesk * pGameDesk=*(m_pDesk+pGameUserInfo->m_UserData.bDeskNO);
					pGameDesk->UserReCome(pGameUserInfo->m_UserData.bDeskStation,pGameUserInfo);
				}

				//�����������
				m_UserManage.FreeUser(pLostUserInfo,false);
				SetTimer(IDT_UPDATE_ROOM_PEOPLE,100);//20081201
				return true;
			}

			//�������ݿ�
			DL_GR_I_LogonByIDStruct DL_Logon;
			DL_Logon.uAccessIP=uAccessIP;
			DL_Logon.LogonInfo=*pLogonInfo;
			m_SQLDataManage.PushLine(&DL_Logon.DataBaseHead,sizeof(DL_Logon),DTK_GR_LOGON_BY_ID,uIndex,dwHandleID);

			return true;
		}
	}
	return false;
}


//�뿪���Ӻ���
bool CGameMainManage::UserLeftDesk(CGameUserInfo * pUserInfo)
{
	//��ȡ����
	CGameDesk * pLeftDesk=*(m_pDesk+pUserInfo->m_UserData.bDeskNO);

	//CString s;
	//s.Format("%d",IsUserPlayGame(pUserInfo)); 
	//AfxMessageBox(s);

    // PengJiLin, 2010-9-17, �Ŷӻ����ܣ��������桰��ɢ���ӡ���һ�δ��룬
    // ��Ϊ����һ������뿪������������Ҷ��������ҹر���Ϸ���棬������
    // ��ҿ������Լ��Ľ���򲢱��������Ŷ�״̬

	//{���� rongqiufen 2010.06.29
	///������Ŷӻ���������Ϸ������������ ��ɢ���ӣ� �����������˽����Ŷ�׼��״̬           
	if (IsQueueGameRoom() && !IsUserPlayGame(pUserInfo))
	{
		pLeftDesk->UserLeftDesk(pUserInfo->m_UserData.bDeskStation,pUserInfo);
		MSG_GR_R_UserAgree UserAgree;
		for (int i = 0; i < pLeftDesk->m_bMaxPeople; i++)
		{
			if (pLeftDesk->m_pUserInfo[i] != NULL)
			{
				CGameUserInfo *p = m_UserManage.FindOnLineUser(pLeftDesk->m_pUserInfo[i]->m_UserData.dwUserID);
				
				//pLeftDesk->UserLeftDesk(pLeftDesk->m_pUserInfo[i]->m_UserData.bDeskStation, pLeftDesk->m_pUserInfo[i]);
				if (p  != pUserInfo)
				{
					 UINT uSocketIndex = p->m_uSocketIndex;
					 m_TCPSocket.SendData(uSocketIndex,&p->m_UserData.dwUserID,sizeof(int),MDM_GR_ROOM,ASS_GR_ROOM_QUEUE_READY, 0, 0);
				}
			}
		}
		return true;
	}
	//���� rongqiufen 2010.06.29}

	if (pUserInfo->m_UserData.bUserState==USER_WATCH_GAME)
	{
		pLeftDesk->WatchUserLeftDesk(pUserInfo->m_UserData.bDeskStation,pUserInfo);
	}
	else 
	{
		pLeftDesk->UserLeftDesk(pUserInfo->m_UserData.bDeskStation,pUserInfo);
	}

	//���ñ���
	pUserInfo->m_UserData.bDeskNO=255;
	pUserInfo->m_UserData.bDeskStation=255;
	pUserInfo->m_UserData.bUserState=USER_LOOK_STATE;


	//{���� rongqiufen
	//ǿ�˷�Ȩ
	CBcfFile fConf(CBcfFile::GetAppPath()+"Function.bcf");
	int fleeSubWeight = fConf.GetKeyVal("OnlineCoin","FleeSubWeight", 0);

	pUserInfo->m_SelectWeight = fleeSubWeight * 50;
	//���� rongqiufen}


	return true;
}
//�û��Ƿ���Ϸ
bool CGameMainManage::IsUserPlayGame(CGameUserInfo * pUserInfo)
{
	if ((pUserInfo->m_UserData.bDeskNO!=255)&&(pUserInfo->m_UserData.bUserState!=USER_WATCH_GAME))
	{
		CGameDesk * pGameDesk=*(m_pDesk+pUserInfo->m_UserData.bDeskNO);
		return pGameDesk->IsPlayGame(pUserInfo->m_UserData.bDeskStation);
	}
	return false;
}
//�û��Ƿ���Ϸ
bool CGameMainManage::IsUserPlayGameByStation(CGameUserInfo * pUserInfo)
{
	if ((pUserInfo->m_UserData.bDeskNO!=255)&&(pUserInfo->m_UserData.bUserState!=USER_WATCH_GAME))
	{
		CGameDesk * pGameDesk=*(m_pDesk+pUserInfo->m_UserData.bDeskNO);
		return pGameDesk->IsPlayingByGameStation();
	}
	return false;
}

//�Ƿ���������
bool CGameMainManage::IsLimitedTalk(CGameUserInfo * pUserInfo, bool bRoom)
{
	if (bRoom==true)
	{
		for (INT_PTR i=0;i<m_RoomLimited.GetCount();i++)
		{
			if (m_RoomLimited.GetAt(i).dwUserID==pUserInfo->m_UserData.dwUserID) return true;
		}
	}
	else
	{
		for (INT_PTR i=0;i<m_GameLimited.GetCount();i++)
		{
			if (m_GameLimited.GetAt(i).dwUserID==pUserInfo->m_UserData.dwUserID) return true;
		}
	}
	return false;
}

//SOCKET �ر�
bool CGameMainManage::OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, long int lConnectTime)
{
	CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uSocketIndex);
	if (pUserInfo!=NULL) 
	{
		//�Ƿ�����Ϸ��
		CGameUserInfo * pNetCutUserInfo=NULL;
		CGameUserInfo *pAlreadyCutUserInfo = m_UserManage.FindNetCutUser(pUserInfo->m_UserData.dwUserID);
		if (pUserInfo->m_UserData.bDeskNO!=255) 
		{
			//ע������û�
			CGameDesk * pGameDesk=*(m_pDesk+pUserInfo->m_UserData.bDeskNO);
			/// �����ж��Ƿ���Զ��ߣ�Ӧ�����κ�ʱ�򶼿��Զ��ߣ���������ڰ�������Ϸ�п���ʱǿ�ˣ��ᵼ�²����㣬ZXD 20100317
			//if ((IsUserPlayGameByStation(pUserInfo)==true)&&(pGameDesk->CanNetCut(pUserInfo->m_UserData.bDeskStation)==true))
			if (IsUserPlayGameByStation(pUserInfo))
			{
				if (pAlreadyCutUserInfo == NULL)
				{
					pNetCutUserInfo=m_UserManage.RegNetCutUser(pUserInfo);
				}
				else
				{
					pNetCutUserInfo=pAlreadyCutUserInfo;
				}
			}
			if (pNetCutUserInfo!=NULL && pUserInfo->m_UserData.bUserState != USER_WATCH_GAME)
			{
				pGameDesk->UserNetCut(pNetCutUserInfo->m_UserData.bDeskStation,pNetCutUserInfo);
				CTime t = CTime::GetCurrentTime();
				pNetCutUserInfo->m_UserData.timeLeft = t.GetTime();
				//pGameDesk->GameFinish(pNetCutUserInfo->m_UserData.bDeskStation, GFF_FORCE_FINISH);
			}
			else
			{
				UserLeftDesk(pUserInfo);

				//wushuqun 2009.5.6
				//������߱�־
				DL_GR_I_ClearOnlineFlag dtClearOnlineFlag;
				memset(&dtClearOnlineFlag, 0, sizeof(dtClearOnlineFlag));
				dtClearOnlineFlag.lUserID = pUserInfo->m_UserData.dwUserID;
				m_SQLDataManage.PushLine(&dtClearOnlineFlag.DataBaseHead, sizeof(dtClearOnlineFlag), DTK_GR_CLEAR_ONLINE_FLAG, 0, 0);
				//��ӽ���}
			}
		}

		
		//�û���û�����¾��뿪��
		if (pNetCutUserInfo==NULL)
		{
			AutoAllotDeskDeleteUser(pUserInfo);
			m_pGameUserInfoList.remove(pUserInfo);	//ɾ�������Ŷӵ����
			CleanUserInfo(pUserInfo);
		}

		if (m_InitData.dwRoomRule & GRR_CONTEST)
		{
			DL_GR_I_ContestUserLeft _t;
			_t.iUserID = pUserInfo->m_UserData.dwUserID;
			_t.iContestID = m_InitData.iContestID;
			m_SQLDataManage.PushLine(&_t.DataBaseHead, sizeof(DL_GR_I_ContestUserLeft), DTK_GR_USER_CONTEST_LEFT, 0, 0);
		}


		if (m_pIAutoAllotDesk)
		{
			AutoAllotDeskDeleteUser(pUserInfo);	//ɾ�������Ŷӵ����
		}

		//�����û�
		m_UserManage.FreeUser(pUserInfo,true);
		SetTimer(IDT_UPDATE_ROOM_PEOPLE,100);//20081201
	}
	return false;
}

//���ݿ⴦����
bool CGameMainManage::OnDataBaseResult(DataBaseResultLine * pResultData)
{
	switch (pResultData->uHandleKind)
	{
	case DTK_GR_DEMAND_MONEY_IN_GAME:	//����Ϸ�����Ǯ��
	case DTK_GR_DEMAND_MONEY:	//��Ǯ��
		{
			return OnAboutMoneyResult(pResultData, pResultData->uHandleKind);
		}
	case DTK_GR_CHECKIN_MONEY:	//���䣭��Ǯ��
	case DTK_GR_CHECKIN_MONEY_INGAME:	//���䣭��Ǯ��
		{
			return OnCheckInMoney(pResultData, pResultData->uHandleKind);
		}
	case DTK_GR_CHECKOUT_MONEY_INGAME: //��Ϸ��ȡǮ<��Ǯ��
	case DTK_GR_CHECKOUT_MONEY: //����<��Ǯ��
		{
			return OnCheckOutMoney(pResultData, pResultData->uHandleKind);
		}
	case DTK_GR_TRANSFER_MONEY://ת��
	case DTK_GR_TRANSFER_MONEY_INGAME://��Ϸ��ת��
		{
			return OnTransferMoney(pResultData, pResultData->uHandleKind);
		}
	case DTK_GR_CHANGE_PASSWD:		 ///<�޸�����
	case DTK_GR_CHANGE_PASSWD_INGAME:///<��Ϸ���޸�����
		{
			return OnChangePasswd(pResultData, pResultData->uHandleKind);
		}
	case DTK_GR_TRANSFER_RECORD:			///<ת�ʼ�¼
	case DTK_GR_TRANSFER_RECORD_INGAME:		///<��Ϸ��ת�ʼ�¼
		{
			return OnTransferRecord(pResultData, pResultData->uHandleKind);
		}
	case DTK_GR_LOGON_BY_ID:	//�����½
		{
			return OnRoomLogonResult(pResultData);
		}
	case DTK_GR_CHECK_CONNECT:	//�������
		{
			return OnCheckConnect(pResultData);
		}
	case DTK_GR_CALL_GM_RESULT://�������ܽ��
		{
			return OnCallGMResult(pResultData);
		}
	case DTK_GR_GAME_POWER_RESULT://������ϷȨ��
		{
			return OnGamePowerSetResult(pResultData);
		}	
	case DTK_GPR_PROP_SMALL_BRD:
	case DTK_GR_PROP_USE://ʹ�õ��� 
		{
			return m_PropService->OnDataBaseResult(pResultData);
		}
	case DTK_GR_SENDMONEY_TIMES:
		{
			return SendUserMoenyByTimesResult(pResultData);		
		}
    case DTK_GR_SEND_MONEY_ON_TIMES_COUNTS:     // PengJiLin, 2011-4-15, ����ʱ���������ͽ��
        {
            return OnSendMoneyOnTimesAndCounts(pResultData);
        }

		//wushuqun 2009.6.5
		//��ս������Ϸ��¼���
	case DTK_GR_BATTLEROOM_RES:
		{
			return OnBattleRoomRecord(pResultData);		
		}
	//case DTK_GR_READ_USER_HOLD_ITEM://���е�����
	//case DTK_GR_USER_USE_ONE_ITEM://ʹ�õ�����
	//case DTK_GR_GET_BUY_ITEM_DATA://�򿪹����б���
	//case DTK_GR_BUY_ITEM:			//�������
	////case DTK_GR_CHECK_VIP_TIMER://vipʱ����
	//	{
	//		return m_ItemDate->OnDataBaseResult(pResultData);
	//	}

		//case DTK_GR_FRIEND_MANAGE://���ѹ���
		//	{
		//		return OnFriendManageDBOperateResult(pResultData);
		//	}
		//case DTK_GR_BUFFER_IM_MESSAGE://������Ϣ
		//	{
		//		return OnBufferIMMessageDBOperateResult(pResultData);
		//	}
    case DTK_GR_GETNICKNAME_ONID:
	case DTK_GR_GETNICKNAME_ONID_INGAME:
    {
        return OnGetNickNameOnIDResult(pResultData);
    }
    case DTK_GR_USE_KICK_PROP:  // PengJiLin, 2010-9-10, ���˿�
    {
        return OnUseKickPropResult(pResultData, TRUE);
    }
    case DTK_GR_USE_ANTI_KICK_PROP: // PengJiLin, 2010-9-10, ���߿�
    {
        return OnUseKickPropResult(pResultData, FALSE);
    }
    case DTK_GR_USER_NEW_KICK_USER_PROP:    // PengJiLin, 2010-9-13, ʹ�����˿�����
    {
        return OnNewKickUserPropResult(pResultData);        
    }
	case DTK_GR_BANK_NORMAL:
	case DTK_GR_BANK_TRANSFER:
	case DTK_GR_BANK_DEPOSIT:
	case DTK_GR_BANK_WALLET:
	case DTK_GR_BANK_QUERY_DEPOSIT:
	{
		if (m_pBankService != NULL)
		{
			m_pBankService->OnDataBaseResult(pResultData);
		}
	}
    break;
	case DTK_GR_GET_CONTEST_RESULT: //JianGuankun,2012.7.16��ȡ������ǰ�������ɼ���
	{
		return OnGetContestRankResult(pResultData);
	}
	break;
    }
	return false;
}

//��ȡָ����������
CGameDesk * CGameMainManage::GetDeskObject(BYTE bIndex)
{
	return 	*(m_pDesk+bIndex);
}

//wushuqun 2009.6.5
//��ս�����¼�������
bool CGameMainManage::OnBattleRoomRecord(DataBaseResultLine * pResultData)
{
	if (pResultData->uHandleRusult == DTR_GR_BATTLEROOM_TIMEOUT_ERROR)
	{
		//��ս�����Ѳ�����Чʱ����
		DL_GR_O_BattleRecordResult * battleRoomRet = (DL_GR_O_BattleRecordResult * )pResultData;

		//֪ͨ�ͻ��˻�ս�����ʱ���Ѿ���Ч
		BattleRoomRecordRes BattleRoomResult;
		memset(& BattleRoomResult,0,sizeof(BattleRoomResult));
		BattleRoomResult.uRoomID = battleRoomRet->uRoomID;
		BattleRoomResult.uRecordResult = battleRoomRet->uRecordResult;
		
		BYTE bHandleCode = 0;

		m_TCPSocket.SendDataBatch(&BattleRoomResult,sizeof(BattleRoomResult),MDM_GR_ROOM,ASS_GR_BATTLEROOM_RESULT,bHandleCode);
	}
	return true;
}

//�����û�����
bool CGameMainManage::CleanUserInfo(CGameUserInfo * pUserInfo)
{
	//������Ϣ
	MSG_GR_R_UserLeft UserLeft;
	UserLeft.dwUserID=pUserInfo->m_UserData.dwUserID;
	if (pUserInfo->m_UserData.bGameMaster==0)
	{
		m_TCPSocket.SendDataBatch(&UserLeft,sizeof(UserLeft),MDM_GR_USER_ACTION,ASS_GR_USER_LEFT,0);
	}

	//��¼�û�����
	DL_GR_I_UserLeft DT_UserLeft;
	DT_UserLeft.dwUserID=pUserInfo->m_UserData.dwUserID;
	DT_UserLeft.uMaster=pUserInfo->m_UserData.bGameMaster;
	DT_UserLeft.dwScrPoint=pUserInfo->m_dwScrPoint;
	DT_UserLeft.dwChangePoint=pUserInfo->m_ChangePoint.dwPoint;
	DT_UserLeft.dwChangeMoney=pUserInfo->m_ChangePoint.dwMoney;
	DT_UserLeft.dwChangeTaxCom=pUserInfo->m_ChangePoint.dwTaxCom;
	DT_UserLeft.dwResultPoint=pUserInfo->m_UserData.dwPoint;
	DT_UserLeft.uWinCount=pUserInfo->m_ChangePoint.uWinCount;
	DT_UserLeft.uLostCount=pUserInfo->m_ChangePoint.uLostCount;
	DT_UserLeft.uMidCount=pUserInfo->m_ChangePoint.uMidCount;
	DT_UserLeft.uCutCount=pUserInfo->m_ChangePoint.uCutCount;
	DT_UserLeft.dwAccessLogID=pUserInfo->m_dwAccessLogID;
	DT_UserLeft.dwPlayTimeCount=pUserInfo->m_dwPlayCount;
	DT_UserLeft.uAccessIP=pUserInfo->m_UserData.dwUserIP;
	DT_UserLeft.dwOnLineTimeCount=(long int)time(NULL)-pUserInfo->m_dwLogonTime;
	m_SQLDataManage.PushLine(&DT_UserLeft.DataBaseHead,sizeof(DT_UserLeft),DTK_GR_USER_LEFT,0,0);
	SetTimer(IDT_UPDATE_ROOM_PEOPLE,1000);

	return true;
}

//���ͷ�����Ϣ
bool CGameMainManage::SendRoomMessage(UINT uSocketIndex, DWORD dwHandleID, TCHAR * szMessage, BYTE bShowStation, BYTE bFontSize, BYTE bCloseFace)
{
	//�������� 
	MSG_GA_S_Message Message;
	memset(&Message,0,sizeof(Message));

	//Ч������
	UINT uLength=lstrlen(szMessage);
	if (uLength>=sizeof(Message.szMessage)/sizeof(TCHAR)) return false;
	UINT uSendSize=sizeof(Message)-sizeof(Message.szMessage)+sizeof(TCHAR)*(uLength+1);

	//��������
	Message.bFontSize=bFontSize;
	Message.bCloseFace=bCloseFace;
	Message.bShowStation=bShowStation;
	lstrcpy(Message.szMessage,szMessage);
	if (uSocketIndex==m_InitData.uMaxPeople) m_TCPSocket.SendDataBatch(&Message,uSendSize,MDM_GR_MESSAGE,ASS_GR_SYSTEM_MESSAGE,0);
	else m_TCPSocket.SendData(uSocketIndex,&Message,uSendSize,MDM_GR_MESSAGE,ASS_GR_SYSTEM_MESSAGE,0,dwHandleID);

	return true;
}

//������Ϸ��Ϣ
bool CGameMainManage::SendGameMessage(UINT uSocketIndex, DWORD dwHandleID, TCHAR * szMessage, BYTE bShowStation, BYTE bFontSize, BYTE bCloseFace)
{
	//�������� 
	MSG_GA_S_Message Message;
	memset(&Message,0,sizeof(Message));

	//Ч������
	UINT uLength=lstrlen(szMessage);
	if (uLength>=sizeof(Message.szMessage)/sizeof(TCHAR)) return false;
	UINT uSendSize=sizeof(Message)-sizeof(Message.szMessage)+sizeof(TCHAR)*(uLength+1);

	//��������
	Message.bFontSize=bFontSize;
	Message.bCloseFace=bCloseFace;
	Message.bShowStation=bShowStation;
	lstrcpy(Message.szMessage,szMessage);
	if (uSocketIndex==m_InitData.uMaxPeople) m_TCPSocket.SendDataBatch(&Message,uSendSize,MDM_GM_MESSAGE,ASS_GM_SYSTEM_MESSAGE,0);
	else m_TCPSocket.SendData(uSocketIndex,&Message,uSendSize,MDM_GM_MESSAGE,ASS_GM_SYSTEM_MESSAGE,0,dwHandleID);

	return true;
}

//���ͱ�����Ϣ
bool CGameMainManage::SendMatchInfo(UINT uSocketIndex, DWORD dwHandleID)
{
	if ((m_uMatchUserCount!=0L)&&(m_pMatchUser!=NULL))
	{
		//��������
		UINT uBeenSendCount=0L;
		while (uBeenSendCount<m_uMatchUserCount)
		{
			UINT uSendCount=__min(m_uMatchUserCount-uBeenSendCount,MAX_SEND_SIZE/sizeof(MSG_GR_SR_MatchUser));
			m_TCPSocket.SendData(uSocketIndex,m_pMatchUser+uBeenSendCount,sizeof(MSG_GR_SR_MatchUser)*uSendCount,
				MDM_GR_MATCH_INFO,ASS_GR_MATCH_USER,0,dwHandleID);
			uBeenSendCount+=uSendCount;
		};
	}

	return true;
}

//���ͷ�����Ϣ
bool CGameMainManage::SendRoomInfo(CGameUserInfo * pUserInfo, UINT uSocketIndex, DWORD dwHandleID,DL_GR_O_LogonResult * pLogonResult)
{
	//���͵�½�ɹ���Ϣ
	MSG_GR_R_LogonResult MSGLogonResult;
	MSGLogonResult.uLessPoint=m_InitData.uLessPoint;
	MSGLogonResult.uMaxPoint=m_InitData.uMaxPoint;
	MSGLogonResult.dwGamePower=pUserInfo->m_dwGamePower;
	MSGLogonResult.dwMasterPower=pUserInfo->m_dwMasterPower;
	MSGLogonResult.pUserInfoStruct=pUserInfo->m_UserData;
	MSGLogonResult.dwRoomRule=m_InitData.dwRoomRule;
	MSGLogonResult.nPresentCoinNum = 0; ///< ���ͽ������
	MSGLogonResult.nVirtualUser = m_InitData.uVirtualUser; ///< �����������
	MSGLogonResult.pUserInfoStruct.dwTax = m_InitData.uTax;  /// add by wyl; 11-5-16

	MSGLogonResult.i64Chip = m_InitData.i64Chip;
	MSGLogonResult.i64LowChip = m_InitData.i64LowChip;
	MSGLogonResult.iLowCount = m_InitData.iLowCount;
	MSGLogonResult.i64TimeStart = m_InitData.i64TimeStart;
	MSGLogonResult.i64TimeEnd = m_InitData.i64TimeEnd;
	MSGLogonResult.iTimeout = m_InitData.iTimeout;
	MSGLogonResult.iBasePoint = m_InitData.uBasePoint;

	if (m_InitData.dwRoomRule & GRR_CONTEST && pUserInfo->m_UserData.iContestCount == 0 ) 
	{
		pUserInfo->m_UserData.i64ContestScore = m_InitData.i64Chip;
	}


	if (NULL != pLogonResult)
	{
		MSGLogonResult.nVirtualUser    = pLogonResult->nVirtualUser; ///< ��ʱ��ȡ���������������
		MSGLogonResult.nPresentCoinNum = pLogonResult->nPresentCoinNum; ///< ���ͽ������
	}

	if (NULL != pLogonResult)
	{
		m_TCPSocket.SendData(uSocketIndex,&MSGLogonResult,sizeof(MSGLogonResult),MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,
			pLogonResult->ResultHead.uHandleRusult,dwHandleID);
	}
	else
	{
		m_TCPSocket.SendData(uSocketIndex,&MSGLogonResult,sizeof(MSGLogonResult),MDM_GR_LOGON,ASS_GR_LOGON_SUCCESS,
			ERR_GR_LOGON_SUCCESS,dwHandleID);
	}
	




	//���������û���Ϣ
	bool bFinish=false;
	UINT uFillCount=0,uIndex=0;
	char bBuffer[MAX_SEND_SIZE];
	do
	{
		UINT uCopyPos=m_UserManage.FillOnLineUserInfo(bBuffer,sizeof(bBuffer),uIndex,uFillCount,bFinish);
		if (bFinish) 
		{
			////wushuqun 2009.5.22
			//::OutputDebugString("���� CGameUserInfoManage::FillOnLineUserInfo: ��ȡ�����û�-���");
			//::OutputDebugString("���� CGameUserInfoManage::FillOnLineUserInfo: ��ȡ�����û�-���-��ʼ���ͷ��������û���Ϣ");

			if (m_TCPSocket.SendData(uSocketIndex,bBuffer,(UINT)uCopyPos*sizeof(UserInfoStruct),
				MDM_GR_USER_LIST,ASS_GR_ONLINE_USER,ERR_GR_LIST_FINISH,dwHandleID)!=uCopyPos*sizeof(UserInfoStruct))
			{
				////wushuqun 2009.5.22
				//::OutputDebugString("���� CGameUserInfoManage::FillOnLineUserInfo: ��ȡ�����û�-���-���ͷ��������û���Ϣʧ��");
				return false;
			}
			////wushuqun 2009.5.22
			//::OutputDebugString("���� CGameUserInfoManage::FillOnLineUserInfo: ��ȡ�����û�-���-���ͷ��������û���Ϣ�ɹ�");

		}
		else 
		{
			////wushuqun 2009.5.22
			//::OutputDebugString("���� CGameUserInfoManage::FillOnLineUserInfo: ��ȡ�����û�-���-���ڷ��ͷ��������û���Ϣ");
			if (m_TCPSocket.SendData(uSocketIndex,bBuffer,(UINT)uCopyPos*sizeof(UserInfoStruct),
				MDM_GR_USER_LIST,ASS_GR_ONLINE_USER,ERR_GR_LIST_PART,dwHandleID)!=uCopyPos*sizeof(UserInfoStruct))
			{
			//	//wushuqun 2009.5.22
			//::OutputDebugString("���� CGameUserInfoManage::FillOnLineUserInfo: ��ȡ�����û�-���-���ڷ��ͷ��������û���Ϣ-ʧ��");
				return false;
			}
			//wushuqun 2009.5.22
			::Sleep(10);

		}
	} while (bFinish==false);

	//���Ͷ����û���Ϣ
	bFinish=false;
	UINT uBeginPos=0;
	do
	{
		UINT uCopyPos=m_UserManage.FillNetCutUserInfo(bBuffer,sizeof(bBuffer),uBeginPos,bFinish);
		if (bFinish) 
		{
			if (m_TCPSocket.SendData(uSocketIndex,bBuffer,(UINT)uCopyPos*sizeof(UserInfoStruct),
				MDM_GR_USER_LIST,ASS_GR_NETCUT_USER,ERR_GR_LIST_FINISH,dwHandleID)!=uCopyPos*sizeof(UserInfoStruct))
				return false;
		}
		else 
		{
			if (m_TCPSocket.SendData(uSocketIndex,bBuffer,(UINT)uCopyPos*sizeof(UserInfoStruct),
				MDM_GR_USER_LIST,ASS_GR_NETCUT_USER,ERR_GR_LIST_PART,dwHandleID)!=(UINT)uCopyPos*sizeof(UserInfoStruct))
				return false;
		}
	} while (bFinish==false);

	//����������Ϣ
	MSG_GR_DeskStation DeskStation;
	memset(&DeskStation,0,sizeof(DeskStation));
	for (UINT i=0;i<m_uDeskCount;i++)
	{
		if ((*(m_pDesk+i))->IsLock()==true) 
			DeskStation.bDeskLock[i/8]|=(1<<(i%8));
		if ((*(m_pDesk+i))->IsVirtualLock()==true) 
			DeskStation.bVirtualDesk[i/8]|=(1<<(i%8));
		if ((*(m_pDesk+i))->IsPlayingByGameStation())
			DeskStation.bDeskStation[i/8]|=(1<<(i%8));
		//if ((*(m_pDesk+i))->IsPlayGame(m_KernelData.uDeskPeople))
		//	DeskStation.bDeskStation[i/8]|=(1<<(i%8));
		if(i < 100) 
			DeskStation.iBasePoint[i]=(*(m_pDesk+i))->GetTableBasePoint();
	}
	m_TCPSocket.SendData(uSocketIndex,&DeskStation,sizeof(DeskStation),MDM_GR_USER_LIST,ASS_GR_DESK_STATION,m_uDeskCount,dwHandleID);

	//�����û�������Ϣ
	MSG_GR_R_UserCome UserCome;
	UserCome.pUserInfoStruct=pUserInfo->m_UserData;
	if (pUserInfo->m_UserData.bGameMaster==0)
	{
		m_TCPSocket.SendDataBatch(&UserCome,sizeof(UserCome),MDM_GR_USER_ACTION,ASS_GR_USER_COME,0);
	}

	//������ɵ�½��Ϣ
	m_TCPSocket.SendData(uSocketIndex,MDM_GR_LOGON,ASS_GR_SEND_FINISH,0,dwHandleID);

	//������Ϣ
	if (m_szMsgRoomLogon[0]!=0) SendRoomMessage(uSocketIndex,dwHandleID,m_szMsgRoomLogon);

	return true;
}

//������Ӵ���
bool CGameMainManage::OnCheckConnect(DataBaseResultLine * pResultData)
{
	if (m_bRun==true) SetTimer(IDT_CHECK_DATA_CONNECT,60000L);
	return true;
}

//�����½����
bool CGameMainManage::OnRoomLogonResult(DataBaseResultLine * pResultData)
{
	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	switch (pResultData->uHandleRusult)
	{
	case DTR_GR_CONTEST_NOSIGNUP:
	case DTR_GR_CONTEST_NOSTART:
	case DTR_GR_LOGON_SUCCESS:		//��½�ɹ�
		{
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_LogonResult)) return false;
			DL_GR_O_LogonResult * pLogonResult=(DL_GR_O_LogonResult *)pResultData;

			try
			{

				if (m_InitData.dwRoomRule & GRR_CONTEST)
				{
					if (pResultData->uHandleRusult == DTR_GR_CONTEST_NOSIGNUP)
					{
						bHandleCode=ERR_GR_CONTEST_NOSIGNUP;
					}
					else if (pResultData->uHandleRusult == DTR_GR_CONTEST_NOSTART)
					{
						bHandleCode=ERR_GR_CONTEST_NOTSTRAT;
					}
					else
					{
						if (pLogonResult->pUserInfoStruct.i64ContestScore < m_InitData.i64LowChip)
						{
							bHandleCode=ERR_GR_CONTEST_KICK;
						}
// 						CTime _t = CTime::GetCurrentTime();
// 						if (_t.GetTime() < m_InitData.i64TimeStart)
// 						{
// 							bHandleCode=ERR_GR_CONTEST_NOTSTRAT;
// 						}
// 						if (_t.GetTime() > m_InitData.i64TimeEnd)
// 						{
// 							bHandleCode=ERR_GR_CONTEST_OVER;
// 							break;
// 						}
					}
					
				}


				//�ж��Ƿ񻹴�������
				if (m_TCPSocket.IsConnectID(pResultData->uIndex,pResultData->dwHandleID)==false) throw DTR_GR_ERROR_UNKNOW;

				//���������û�
				CGameUserInfo * pGameUser=m_UserManage.FindOnLineUser(pLogonResult->pUserInfoStruct.dwUserID);
				if (pGameUser!=NULL)
				{
					bHandleCode=ERR_GR_ACCOUNTS_IN_USE;
					break;
				}

				//Ч���û���Ŀ
				if ((pLogonResult->pUserInfoStruct.bMember==0)&&((m_UserManage.GetGameRoomCount())>=m_UserManage.m_OnLineUserInfo.GetPermitCount()-m_UserManage.m_OnLineUserInfo.GetVipReseverCount()))
				{
					bHandleCode=ERR_GR_PEOPLE_FULL;
					break;
				}

				//�����û�����
				pGameUser=m_UserManage.ActiveUser(&pLogonResult->pUserInfoStruct,pResultData->uIndex,pResultData->dwHandleID,pLogonResult->dwGamePower,pLogonResult->dwMasterPower,0);
				if (pGameUser==NULL) throw DTR_GR_ERROR_UNKNOW;
				SetTimer(IDT_UPDATE_ROOM_PEOPLE,100);//20081201

				//�����û���Ϣ
				pGameUser->SetMD5Pass(pLogonResult->szMD5Pass);
				pGameUser->SetUserScrPoint(pLogonResult->pUserInfoStruct.dwPoint);

				//����Ȩ��
				if (IsLimitedTalk(pGameUser,true)==true) CUserPower::SetCanRoomTalk(pGameUser->m_dwGamePower,false);
				if (IsLimitedTalk(pGameUser,false)==true) CUserPower::SetCanGameTalk(pGameUser->m_dwGamePower,false);

				//wushuqun 2009.5.22
				//�޸Ľ�ֹ��¼���⣬�޸Ĳ���TWLoginRecord ��¼ʱ��
				::OutputDebugString("���� OnRoomLogonResult: DTR_GR_LOGON_SUCCESS");
				//���ͷ�����Ϣ

				//wushuqun 2009.6.6
				//���Ӽ�ʱ��ȡ����������Ҳ���
				pLogonResult->ResultHead.uHandleRusult = bHandleCode;
				if (SendRoomInfo(pGameUser,pResultData->uIndex,pResultData->dwHandleID,pLogonResult)==false)
				{
					throw DTR_GR_ERROR_UNKNOW;
				}

				//wushuqun 2009.5.22
				//�޸Ľ�ֹ��¼���⣬�޸Ĳ���TWLoginRecord ��¼ʱ��
				::OutputDebugString("���� OnRoomLogonResult: DTR_GR_LOGON_SUCCESS");
				DL_GR_I_WriteWLoginRecord DlWriteWLogin;
				DlWriteWLogin.dwUserID = pGameUser->m_UserData.dwUserID;
				DlWriteWLogin.uRoomID = m_InitData.uRoomID;
				m_SQLDataManage.PushLine(&DlWriteWLogin.DataBaseHead,sizeof(DlWriteWLogin),DTK_GR_WRITE_WLOGINRECORD,0,0);

				//////////////

			}
			catch (...)
			{
				TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
				///////////////////////////////
				//20090203 Kylin ��ֹ�������ݿ���û���½ʧ��
				if (pLogonResult->bRegOnline==TRUE)
				{
					DL_GR_I_UnRegisterUser UnRegisterUser;
					UnRegisterUser.dwUserID=pLogonResult->pUserInfoStruct.dwUserID;
					m_SQLDataManage.PushLine(&UnRegisterUser.DataBaseHead,sizeof(UnRegisterUser),DTK_GR_UNREGISTER_ONLINE,0,0);
				}
				///////////////////////////////
				return false;
			}

			return true;
		}
	case DTR_GR_IN_OTHER_ROOM:		//����������
		{
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_LogonError)) return false;
			DL_GR_O_LogonError * pLogonResult=(DL_GR_O_LogonError *)pResultData;

			//��������
			MSG_GR_R_OtherRoom MSGOtherRoom;
			ZeroMemory(&MSGOtherRoom, sizeof(MSG_GR_R_OtherRoom));
			if (pLogonResult != NULL)
			{
				MSGOtherRoom.iRoomID = pLogonResult->uRoomID;
				lstrcpy(MSGOtherRoom.szGameRoomName,pLogonResult->szGameRoomName);
			}
			m_TCPSocket.SendData(pResultData->uIndex,&MSGOtherRoom,sizeof(MSGOtherRoom),MDM_GR_LOGON,
				ASS_GR_LOGON_ERROR,ERR_GR_IN_OTHER_ROOM,pResultData->dwHandleID);

			return true;
		}
	case DTR_GR_USER_NO_FIND: {	bHandleCode=ERR_GR_USER_NO_FIND; break; }
	case DTR_GR_USER_PASS_ERROR: { bHandleCode=ERR_GR_USER_PASS_ERROR; break; }
	case DTR_GR_USER_VALIDATA: { bHandleCode=ERR_GR_STOP_LOGON; break; }
	case DTR_GR_USER_IP_LIMITED: { bHandleCode=ERR_GR_USER_IP_LIMITED; break; }
	case DTR_GR_IP_NO_ORDER: { bHandleCode=ERR_GR_IP_NO_ORDER; break; }
	case DTR_GR_ONLY_MEMBER: { bHandleCode=ERR_GR_ONLY_MEMBER; break; }
	case DTR_GR_MATCH_LOGON: { bHandleCode=ERR_GR_MATCH_LOGON; break; }	
	case DTR_GR_OVER_TIMER:{bHandleCode=ERR_GR_TIME_OVER;break;}//��Ϸʱ�䵽��
	//wushuqun 2009.6.5
	//���ڻ�ս���ʱ����
	case DTR_GR_BATTLEROOM_TIMEOUT:
		{
			bHandleCode=ERR_GR_BATTLEROOM_OUTTIME;
			break;
		}
// 	case DTR_GR_CONTEST_NOSIGNUP:
// 		{
// 			bHandleCode = ERR_GR_CONTEST_NOSIGNUP;
// 			break;
// 		}
	case DTR_GR_CONTEST_TIMEROUT:
		{
			bHandleCode = ERR_GR_CONTEST_TIMEOUT;
			break;
		}
// 	case DTR_GR_CONTEST_NOSTART:
// 		{
// 			bHandleCode = ERR_GR_CONTEST_NOTSTRAT;
// 			break;
// 		}
	case DTR_GR_CONTEST_OVER:
		{
			bHandleCode = ERR_GR_CONTEST_OVER;
			break;
		}

	case DTR_GR_CONTEST_BEGUN:
		{
			bHandleCode = ERR_GR_CONTEST_BEGUN;
			break;
		}
	}

	//Ĭ�ϴ���
	m_TCPSocket.SendData(pResultData->uIndex,MDM_GR_LOGON,ASS_GR_LOGON_ERROR,bHandleCode,pResultData->dwHandleID);

	return true;
}

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
bool CGameMainManage::OnGetNickNameOnIDResult(DataBaseResultLine * pResultData)
{
    BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	
	//add by lxl 2010-11-17 ��Ϸ�л�ȡ�ǳ����⴦��
	int nMainID = MDM_GR_ROOM;
	int nAssistantID = ASS_GR_GET_NICKNAME_ONID;
	if(pResultData->uHandleKind == DTK_GR_GETNICKNAME_ONID_INGAME)
	{
		nMainID = MDM_GR_ROOM2GAME;
		nAssistantID = ASS_GR_GET_NICKNAME_ONID_INGAME;
	}

    switch (pResultData->uHandleRusult)
    {
    case DTR_GR_GETNICKNAME_SUCCESS:
        {
            //Ч������
            if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_GetNickNameOnIDResult)) return false;
            DL_GR_O_GetNickNameOnIDResult* pGetNickNameOnID =(DL_GR_O_GetNickNameOnIDResult *)pResultData;

            //��������
            bHandleCode = ASS_GR_GETNICKNAME_SUCCESS;
            MSG_GR_S_GetNickNameOnID_t stGetNickNameOnID;
            stGetNickNameOnID.iUserID = pGetNickNameOnID->iUserID;
            lstrcpy(stGetNickNameOnID.szNickName, pGetNickNameOnID->szNickName);
            m_TCPSocket.SendData(pResultData->uIndex,&stGetNickNameOnID,sizeof(stGetNickNameOnID),
                            nMainID, nAssistantID,bHandleCode,pResultData->dwHandleID);

            return true;
        }
        break;
    case DTR_GR_GETNICKNAME_NOTEXIST:
        {
            bHandleCode=ASS_GR_GETNICKNAME_NOTEXIST;
        }
        break;
    case DTR_GR_GETNICKNAME_ISNULL:
        {
            bHandleCode=ASS_GR_GETNICKNAME_ISNULL;
        }
        break;
    default:
        {
            bHandleCode=ERR_GR_ERROR_UNKNOW;
        }
        break;
    }
    m_TCPSocket.SendData(pResultData->uIndex, nMainID, nAssistantID, 
                        bHandleCode, pResultData->dwHandleID);

    return true;
}

// PengJiLin, 2010-9-10, ���˿������߿�����
bool CGameMainManage::OnUseKickPropResult(DataBaseResultLine * pResultData, BOOL bIsKickUser)
{
    int iHandleIDNum = ASS_GM_USE_KICK_PROP;
    if(FALSE == bIsKickUser)
    {
        iHandleIDNum = ASS_GM_USE_ANTI_KICK_PROP;
    }

    BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
    switch (pResultData->uHandleRusult)
    {
    case DTR_GR_USE_KICKPROP_NULL:      // û�е���
        {
            bHandleCode = ERR_GR_KICK_PROP_NULL;
        }
        break;
    case DTR_GR_USE_KICKPROP_SET_ERROR: // ����ʱ�����
        {
            bHandleCode = ERR_GR_KICK_SET_TIME_FAIL;
        }
        break;
    case DTR_GR_USE_KICKPROP_SET_SUCCESS:   // ����ʱ��ɹ�
        {
            bHandleCode = ERR_GR_KICK_SET_TIME_SUCCESS;
        }
        break;
    }

    DL_GR_O_UseKickProp* pUseKickProp = (DL_GR_O_UseKickProp*)pResultData;
    MSG_GR_RS_KickProp KickProp;
    memset(&KickProp, 0, sizeof(KickProp));
    KickProp.dwUserID = pUseKickProp->dwUserID;
    KickProp.iPropID = pUseKickProp->iPropID;
    KickProp.iTotalTime = pUseKickProp->iTotalTime;

    m_TCPSocket.SendData(pResultData->uIndex, &KickProp, sizeof(KickProp), MDM_GM_GAME_FRAME, 
                         iHandleIDNum, bHandleCode, pResultData->dwHandleID);
    return true;
}

// PengJiLin, 2010-9-13, ʹ�����˿�����
bool CGameMainManage::OnNewKickUserPropResult(DataBaseResultLine * pResultData)
{
    DL_GR_O_UseNewKickUserProp* pNewKickProp = (DL_GR_O_UseNewKickUserProp*)pResultData;

    BYTE bHandleCode = ERR_GR_ERROR_UNKNOW;
    switch (pResultData->uHandleRusult)
    {
    case DTR_GR_NEW_KICKUSER_SUCCESS:
        bHandleCode = ERR_GR_NEW_KICKUSER_SUCCESS;
        break;
    case DTR_GR_NEW_KICKUSER_NOTIME:
        bHandleCode = ERR_GR_NEW_KICKUSER_NOTIME;
        break;
    case DTR_GR_NEW_KICKUSER_HAD_ANTI:
        bHandleCode = ERR_GR_NEW_KICKUSER_HAD_ANTI;
        break;
    case DTR_GR_NEW_KICKUSER_HAD_VIP:
        bHandleCode = ERR_GR_NEW_KICKUSER_HAD_VIP;
        break;
    }

    _TAG_NEW_KICKUSER stKick;
    stKick.dwUserID = pNewKickProp->dwUserID;
    stKick.dwDestID = pNewKickProp->dwDestID;

    // �����ɹ�
    if(ERR_GR_NEW_KICKUSER_SUCCESS == bHandleCode)
    {
        CGameUserInfo * pUserInfo = m_UserManage.GetOnLineUserInfo(pResultData->uIndex);
        if(pUserInfo == NULL) return true;
        CGameUserInfo * pDestUserInfo = m_UserManage.FindOnLineUser(pNewKickProp->dwDestID);
        if(pDestUserInfo == NULL) return true;
        if(pUserInfo->m_UserData.bDeskNO == 255)return true;

        CGameDesk *pDesk = GetDeskObject(pUserInfo->m_UserData.bDeskNO);
        if(pDesk == NULL) return true;

        if(pDesk->IsPlayingByGameStation()) //��Ϸ������
        {
            bHandleCode = ERR_GR_NEW_KICKUSER_PLAYING;
        }
        else
        {
            if (pDestUserInfo->m_UserData.bUserState==USER_WATCH_GAME) 
            { 
                pDesk->WatchUserLeftDesk(pDestUserInfo->m_UserData.bDeskStation,pDestUserInfo); 
            } 
            else  
            { 
                pDesk->UserLeftDesk(pDestUserInfo->m_UserData.bDeskStation,pDestUserInfo); 
            }
        } 
    }

    // ���Լ�����Ϣ
    m_TCPSocket.SendData(pResultData->uIndex, &stKick, sizeof(stKick), MDM_GR_PROP, ASS_PROP_NEW_KICKUSER, 
                         bHandleCode, pResultData->dwHandleID);

    // ��Ŀ�귢��Ϣ
    m_TCPSocket.SendData(pNewKickProp->dwDestIndex, &stKick, sizeof(stKick), MDM_GR_PROP, ASS_PROP_NEW_KICKUSER, 
                        bHandleCode, pNewKickProp->dwDestHandleID);

    return true;
}


// PengJiLin, 2010-9-13, ���˿����˹���
bool CGameMainManage::OnNewKickUserProp(NetMessageHead *pNetHead, void* pData, UINT uSize, 
                                        ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
    if(uSize != sizeof(_TAG_NEW_KICKUSER))return false;
    _TAG_NEW_KICKUSER* pNewKickUser = (_TAG_NEW_KICKUSER*)pData;

    CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(uIndex);
    if(pUserInfo == NULL) return false;
    CGameUserInfo* pDestUserInfo = m_UserManage.FindOnLineUser(pNewKickUser->dwDestID);
    if(NULL == pDestUserInfo) return false;

    DL_GR_I_NewKickUserProp KickProp;
    memset(&KickProp, 0, sizeof(KickProp));
    KickProp.dwUserID = pNewKickUser->dwUserID;
    KickProp.dwDestID = pNewKickUser->dwDestID;
    KickProp.dwDestIndex = pDestUserInfo->m_uSocketIndex;
    KickProp.dwDestHandleID = pDestUserInfo->m_dwHandleID;
    m_SQLDataManage.PushLine(&KickProp.DataBaseHead, sizeof(KickProp), DTK_GR_USER_NEW_KICK_USER_PROP, 
                              uIndex, dwHandleID);
    return true;
}

//Ǯ����
bool CGameMainManage::OnAboutMoneyResult(DataBaseResultLine * pResultData, UINT uCode)
{
	// ����ͬ��ָ��
	UINT uCmd = ASS_GR_OPEN_WALLET;
	if (DTK_GR_DEMAND_MONEY_IN_GAME==uCode)
	{
		uCmd = ASS_GR_OPEN_WALLET_INGAME;
	}
	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	switch (pResultData->uHandleRusult)
	{
	case DTR_GR_OPEN_WALLET_ERROR://��Ǯ�����
		{
			bHandleCode=ASS_GR_OPEN_WALLET_ERROR;
			m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_OPEN_WALLET_ERROR_PSW://��Ǯ������������
		{
			bHandleCode = ASS_GR_OPEN_WALLET_ERROR_PSW;
			m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}		
	case DTR_GR_OPEN_WALLET_SUC://��Ǯ��ɹ�
		{
			bHandleCode=ASS_GR_OPEN_WALLET_SUC;
			DL_GR_O_OpenWalletResult *pOpenWalletResult=(DL_GR_O_OpenWalletResult*) pResultData;
			MSG_GR_UserBank UserBank = pOpenWalletResult->stUserBank;

			//�����к�����ڴ��е�������ֵ
			//Added by JianGuankun 2012.1.10
			CGameUserInfo* pUserInfo = m_UserManage.FindOnLineUser(UserBank.dwUserID);
			if (pUserInfo)
			{
				pUserInfo->m_UserData.i64Bank = UserBank.i64Bank;
			}
			//End Add

// 			memset(&UserBank,0,sizeof(UserBank));
// 			UserBank.i64Bank=pOpenWalletResult->stUserBank.i64Bank;
			m_TCPSocket.SendData(pResultData->uIndex,&UserBank,sizeof(UserBank),MDM_GR_MONEY, uCmd, bHandleCode,pResultData->dwHandleID);
			break;
		}

	}

	return true;
}

//���䣭��Ǯ��
bool CGameMainManage::OnCheckInMoney(DataBaseResultLine * pResultData, UINT uCode)
{
	// ����ͬ��ָ��
	UINT uCmd = ASS_GR_CHECK_IN;
	if (DTK_GR_CHECKIN_MONEY_INGAME==uCode)
	{
		uCmd = ASS_GR_CHECK_IN_INGAME;
	}

	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	switch (pResultData->uHandleRusult)
	{
	case DTR_GR_ERROR_UNKNOW://
		{
			bHandleCode=ASS_GR_CHECKIN_ERROR;
			m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_CHECKIN_MONEY_INV:
		{
			bHandleCode=ASS_GR_CHECKIN_ERROR_PLAYING;
			m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_CHECKIN_MONEY_SUC://����ɹ�
		{
			bHandleCode=ASS_GR_CHECKIN_SUC;
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_CheckOutMoney)) 
			{
				bHandleCode=ASS_GR_CHECKIN_ERROR;
				m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
				return false;
			}
			DL_GR_O_CheckOutMoney * pCheckInResult=(DL_GR_O_CheckOutMoney *)pResultData;

			//�޸��û�����

			CGameUserInfo * pUserInfo=m_UserManage.FindOnLineUser(pCheckInResult->dwUserID);
			if (pUserInfo==NULL) return false;
			else
			{
				pUserInfo->m_UserData.i64Money-=pCheckInResult->i64CheckOut;
				pUserInfo->m_UserData.i64Bank+=pCheckInResult->i64CheckOut;//2008-06-24,Fred Huang
			}

			MSG_GR_CheckMoney CheckInMoney;
			memset(&CheckInMoney,0,sizeof(CheckInMoney));
			CheckInMoney.i64CheckOut=pCheckInResult->i64CheckOut;
			CheckInMoney.i64MoneyInBank=pCheckInResult->i64MoneyInBank;
			CheckInMoney.i64MoneyInRoom=pCheckInResult->i64MoneyInRoom;
			CheckInMoney.dwUserID=pCheckInResult->dwUserID;
			CheckInMoney.uGameID = m_KernelData.uNameID;
			m_TCPSocket.SendDataBatch(&CheckInMoney,sizeof(CheckInMoney),MDM_GR_MONEY,uCmd,bHandleCode);
			break;
		}
	}

	return true;
}

//���䣼��Ǯ��
bool CGameMainManage::OnCheckOutMoney(DataBaseResultLine * pResultData, UINT uCode)
{
	// ����ͬ��ָ��
	UINT uCmd = ASS_GR_CHECK_OUT;
	if (DTK_GR_CHECKOUT_MONEY_INGAME==uCode)
	{
		uCmd = ASS_GR_CHECK_OUT_INGAME;
	}

	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	switch (pResultData->uHandleRusult)
	{
	case DTR_GR_ERROR_UNKNOW://
		{
			bHandleCode=ASS_GR_CHECKOUT_ERROR;
			m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_CHECKOUT_MONEY_INV:
		{
			bHandleCode=ASS_GR_CHECKOUT_ERROR_PLAYING;
			m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_CHECKOUT_MONEY_SUC://ȡ���ɹ�
		{
			bHandleCode=ASS_GR_CHECKOUT_SUC;
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_CheckOutMoney)) 
			{
				bHandleCode=ASS_GR_CHECKOUT_ERROR;
				m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
				return false;
			}
			DL_GR_O_CheckOutMoney * pCheckOutResult=(DL_GR_O_CheckOutMoney *)pResultData;


			CGameUserInfo * pUserInfo=m_UserManage.FindOnLineUser(pCheckOutResult->dwUserID);
			if (pUserInfo==NULL) return false;
			else
			{
				pUserInfo->m_UserData.i64Money+=pCheckOutResult->i64CheckOut;
				pUserInfo->m_UserData.i64Bank-=pCheckOutResult->i64CheckOut;//2008-06-24,Fred Huang
			}

			MSG_GR_CheckMoney CheckOutMoney;
			memset(&CheckOutMoney,0,sizeof(CheckOutMoney));
			CheckOutMoney.i64CheckOut=pCheckOutResult->i64CheckOut;
			CheckOutMoney.i64MoneyInBank=pCheckOutResult->i64MoneyInBank;
			CheckOutMoney.i64MoneyInRoom=pCheckOutResult->i64MoneyInRoom;
			CheckOutMoney.dwUserID=pCheckOutResult->dwUserID;
			CheckOutMoney.uGameID = m_KernelData.uNameID;
			m_TCPSocket.SendDataBatch(&CheckOutMoney,sizeof(CheckOutMoney),MDM_GR_MONEY,uCmd,bHandleCode);
			break;
		}
	}

	return true;
}
//ת�ʼ�¼���
bool CGameMainManage::OnTransferRecord(DataBaseResultLine * pResultData, UINT uCode)
{
	UINT uCmd = ASS_GR_TRANSFER_RECORD;
	if (DTK_GR_TRANSFER_RECORD_INGAME == uCode)
	{
		uCmd = ASS_GR_TRANSFER_RECORD_INGAME;
	}
	if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_TransferRecord_t)) 
	{
		return false;
	}
	DL_GR_O_TransferRecord_t *pTransferRecordResult = (DL_GR_O_TransferRecord_t *)pResultData;
	MSG_GR_R_TransferRecord_t transferRecord;
	transferRecord.nSeriNo			= pTransferRecordResult->transferRecord.nSeriNo;
	transferRecord.dwUserID			= pTransferRecordResult->transferRecord.dwUserID;
	transferRecord.destUserID		= pTransferRecordResult->transferRecord.destUserID;
	transferRecord.i64Money			= pTransferRecordResult->transferRecord.i64Money;
	transferRecord.i64ActualTransfer	= pTransferRecordResult->transferRecord.i64ActualTransfer;
	transferRecord.oleDateTime		= pTransferRecordResult->transferRecord.oleDateTime;

	BYTE bHandleCode=ASS_GR_TRANSFER_SUC;

	m_TCPSocket.SendData(pResultData->uIndex, &transferRecord, sizeof(transferRecord), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);

	return true;
}

/// �޸�����
bool CGameMainManage::OnChangePasswd(DataBaseResultLine * pResultData, UINT uCode)
{
	// ����ͬ��ָ��
	UINT uCmd = ASS_GR_CHANGE_PASSWD;
	if (DTK_GR_CHANGE_PASSWD_INGAME==uCode)
	{
		uCmd = ASS_GR_CHANGE_PASSWD_INGAME;
	}
	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_ChangePasswd_t)) 
	{
		bHandleCode=ASS_GR_PASSWORD_ERROR;
		m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
		return true;
	}
	
	if (pResultData->uHandleRusult != DTR_GR_OPEN_WALLET_SUC)
	{
		bHandleCode=ASS_GR_PASSWORD_ERROR;
		m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
	}
	else
	{
		bHandleCode=ASS_GR_OPEN_WALLET_SUC;
		m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
	}
	
	return true;
}

/// ת��
bool CGameMainManage::OnTransferMoney(DataBaseResultLine * pResultData, UINT uCode)
{
	// ����ͬ��ָ��
	UINT uCmd = ASS_GR_TRANSFER_MONEY;
	if (DTK_GR_TRANSFER_MONEY_INGAME==uCode)
	{
		uCmd = ASS_GR_TRANSFER_MONEY_INGAME;
	}

	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	if (pResultData->LineHead.uSize!=sizeof(DL_GR_O_TransferMoney)) 
	{
		bHandleCode=ASS_GR_TRANSFER_ERROR;
		m_TCPSocket.SendData(pResultData->uIndex, MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
		return false;
	}
	DL_GR_O_TransferMoney * pTransferMoney=(DL_GR_O_TransferMoney *)pResultData;
	MSG_GR_S_TransferMoney TransferMoney;
	TransferMoney.destUserID = pTransferMoney->stTransfer.destUserID;
	TransferMoney.i64Money = pTransferMoney->stTransfer.i64Money;
	TransferMoney.UserID = pTransferMoney->stTransfer.UserID;
	TransferMoney.bUseDestID = pTransferMoney->stTransfer.bUseDestID;
	strcpy(TransferMoney.szNickName, pTransferMoney->stTransfer.szNickName);		//�����ǳ���Ϣ add by lxl 2010-11-10
	strcpy(TransferMoney.szDestNickName, pTransferMoney->stTransfer.szDestNickName);
	TransferMoney.i64ActualTransfer = pTransferMoney->stTransfer.i64ActualTransfer;
	switch (pResultData->uHandleRusult)
	{
	case DTR_GR_OPEN_WALLET_ERROR_PSW: //�������
		{
			bHandleCode=ASS_GR_PASSWORD_ERROR;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_TOTAL_NOT_ENOUGH:	///< ���д��̫�٣�������ת��
		{
			bHandleCode=ASS_GR_TRANSFER_TOTAL_LESS;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_TOO_LESS:			///<����ת�ʶ�̫С
		{
			bHandleCode=ASS_GR_TRANSFER_TOO_LESS;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_MULTIPLE:			///<����ת�ʶ������ĳ����������
		{
			bHandleCode=ASS_GR_TRANSFER_MULTIPLE;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_NO_ENOUGH_MONEY:	///<���д��㱾��ת��
		{
			bHandleCode=ASS_GR_TRANSFER_NOT_ENOUGH;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_TOO_MANY_TIME:  ///< ɢ���䵱��ת������ﵽһ���޶�
		{
			bHandleCode = ASS_GR_TRANSFER_TOO_MANY_TIME;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_MONEY_NO_DEST:		///< Ŀ�겻����
		{
			bHandleCode=ASS_GR_TRANSFER_NO_DEST;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_ERROR_UNKNOW://ת��ʧ��
		{
			bHandleCode=ASS_GR_TRANSFER_ERROR;
			m_TCPSocket.SendData(pResultData->uIndex, &TransferMoney, sizeof(TransferMoney), MDM_GR_MONEY, uCmd, bHandleCode, pResultData->dwHandleID);
			break;
		}
	case DTR_GR_TRANSFER_MONEY_SUC://ת�ʳɹ�
		{
			bHandleCode=ASS_GR_TRANSFER_SUC;
			//Ч������
			CGameUserInfo * pUserInfo=m_UserManage.FindOnLineUser(pTransferMoney->stTransfer.UserID);
			if (pUserInfo==NULL)
			{
				return false;
			}

			m_TCPSocket.SendData(pResultData->uIndex,&TransferMoney,sizeof(TransferMoney),MDM_GR_MONEY,uCmd,bHandleCode, pResultData->dwHandleID);
			///// Ҳ��ת��Ŀ���û�������Ϣ
			//CGameUserInfo *pDestUserInfo = m_UserManage.FindOnLineUser(pTransferMoney->stTransfer.destUserID);
			//if (pDestUserInfo !=NULL)
			//{
			//	m_TCPSocket.SendData(pDestUserInfo->m_uSocketIndex,&TransferMoney,sizeof(TransferMoney),MDM_GR_MONEY,uCmd,bHandleCode, pResultData->dwHandleID);
			//}
			break;
		}
	}

	return true;
}
bool CGameMainManage::SendUserMoenyByTimesResult(DataBaseResultLine * pResultData)
{

	//���ͽ�ҳɹ�
	if (pResultData->uHandleRusult == 0)
	{
		//BYTE bHandleCode = 0;
		//
		//SendUserMoneyByTimes senResult;
		//memset(&senResult,0,sizeof(senResult));
		////memcpy(&senResult,pResultData,sizeof(senResult));
		//DL_GR_O_SendMonet_By_Times* pSendMoeny = (DL_GR_O_SendMonet_By_Times*)pResultData;
		//senResult.dwAllSendTImes = pSendMoeny->dwAllSendTImes;
		//senResult.dwSendFailedCause = pSendMoeny->dwSendFailedCause;
		//senResult.dwSendGameMoney = pSendMoeny->dwSendGameMoney;
		//senResult.dwSendTimes = pSendMoeny->dwSendTimes;
		//senResult.dwSrcGameMoney = pSendMoeny->dwSrcGameMoney;
		//senResult.dwSrcMoneyInBank = pSendMoeny->dwSrcMoneyInBank;
		//senResult.dwUserID = pSendMoeny->dwUserID;
		//CGameUserInfo* pGameUser = m_UserManage.FindOnLineUser(senResult.dwUserID);
		//if (pGameUser != NULL)
		//{
		//	pGameUser->SetUserSendedMoeny(senResult.dwSendGameMoney);
		//	m_TCPSocket.SendDataBatch(&senResult,sizeof(senResult),MDM_GR_MONEY,ASS_GR_SENDMONEY_TIMES,bHandleCode);
		//}
		
		///< �Ѿ���void CGameDesk::PresentCoin()�з��͸��ͻ��ˡ�
	}
	return true;

}

// PengJiLin, 2011-4-15, ����ʱ���������ͽ��
bool CGameMainManage::OnSendMoneyOnTimesAndCounts(DataBaseResultLine * pResultData)
{
    SendMoneyOnTimesAndCounts sendResult;
    memset(&sendResult,0,sizeof(sendResult));

    DL_GR_O_SendMoney_On_TC* pSendMoeny = (DL_GR_O_SendMoney_On_TC*)pResultData;

    sendResult.dwUserID = pSendMoeny->dwUserID;
    sendResult.dwGetMoney = pSendMoeny->dwGetMoney;
    sendResult.dwMoneyOnTimes = pSendMoeny->dwMoneyOnTimes;
    sendResult.dwMoneyOnCounts = pSendMoeny->dwMoneyOnCounts;
    sendResult.dwTimesNeed = pSendMoeny->dwTimesNeed;
    sendResult.dwCountsNeed = pSendMoeny->dwCountsNeed;

    
    CGameUserInfo* pGameUser = m_UserManage.FindOnLineUser(sendResult.dwUserID);
    if (pGameUser != NULL)
    {
        pGameUser->SetUserSendedMoeny(sendResult.dwGetMoney);
        m_TCPSocket.SendDataBatch(&sendResult,sizeof(sendResult),MDM_GR_MONEY,ASS_GR_SENDMONEY_TIMES,0);
    }

    return true;
}

//JianGuankun,2012.7.16��ȡ������ǰ�������ɼ���
bool CGameMainManage::OnGetContestRankResult(DataBaseResultLine* pResultData)
{
	DL_GR_O_UserContestRank* pRank = (DL_GR_O_UserContestRank*)pResultData;

	CGameUserInfo* pUser = m_UserManage.FindOnLineUser(pRank->_data.dwUserID);
	if (!pUser)
	{
		return true;
	}

	pRank->_data.i64ContestScore = pUser->m_UserData.i64ContestScore;
	pRank->_data.iContestCount = pUser->m_UserData.iContestCount;
	pUser->m_UserData.iRankNum = pRank->_data.iRankNum;

	m_TCPSocket.SendData(pUser->m_uSocketIndex,&pRank->_data,sizeof(MSG_GR_ContestChange),MDM_GR_ROOM,ASS_GR_USER_CONTEST,0,0);

	return true;
}

//������ϷȨ�޽������
bool CGameMainManage::OnGamePowerSetResult(DataBaseResultLine * pResultData)
{
	BYTE bHandleCode=ERR_GR_ERROR_UNKNOW;
	switch(pResultData->uHandleRusult)
	{
	case DTK_GR_GAME_POWER_SUCCESS:		//���гɹ�
		{
			bHandleCode=ASS_GR_GAME_POWER_SET_SUCCESS;
			break;
		}
	case DTK_GR_GAME_POWER_RELEASE:   //������Ƴɹ�
		{
			bHandleCode=ASS_GR_GAME_POWER_SET_RELEASE;
			break;
		}
	case DTR_GR_ERROR_UNKNOW:
	case DTK_GR_GAME_POWER_FAIL:
		{
			bHandleCode=ASS_GR_GAME_POWER_SET_FAIL;
			break;
		}
	}
	m_TCPSocket.SendData(pResultData->uIndex,MDM_GR_MANAGE,ASS_GR_POWER_SET,bHandleCode,pResultData->dwHandleID);
	return true;
}

//���ѹ���
bool CGameMainManage::OnFriendManageDBOperateResult(DataBaseResultLine *pResultData)
{
	return true;
}
//��������
bool CGameMainManage::OnBufferIMMessageDBOperateResult(DataBaseResultLine *pResultData)
{
	return true;
}
//���й���Ա�������
bool CGameMainManage::OnCallGMResult(DataBaseResultLine * pResultData)
{

	switch(pResultData->uHandleRusult)
	{
	case DTK_GR_CALL_GM_SUCCESS:		//���гɹ�
		{
			m_TCPSocket.SendData(pResultData->uIndex,MDM_GR_MANAGE,ASS_GR_CALL_GM_SUCCESS,0,pResultData->dwHandleID);
			break;
		}
	case DTK_GR_CALL_GM_IN_ONE_MI:		//�����ڣ������ڳ�������
		{
			m_TCPSocket.SendData(pResultData->uIndex,MDM_GR_MANAGE,ASS_GR_CALL_GM_FAIL,1,pResultData->dwHandleID);
			break;
		}
	case DTR_GR_ERROR_UNKNOW:
	case DTK_GR_CALL_GM_FAIL:
		{

			m_TCPSocket.SendData(pResultData->uIndex,MDM_GR_MANAGE,ASS_GR_CALL_GM_FAIL,0,pResultData->dwHandleID);
			break;
		}
	}
	return true;
}
//��ʱ����Ϣ
bool CGameMainManage::OnTimerMessage(UINT uTimerID)
{
	//��Ϸ��ʱ��
	if (uTimerID>=TIME_START_ID)
	{
		BYTE bDeskIndex=(BYTE)((uTimerID-TIME_START_ID)/TIME_SPACE);
		if (bDeskIndex<m_InitData.uDeskCount) 
		{
			return (*(m_pDesk+bDeskIndex))->OnTimer((uTimerID-TIME_START_ID)%TIME_SPACE);
		}
	}

	//�ڲ���ʱ��
	switch (uTimerID)
	{
	case IDT_WRITE_SERVER_INFO:		//���·�������Ϣ
		{
			DebugPrintf1(m_InitData.uRoomID, "OnTimerMessage��IDT_WRITE_SERVER_INFO uNameID =%d uRoomID = %d uOnLineUserCount = %d SQLQueueCount = %d", m_KernelData.uNameID, m_InitData.uRoomID, m_UserManage.GetGameRoomCount(), m_SQLDataManage.m_DataLine.GetDataCount());
			//��������
			DL_GR_I_UpdateServerInfo UpdateInfo;
			UpdateInfo.uRoomID=m_InitData.uRoomID;
			UpdateInfo.uNameID=m_KernelData.uNameID;
			UpdateInfo.uOnLineUserCount=m_UserManage.GetGameRoomCount();
			m_SQLDataManage.PushLine(&UpdateInfo.DataBaseHead,sizeof(UpdateInfo),DTK_GR_UPDATE_INFO,0,0);

			return true;
		}
	case IDT_CHECK_DATA_CONNECT:	//������ݿ�����
		{
			KillTimer(IDT_CHECK_DATA_CONNECT);
			DataBaseLineHead DataBaseHead;
			m_SQLDataManage.PushLine(&DataBaseHead,sizeof(DataBaseHead),DTK_GR_CHECK_CONNECT,0,0);

			return true;
		}
	case IDT_UPDATE_ROOM_PEOPLE:		//���·�������Ϣ
		{
			//��������
			KillTimer(IDT_UPDATE_ROOM_PEOPLE);
			DL_GR_I_UpdateServerInfo UpdateInfo;
			UpdateInfo.uRoomID=m_InitData.uRoomID;
			UpdateInfo.uNameID=m_KernelData.uNameID;
			UpdateInfo.uOnLineUserCount=m_UserManage.GetGameRoomCount();
			m_SQLDataManage.PushLine(&UpdateInfo.DataBaseHead,sizeof(UpdateInfo),DTK_GR_UPDATE_INFO,0,0);

			return true;
		}
	case IDT_QUEUE_GAME: /// �Ŷӻ���ʱ��
		{
			KillTimer(IDT_QUEUE_GAME);
			//queuerGame();
			NewQueueGame();
			SetTimer(IDT_QUEUE_GAME, m_iQueueTime * 1000);
			return true;
		}
	case IDT_CONTEST_GAMEOVER:
		{
			KillTimer(IDT_CONTEST_GAMEOVER);
			if (IsContestGameOver())
			{
				for (int i = 0; i < m_UserManage.m_OnLineUserInfo.GetPermitCount(); ++i)
				{
					CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(i);
					if (pUserInfo!=NULL && !IsUserPlayGameByStation(pUserInfo)) 
					{
						AutoAllotDeskDeleteUser(pUserInfo);
						m_pGameUserInfoList.remove(pUserInfo);
						m_TCPSocket.SendData(pUserInfo->m_uSocketIndex, MDM_GR_USER_ACTION, ASS_GR_CONTEST_GAMEOVER, 0, 0);

						//m_TCPSocket.CloseSocket(pUserInfo->m_uSocketIndex, 0);
						CleanUserInfo(pUserInfo);
						m_UserManage.FreeUser(pUserInfo, true);
					}
				}
			}
			SetTimer(IDT_CONTEST_GAMEOVER, 10000);
			return true;
		}
	case IDT_LEASE_TIMEOVER:
		{
			KillTimer(IDT_LEASE_TIMEOVER);
			if (IsLeaseTimeOver())
			{
				for (int i = 0; i < m_UserManage.m_OnLineUserInfo.GetPermitCount(); ++i)
				{
					CGameUserInfo * pUserInfo=m_UserManage.GetOnLineUserInfo(i);
					if (pUserInfo!=NULL && !IsUserPlayGameByStation(pUserInfo)) 
					{
						AutoAllotDeskDeleteUser(pUserInfo);
						m_pGameUserInfoList.remove(pUserInfo);
						m_TCPSocket.SendData(pUserInfo->m_uSocketIndex, MDM_GR_USER_ACTION, ASS_GR_LEASE_TIMEOVER, 0, 0);

						//m_TCPSocket.CloseSocket(pUserInfo->m_uSocketIndex, 0);
						CleanUserInfo(pUserInfo);
						m_UserManage.FreeUser(pUserInfo, true);
					}
				}
			}
			SetTimer(IDT_LEASE_TIMEOVER, 10000);
			return true;
		}
	}
	return false;
}

///�ж��Ƿ����Ŷӻ�����
///@return true:���Ŷӻ����䣻false�������Ŷӻ�����
bool CGameMainManage::IsQueueGameRoom()
{
	//return(((m_InitData.dwRoomRule & GRR_QUEUE_GAME) > 0) ? true : false);
	return m_InitData.dwRoomRule & GRR_QUEUE_GAME || m_InitData.dwRoomRule & GRR_CONTEST;
}

//��ʼ����Ϸ��
bool CGameMainManage::InitGameDesk(UINT uDeskCount, UINT uDeskType)
{
	//������Ϸ����
	UINT uDeskClassSize=0;
	m_pDeskArray=CreateDeskObject(uDeskCount,uDeskClassSize);
	if ((m_pDeskArray==NULL)||(uDeskClassSize==0))
		throw new CAFCException(TEXT("CGameMainManage::InitGameDesk �ڴ�����ʧ��"),0x418);

	//�����ڴ�
	m_uDeskCount=uDeskCount;
	m_pDesk=new CGameDesk * [m_uDeskCount];
	if (m_pDesk==NULL)
	{
		throw new CAFCException(TEXT("CGameMainManage::InitGameDesk �ڴ�����ʧ��"),0x419);
	}

    // PengJiLin, 2010-5-20, �Ŷӻ��������ó������ﵽʱ�ſ�ʼ��������
    // С�����ӵ�������ʹ�����ӵ�����������ʹ�����õ�����
    CBcfFile fConf(CBcfFile::GetAppPath()+"Function.bcf");
    m_uQueuerMaxPeople = fConf.GetKeyVal("OnlineCoin","QueuerMaxPeople",0);

	//{add rongqiufen 2010.06.29
	///���ŶӶ���������>=m_uQueuerPeople + ÿ����Ϸ��ʱ��ʼ�Ŷ�
	m_uQueuerPeople = m_uQueuerMaxPeople;
	//add rongqiufen 2010.06.29}
	

    if(m_uQueuerMaxPeople < m_KernelData.uDeskPeople) m_uQueuerMaxPeople = m_KernelData.uDeskPeople;
    srand((unsigned)time(NULL));

	//����ָ��
	for (UINT i=0;i<m_uDeskCount;i++)
	{
		*(m_pDesk+i)=(CGameDesk *)((char *)m_pDeskArray+i*uDeskClassSize);
		(*(m_pDesk+i))->Init(i,m_KernelData.uDeskPeople,this,uDeskType);//�ټ���
	}

	return true;
}


/// �����Զ���������
IAutoAllotDesk *CGameMainManage::CreateAutoAllotDesk(int iDeskPeopleCount, int iDeskCount)
{
	return new CAutoAllotDesk(iDeskPeopleCount, iDeskCount, m_KernelData.uMinDeskPeople);
}

///�Ŷӻ�����û�
int CGameMainManage::AutoAllotDeskAddUser(CGameUserInfo *pInfo)
{
	if (m_pIAutoAllotDesk)
	{
		m_pIAutoAllotDesk->Add(pInfo);	///< �����Ŷӵ����
	}
	return 0;
}

///�Ŷӻ�ɾ���û�
int CGameMainManage::AutoAllotDeskDeleteUser(CGameUserInfo *pInfo)
{
	if (m_pIAutoAllotDesk)
	{
		m_pIAutoAllotDesk->Delete(pInfo);
	}
	return 0;
}

void CGameMainManage::NewQueueGame()
{
	if (NULL == m_pIAutoAllotDesk)
	{
		return;
	}
	int nCount = 0;
	int *pArrDesk = NULL;
	m_pIAutoAllotDesk->AllotDesk(nCount, pArrDesk);
	if (nCount <= 0 || NULL == pArrDesk)
	{
		//ErrorPrintf("�������Ʋ�Ӧ���ؿ�ָ��--[%d]", __LINE__);
		return;
	}
	/// Ŀ������
	int nDeskNo=0;
	int *pQueueUserSit = (int*)m_pQueueUserSitBuffer;
	CGameUserInfo *pUserInfo = NULL;
	int nMaxQueue = nCount*m_KernelData.uDeskPeople;
	CGameDesk* pGameDesk = NULL;
	for (int i=0; i<nCount; ++i)
	{
		/// �ҿ���
		while(nDeskNo < m_InitData.uDeskCount)
		{
			pGameDesk=*(m_pDesk+nDeskNo);
			if (pGameDesk != NULL && pGameDesk->GetDeskPlayerNum() ==0) break;
			nDeskNo++;
		}
			/// û�ҵ����˳�ѭ��
		if (((UINT)nDeskNo>=m_InitData.uDeskCount)||(pGameDesk->GetDeskPlayerNum() != 0))
		{
			int nPos = i*m_KernelData.uDeskPeople;
			while (nPos<nMaxQueue)
			{
				pUserInfo = m_UserManage.FindOnLineUser(pArrDesk[nPos++]);
				m_pIAutoAllotDesk->Add(pUserInfo);
			}
			break;
		}
		int bDeskStation=0;
		int iusercount = 0;
		pQueueUserSit[0] = nDeskNo;
		vector<CGameUserInfo*> pDeskUserArr;
		pDeskUserArr.clear();
		/// ÿ���������
		for (bDeskStation=0; bDeskStation<m_KernelData.uDeskPeople; ++bDeskStation)
		{
			pQueueUserSit[1+bDeskStation] = pArrDesk[i*m_KernelData.uDeskPeople + bDeskStation];
			//m_pDeskInfo[nDeskNo]._ppUserInfo[bDeskStation] = NULL;
			if (pQueueUserSit[1+bDeskStation] == 0) continue;
			
			pUserInfo = m_UserManage.FindOnLineUser(pQueueUserSit[1+bDeskStation]);
			if (NULL == pUserInfo) continue;
			if ((pUserInfo->m_UserData.bDeskNO < m_InitData.uDeskCount) || (pUserInfo->m_UserData.bDeskStation < m_KernelData.uDeskPeople))
			{
				// ����Ѿ������ˣ��������ڶ���
				break;
			}
			else
			{
				//m_pDeskInfo[nDeskNo]._ppUserInfo[bDeskStation] = pUserInfo;
				pDeskUserArr.push_back(pUserInfo);
			}
			iusercount++;
		}

		/// �ж����������Ƿ�ʧ��
		if (pDeskUserArr.size() <= m_KernelData.uDeskPeople && pDeskUserArr.size() >= m_KernelData.uMinDeskPeople)
		{
			
			for (bDeskStation=0; bDeskStation<pDeskUserArr.size(); ++bDeskStation)
			{
				pUserInfo = pDeskUserArr[bDeskStation];
				if (NULL != pUserInfo)
				{
					pUserInfo->m_UserData.bDeskNO		= nDeskNo;
					pUserInfo->m_UserData.bDeskStation	= bDeskStation;
					pUserInfo->m_UserData.bUserState = USER_ARGEE; 
					pGameDesk->SetDeskUserInfo(pUserInfo, bDeskStation);
					m_pIAutoAllotDesk->Delete(pUserInfo);
				}
			}
			m_TCPSocket.SendDataBatch(m_pQueueUserSitBuffer, (m_KernelData.uDeskPeople+1)*sizeof(int),MDM_GR_USER_ACTION,ASS_GR_QUEUE_USER_SIT,ERR_GR_SIT_SUCCESS);
			//m_TCPSocket.SendDataBatch(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_QUEUE_USER_SIT,ERR_GR_SIT_SUCCESS);

			//		MSG_GR_R_UserSit UserSit;
			//		memset(&UserSit, 0, sizeof(UserSit));
			//		UserSit.dwUserID = pUserInfo->m_UserData.dwUserID;
			//		UserSit.bDeskIndex = pUserInfo->m_UserData.bDeskNO;
			//		UserSit.bDeskStation = pUserInfo->m_UserData.bDeskStation;
			//		UserSit.bUserState = USER_SITTING;
			//		m_TCPSocket.SendDataBatch(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_QUEUE_USER_SIT,ERR_GR_SIT_SUCCESS);
			//	}
			//}
		}
		else
		{
			/// ���ǿ���ҷŻش���ģ��
			for (bDeskStation=0; bDeskStation<m_KernelData.uDeskPeople; ++bDeskStation)
			{
				pUserInfo = pDeskUserArr[bDeskStation];
				if (NULL != pUserInfo)
				{
					pUserInfo->m_UserData.bDeskNO = 255;
					pUserInfo->m_UserData.bDeskStation = 255;
					m_pIAutoAllotDesk->Add(pUserInfo);
				}
			}
		}

	}
	return;
}

/*******************************************************************************************************/

//���ݿ⴦��ӿ�
UINT CGameDataBaseHandle::HandleDataBase(DataBaseLineHead * pSourceData)
{
	switch (pSourceData->uHandleKind)
	{
	case DTK_GR_DEMAND_MONEY:								//��Ǯ��
	case DTK_GR_DEMAND_MONEY_IN_GAME:						//����Ϸ�����д�Ǯ��
		{
			return OnOpenWallet(pSourceData, pSourceData->uHandleKind);				
		}
	case DTK_GR_CHECKOUT_MONEY:								//ȡ�����
	case DTK_GR_CHECKOUT_MONEY_INGAME:						//����Ϸ������ȡ�����
		{	
			return OnCheckOutMoney(pSourceData, pSourceData->uHandleKind);
		}
	case DTK_GR_CHECKIN_MONEY:								//������
	case DTK_GR_CHECKIN_MONEY_INGAME:						//����Ϸ�����д�����
		{
			return OnCheckInMoney(pSourceData, pSourceData->uHandleKind);
		}
	case DTK_GR_TRANSFER_MONEY:								//ת��
	case DTK_GR_TRANSFER_MONEY_INGAME:						//��Ϸ��ת��
		{
			return OnTransferMoney(pSourceData, pSourceData->uHandleKind);
		}
	case DTK_GR_CHANGE_PASSWD:								///< �޸�����
	case DTK_GR_CHANGE_PASSWD_INGAME:						///< �޸�����
		{
			return OnChangePasswd(pSourceData, pSourceData->uHandleKind);
		}
	case DTK_GR_TRANSFER_RECORD:							///< ת�ʼ�¼
	case DTK_GR_TRANSFER_RECORD_INGAME:						///< ת�ʼ�¼
		{
			return OnTransferRecord(pSourceData, pSourceData->uHandleKind);
		}

	case DTK_GR_UPDATE_INSTANT_MONEY:						//��ʱ���½�һ���
		{
			return OnModifyUserMoney(pSourceData);
		}
	case DTK_GR_UPDATE_CHARM:								//��������ֵ
		{
			return OnUpdateUserCharm(pSourceData);
		}

		//wushuqun 2009.5.6
		//����ZXJ �ķ���
	case DTK_GR_CLEAR_ONLINE_FLAG:	//2009-4-17���
		{
			return OnClearNetCutUserOnlineFlag(pSourceData);
		}
	case DTK_GR_USER_RECOME:	//�û�����������дW��¼ 2009-4-28
		{
			return OnReWriteWLoginRecord(pSourceData);
		}
	case DTK_GR_USER_CONTEST_LEFT:
		{
			return OnContestUserLeft(pSourceData);
		}

		//case DTK_GR_GET_BUY_ITEM_DATA:							//��ȡ��ҿɹ�������б�
		//	{
		//		return OnGetBuyItemData(pSourceData);			
		//	}
		//case DTK_GR_BUY_ITEM:										//��ҹ������
		//	{
		//		return OnBuyItem(pSourceData);
		//	}
		//case DTK_GR_READ_USER_HOLD_ITEM:						//������ҳ��е���
		//	{
		//		return OnGetUserHoldPlatformItemCount(pSourceData);
		//	}
	case DTK_GR_PROP_USE:// DTK_GR_USER_USE_ONE_ITEM:							//���ʹ�õ���
		{
			return OnUserUseOneProp(pSourceData);
		}
	case DTK_GPR_PROP_SMALL_BRD:
		{
			return OnUseSmallBoardcast(pSourceData);
		}

	case DTK_GR_LOGON_BY_ID:								//ͨ���û� ID ��½
		{
			return OnRoomLogon(pSourceData);
		}
	case DTK_GR_USER_LEFT:									//�û��뿪����
		{
			return OnUserLeft(pSourceData);
		}
	case DTK_GR_UPDATE_INFO:								//���·�������Ϣ
		{
			return OnUpdateServerInfo(pSourceData);
		}
	case DTK_GR_RECORD_GAME:		//��¼��Ϸ����
		{
			return OnRecordGameInfo(pSourceData);
		}
	case DTK_GR_CHECK_CONNECT:		//������ݿ�����
		{
			return OnCheckDataConnect(pSourceData);
		}
	case DTK_GR_UPDATE_GAME_RESULT:
		{
			return OnUpdateUserResult(pSourceData);
		}
	case DTK_GR_UPDATE_GAME_RESULT_ALLUSER:
		{
			return OnUpdateAllUserResult(pSourceData);
		}
	case DTK_GR_UPDATE_CONTEST_RESULT:
		{
			return OnUpdateContestInfo(pSourceData);
		}
	case DTK_GR_GET_CONTEST_RESULT:
		{
			return OnGetContestInfo(pSourceData);
		}
	case DTK_GR_CALL_GM:			//����GM
		{
			return OnCallGM(pSourceData);
		}
	case DTK_GR_GAME_POWER:			//����Ȩ���޸�
		{
			return OnGamePower(pSourceData);
		}
	case DTK_GR_GM_KICK_USER_RECORD:			//����Ա������Ϊ��¼
		{
			return OnGMKickUserRecord(pSourceData);
		}
	case DTK_GR_GM_WARNING_USER_RECORD:	//����Ա��������Ϣ��Ϊ��¼
		{
			return OnGMWarningUserRecord(pSourceData);
		}
	case DTK_GR_SENDMONEY_TIMES:
		{
			return OnSendUserMoenyByTimes(pSourceData);
		}
	case DTK_GR_UNREGISTER_ONLINE:	//�����û�����
		{
			return OnUnRegisterOnLine(pSourceData);
		}
	//wushuqun 2009.5.22
	//�޸Ľ�ֹ��¼���⣬�޸Ĳ���TWLoginRecord ��¼ʱ��
	case DTK_GR_WRITE_WLOGINRECORD:
		{
			return OnWriteTWLoginRecord(pSourceData);
		}
	//wushuqun 2009.6.6
	//��ʱ��������
	case DTR_GR_ALONEDESK_INTIME:
		{
			return OnAloneDeskInTime(pSourceData);
		}
	case DTK_GR_ROOM_PW_CHANGE:
		{
			return OnRoomPWChangeRecord(pSourceData);
		}
		//case DTK_GR_USER_OPT://��ҽ��к���,���˲���
		//	{
		//		return UserOpetation(pSourceData);
		//	}
    case DTK_GR_GETNICKNAME_ONID: // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
	case DTK_GR_GETNICKNAME_ONID_INGAME:
        {
            return OnGetNickNameOnID(pSourceData);
        }
    case DTK_GR_USE_KICK_PROP:      // PengJiLin, 2010-9-10, ʹ�����˿�����
        {
            return OnUseKickProp(pSourceData, TRUE);
        }
    case DTK_GR_USE_ANTI_KICK_PROP: // PengJiLin, 2010-9-10, ʹ�÷��߿�����
        {
            return OnUseKickProp(pSourceData, FALSE);
        }
    case DTK_GR_USER_NEW_KICK_USER_PROP:    // PengJiLin, 2010-9-13, ʹ�����˿����˹���
        {
            return OnNewKickUserProp(pSourceData);
        }
	case DTK_GR_BANK_NORMAL:
		{
			return OnBankOperateNormal(pSourceData);
		}
	case DTK_GR_BANK_TRANSFER:
		{
			return OnBankOperateTransfer(pSourceData);
		}
	case DTK_GR_BANK_DEPOSIT:
		{
			return OnBankOperateDeposit(pSourceData);
		}
	case DTK_GR_BANK_WALLET:
		{
			return OnBankGetWallet(pSourceData);
		}
        break;
	case DTK_GR_BANK_QUERY_DEPOSIT:
		{
			return OnBankQueryDeposits(pSourceData);
		}
		break;
	}
	return 0;
}



//������ݿ�����
UINT CGameDataBaseHandle::OnCheckDataConnect(DataBaseLineHead * pSourceData)
{
	try 
	{ 
		//�������
		m_pDataBaseManage->CheckSQLConnect();

		//������
		DataBaseResultLine ResultHead;
		m_pRusultService->OnDataBaseResultEvent(&ResultHead,DTK_GR_CHECK_CONNECT,0,sizeof(ResultHead),0,0);
	} 
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}

	return 0;
}

//��¼��Ϸ��Ϣ
UINT CGameDataBaseHandle::OnRecordGameInfo(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_GameRecord)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_GameRecord * pGameRecord=(DL_GR_I_GameRecord *)pSourceData;

	// duanxiaohui 20111103 ���鳡(��ѳ�)��������Ϸ��¼
	if (m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM || m_pInitInfo->dwRoomRule & GRR_CONTEST)
	{
		return 0;
	}
		long int dwNowTime=(long int)time(NULL);
		//�������ݿ�
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		//wushuqun 2009.6.5
		//�ı�ô洢����Ϊ�з���ֵ
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_RecordGameInfo_new",true);

		if(hRecord<(HANDLE)0x1000)
			return 0;

		//�������ݿ�

		sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTime"),dwNowTime-pGameRecord->dwBeginTime );
		sqlSPAddPara(hDatabase,hRecord,TEXT("@RoomID"),pGameRecord->uRoomID );
		sqlSPAddPara(hDatabase,hRecord,TEXT("@DeskIndex"),pGameRecord->bDeskIndex );
		sqlSPAddPara(hDatabase,hRecord,TEXT("@Tax"),pGameRecord->dwTax );
		//TCHAR szParam[4096];
		CString strParam = "";
		TCHAR szTmp[128];
		int nUserCount = 0;
		for (int i=0; i<MAX_PEOPLE; ++i)
		{
			if (pGameRecord->dwUserID[i]==0)
			{
				continue;
			}
			wsprintf(szTmp, TEXT("%d,%I64d,%I64d,%I64d,%I64d,%I64d,")
				, pGameRecord->dwUserID[i]
				, pGameRecord->dwScrPoint[i]
				, pGameRecord->dwChangePoint[i]
				, pGameRecord->i64ScrMoney[i]
				, pGameRecord->dwChangeMoney[i]
				, pGameRecord->dwTaxCom[i]
			);
			//_tcscat(szParam, szTmp);
			strParam += szTmp;
			++nUserCount;
		}
		sqlSPAddPara(hDatabase,hRecord,TEXT("@UserCount"), nUserCount );
		bool bRet = sqlSPAddPara(hDatabase, hRecord, TEXT("@Param"), strParam.GetBuffer(), sizeof(TCHAR) * strParam.GetLength());

		sqlSPExec(hDatabase,hRecord);

		//wushuqun 2009.6.5
		//����ս���ʱ���Ƿ���Ч
		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		//Ϊ0ʱ���ô��������ڸû�ս����
		// =1 ʱ,��ʾ�û�ս�����Ѿ����ڻʱ������
		if (ret == 1)
		{
			int iHandleResult = DTR_GR_BATTLEROOM_TIMEOUT_ERROR;
			DL_GR_O_BattleRecordResult BattleRecordRet;
			memset(& BattleRecordRet,0,sizeof(BattleRecordRet));
			BattleRecordRet.uRoomID = pGameRecord->uRoomID;
			BattleRecordRet.uRecordResult = DTR_GR_BATTLEROOM_TIMEOUT_ERROR;

			m_pRusultService->OnDataBaseResultEvent(&BattleRecordRet.ResultHead,DTK_GR_BATTLEROOM_RES,iHandleResult,
				sizeof(BattleRecordRet),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlCloseRecord(hDatabase,hRecord);
			return 0;
		}
		////////////////////////////
		sqlCloseRecord(hDatabase,hRecord);
		
		return 0;

	return 0;
}

//2009-4-17��ӣ�������������û����߱�־
UINT CGameDataBaseHandle::OnClearNetCutUserOnlineFlag(DataBaseLineHead * pSourceData)
{
	DL_GR_I_ClearOnlineFlag *pClearOnlineFlag = (DL_GR_I_ClearOnlineFlag*)pSourceData;
	if(pClearOnlineFlag == NULL )
		return -1;

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_DeleteAbnormalFlag");
	if(hRecord<(HANDLE)0x1000)
		return 0;
	sqlSPAddPara(hDatabase,hRecord,"@UserID", pClearOnlineFlag->lUserID);
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);
	return 0;
}

//2009-4-28 ��� zxj
UINT CGameDataBaseHandle::OnReWriteWLoginRecord(DataBaseLineHead * pSourceData)
{
	WriteStr("�û�����������дW��¼");
	DL_GR_I_UserRecome * pUserRecome = (DL_GR_I_UserRecome*)pSourceData;
	if( pSourceData == NULL )
		return -1;
	/*
	TCHAR szSQL[128];
	memset(szSQL, 0, sizeof(TCHAR));
	wsprintf(szSQL, TEXT("INSERT INTO [TWLoginRecord] ([UserID],[WID]) VALUES (%d,%d)"), pUserRecome->lUserID, m_pInitInfo->uRoomID);
	m_pDataBaseManage->m_DataBaseCenter.Execute(szSQL);
	*/

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase, "SP_InsertAbnormalOffline");
	if(hRecord<(HANDLE)0x1000)
		return 0;
	sqlSPAddPara(hDatabase,hRecord,"@UserID", pUserRecome->lUserID);
	sqlSPAddPara(hDatabase,hRecord,"@RoomID", m_pInitInfo->uRoomID);
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);
	return 0;
}

UINT CGameDataBaseHandle::OnContestUserLeft(DataBaseLineHead * pSourceData)
{
	DL_GR_I_ContestUserLeft * _p = (DL_GR_I_ContestUserLeft*)pSourceData;
	if(_p == NULL )
		return -1;
	/*
	TCHAR szSQL[128];
	memset(szSQL, 0, sizeof(TCHAR));
	wsprintf(szSQL, TEXT("INSERT INTO [TWLoginRecord] ([UserID],[WID]) VALUES (%d,%d)"), pUserRecome->lUserID, m_pInitInfo->uRoomID);
	m_pDataBaseManage->m_DataBaseCenter.Execute(szSQL);
	*/
	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetName(hDatabase, "SP_ContestUserLeft");
	if(hRecord<(HANDLE)0x1000)
		return 0;
	sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->iUserID);
	sqlSPAddPara(hDatabase,hRecord,"@ContestID", _p->iContestID);
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);
	return 0;
}


//���·�������Ϣ
UINT CGameDataBaseHandle::OnUpdateServerInfo(DataBaseLineHead * pSourceData)
{


	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UpdateServerInfo)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_UpdateServerInfo * pUpdateInfo=(DL_GR_I_UpdateServerInfo *)pSourceData;

	DebugPrintf1(pUpdateInfo->uRoomID, "OnUpdateServerInfo uRoomID = %d uOnLineUserCount = %d", pUpdateInfo->uRoomID, pUpdateInfo->uOnLineUserCount);


	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_UpdateRoomOnlinePeople");
	if(hRecord<(HANDLE)0x1000)
	{
		if (!sqlExec(hDatabase,TEXT("select top 1 * from TGameServerInfo")))
		{
			DebugPrintf1(pUpdateInfo->uRoomID, "OnUpdateServerInfo hDatabase Error");

		}
		DebugPrintf1(pUpdateInfo->uRoomID, "OnUpdateServerInfo hRecord Error");
		return 0;
	}

	pUpdateInfo->uOnLineUserCount=pUpdateInfo->uOnLineUserCount;
	pUpdateInfo->uOnLineUserCount=__min(pUpdateInfo->uOnLineUserCount,m_pInitInfo->uMaxPeople);

	sqlSPAddPara(hDatabase,hRecord,"@OnlineCount",pUpdateInfo->uOnLineUserCount);
	sqlSPAddPara(hDatabase,hRecord,"@RoomID",pUpdateInfo->uRoomID);
	sqlSPExec(hDatabase,hRecord);

	DebugPrintf1(pUpdateInfo->uRoomID, "OnUpdateServerInfo Over");

	return 0;
}


UINT CGameDataBaseHandle::OnRoomLogon(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_LogonByIDStruct)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_LogonByIDStruct * pLogonInfo=(DL_GR_I_LogonByIDStruct *)pSourceData;

	//��ʼ����½���
	DL_GR_O_LogonResult UserLogonResult;
	memset(&UserLogonResult,0,sizeof(UserLogonResult));

	//�������
	BYTE bRegisterLogon=FALSE;

	int iHandleResult=DTR_GR_LOGON_SUCCESS;

	//IP Ч�飨ÿ�������IP���ƣ�

	TCHAR szIP[16];
	CTCPSocketManage::ULongToIP(pLogonInfo->uAccessIP,szIP);

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_UserLoginRoom",true);
	if(hRecord<(HANDLE)0x1000)
	{
		iHandleResult=DTR_GR_DATABASE_CUT;
		m_pRusultService->OnDataBaseResultEvent(&UserLogonResult.ResultHead,DTK_GR_LOGON_BY_ID,iHandleResult,
			sizeof(UserLogonResult),pSourceData->uIndex,pSourceData->dwHandleID);
		return 0;
	}


	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pLogonInfo->LogonInfo.dwUserID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@PassMD5"),pLogonInfo->LogonInfo.szMD5Pass,sizeof(pLogonInfo->LogonInfo.szMD5Pass));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@LoginIP"),szIP,sizeof(szIP));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@RoomID"),m_pInitInfo->uRoomID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@KernelNameID"),pLogonInfo->LogonInfo.uNameID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@MatchTable"),m_pInitInfo->szGameTable,sizeof(m_pInitInfo->szGameTable));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@IPRuleTable"),m_pInitInfo->szIPRuleTable,sizeof(m_pInitInfo->szIPRuleTable));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@NameRuleTable"),m_pInitInfo->szNameRuleTable,sizeof(m_pInitInfo->szNameRuleTable));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@LockTable"),m_pInitInfo->szLockTable,sizeof(m_pInitInfo->szLockTable));


	sqlSPExec(hDatabase,hRecord,true);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);
	
	if(ret!=0)
	{
		iHandleResult = DTR_GR_ERROR_UNKNOW;
		if(ret==1)
			iHandleResult = DTR_GR_USER_IP_LIMITED;
		if(ret==2)
			iHandleResult = DTR_GR_USER_VALIDATA;
		if(ret==11)
		{
			//����û��Ѿ����������䣬�������ڵķ���ID
			DL_GR_O_LogonError _p;
			sqlGetValue(hDatabase,hRecord,"OldGameRoom", _p.uRoomID);
			sqlGetValue(hDatabase,hRecord,"RoomName", _p.szGameRoomName, sizeof(_p.szGameRoomName));
			iHandleResult = DTR_GR_IN_OTHER_ROOM;
			m_pRusultService->OnDataBaseResultEvent(&_p.ResultHead,DTK_GR_LOGON_BY_ID,iHandleResult,
				sizeof(DL_GR_O_LogonError),pSourceData->uIndex,pSourceData->dwHandleID);

			sqlSPClose(hDatabase, hRecord);
			return 0;
		}
		if(ret==3)
			iHandleResult = DTR_GR_USER_NO_FIND;
		if(ret==4)
			iHandleResult = DTR_GR_USER_PASS_ERROR;
		//wushuqun 2009.6.5
		//���ڻ�ս���ʱ����
		if(ret==5)
		{
			iHandleResult = DTR_GR_BATTLEROOM_TIMEOUT;
		}
		if (ret == 9)
			iHandleResult = DTR_GR_CONTEST_NOSIGNUP;
		if (ret == 10)
			iHandleResult = DTR_GR_CONTEST_TIMEROUT;
		if (ret == 12)
			iHandleResult = DTR_GR_CONTEST_NOSTART;
		if (ret == 13)
			iHandleResult = DTR_GR_CONTEST_OVER;
		if (ret == 14)
			iHandleResult = DTR_GR_CONTEST_BEGUN;
		if (ret != 9 && ret != 12)
		{
			m_pRusultService->OnDataBaseResultEvent(&UserLogonResult.ResultHead,DTK_GR_LOGON_BY_ID,iHandleResult,
				sizeof(UserLogonResult),pSourceData->uIndex,pSourceData->dwHandleID);

			sqlSPClose(hDatabase, hRecord);
			return 0;
		}
	}


	//ע���½��Ϣ
	DL_GR_I_RegisterLogon RegUserOnLine;
	DL_GR_O_RegisterLogon RegUserResult;
	memset(&RegUserOnLine,0,sizeof(RegUserOnLine));
	memset(&RegUserResult,0,sizeof(RegUserResult));
	lstrcpy(RegUserOnLine.szAccessIP,szIP);
	RegUserOnLine.dwUserID=pLogonInfo->LogonInfo.dwUserID;
	UserLogonResult.bRegOnline=TRUE;


	//��½�ɹ�
	long int dwGamePower=0,dwMasterPower=0,uMaster=0,uMemberPower=0;
    __int64 i64Money=0;
	long int dwFascination = 0L;//
	UINT uMatch=0;
	UserLogonResult.pUserInfoStruct.bDeskNO=255;///��Ϸ����
	UserLogonResult.pUserInfoStruct.bDeskStation=255;//����λ��
	UserLogonResult.pUserInfoStruct.bUserState=USER_LOOK_STATE;//�û�״̬
	UserLogonResult.pUserInfoStruct.dwUserIP=pLogonInfo->uAccessIP;//��¼IP��ַ
	UserLogonResult.pUserInfoStruct.dwUserID=pLogonInfo->LogonInfo.dwUserID;//ID ����

	sqlGetValue(hDatabase,hRecord,"UserName",UserLogonResult.pUserInfoStruct.szName,sizeof(UserLogonResult.pUserInfoStruct.szName));
	sqlGetValue(hDatabase,hRecord,"NickName",UserLogonResult.pUserInfoStruct.nickName,sizeof(UserLogonResult.pUserInfoStruct.nickName));
	int bb;
	sqlGetValue(hDatabase,hRecord,"sex",bb);
	UserLogonResult.pUserInfoStruct.bBoy=(bb==0)?false:true;
	sqlGetValue(hDatabase,hRecord,"Member",uMemberPower);
	UserLogonResult.pUserInfoStruct.bMember=uMemberPower;
	sqlGetValue(hDatabase,hRecord,"LogoID",UserLogonResult.pUserInfoStruct.bLogoID);
	sqlGetValue(hDatabase,hRecord,"Master",uMaster);
	//////////////////////////////////////////
	///////////Kylin 20090116 VIP�����书��
	UserLogonResult.pUserInfoStruct.bGameMaster=(BYTE)uMaster;
	//////////////////////////////////////////
	sqlGetValue(hDatabase,hRecord,"MasterPower",dwMasterPower);
	UserLogonResult.dwGamePower=dwMasterPower;
	sqlGetValue(hDatabase,hRecord,"Fascination",dwFascination);
	UserLogonResult.pUserInfoStruct.dwFascination=dwFascination;
	int iTime=0;
	sqlGetValue(hDatabase,hRecord,"viptime",iTime);
	UserLogonResult.pUserInfoStruct.iVipTime=iTime;
	sqlGetValue(hDatabase,hRecord,"DoublePointTime",iTime);
	UserLogonResult.pUserInfoStruct.iDoublePointTime=iTime;
	sqlGetValue(hDatabase,hRecord,"ProtectTime",iTime);
	UserLogonResult.pUserInfoStruct.iProtectTime=iTime;
	sqlGetValue(hDatabase,hRecord,"MatchMember",uMatch);

	////������ʯ��ݱ�ʶ add by huangYuanSong 09.07.14
	int iDiamondTime=0;
	sqlGetValue(hDatabase,hRecord,"DiamondTime",iDiamondTime);

	CString strTime;
	strTime.Format("%d",CTime::GetCurrentTime());
	int curTime=atoi(strTime);
	if(iDiamondTime>curTime)
	{
		UserLogonResult.pUserInfoStruct.userInfoEx1=iDiamondTime;
	}
	else
	{
		UserLogonResult.pUserInfoStruct.userInfoEx1=0;
	}
	//----------

	//wushuqun 2009.6.1
	//����VIPʱ����ȷ���Ƿ��ǻ�Ա
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);
	if(UserLogonResult.pUserInfoStruct.iVipTime>curtime)
	{
		uMemberPower=1;
		UserLogonResult.pUserInfoStruct.bMember=uMemberPower;
	}

	//�Ƿ��Ա����
	if (m_pInitInfo->dwRoomRule&GRR_MEMBER_ROOM)
	{
		if (uMemberPower==0 && uMaster==0) 
		{
			iHandleResult = DTR_GR_ONLY_MEMBER;
			m_pRusultService->OnDataBaseResultEvent(&UserLogonResult.ResultHead,DTK_GR_LOGON_BY_ID,iHandleResult,
				sizeof(UserLogonResult),pSourceData->uIndex,pSourceData->dwHandleID);

			sqlSPClose(hDatabase, hRecord);

			return 0;
		}
	}
	if (m_pInitInfo->uComType==2&&m_pInitInfo->dwRoomRule&GRR_MATCH_REG)
	{
		if (uMatch==0 && uMaster==0) 
		{
			iHandleResult = DTR_GR_MATCH_LOGON;
			m_pRusultService->OnDataBaseResultEvent(&UserLogonResult.ResultHead,DTK_GR_LOGON_BY_ID,iHandleResult,
				sizeof(UserLogonResult),pSourceData->uIndex,pSourceData->dwHandleID);

			sqlSPClose(hDatabase, hRecord);

			return 0;
		}
	}

	sqlGetValue(hDatabase,hRecord,"Wallet",i64Money);
	if(m_pInitInfo->uComType==TY_MONEY_GAME && (m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM || m_pInitInfo->dwRoomRule & GRR_CONTEST))
		sqlGetValue(hDatabase,hRecord,"ExercisePoint",i64Money);

	sqlGetValue(hDatabase,hRecord,"BankMoney",UserLogonResult.pUserInfoStruct.i64Bank);
	sqlGetValue(hDatabase,hRecord,"GamePower",dwGamePower);
	UserLogonResult.dwGamePower=dwGamePower;
	sqlGetValue(hDatabase,hRecord,"MasterPowerInRoom",dwMasterPower);

	sqlGetValue(hDatabase,hRecord,"Win",UserLogonResult.pUserInfoStruct.uWinCount);
	sqlGetValue(hDatabase,hRecord,"Lost",UserLogonResult.pUserInfoStruct.uLostCount);
	sqlGetValue(hDatabase,hRecord,"Cut",UserLogonResult.pUserInfoStruct.uCutCount);
	sqlGetValue(hDatabase,hRecord,"Mid",UserLogonResult.pUserInfoStruct.uMidCount);
	sqlGetValue(hDatabase,hRecord,"Point",UserLogonResult.pUserInfoStruct.dwPoint);

	////�õ������¼ӵ��û���Ϣ  yjj 090319
	sqlGetValue(hDatabase,hRecord,"OccuPation",UserLogonResult.pUserInfoStruct.szOccupation,sizeof(UserLogonResult.pUserInfoStruct.szOccupation));
	sqlGetValue(hDatabase,hRecord,"PhoneNum",UserLogonResult.pUserInfoStruct.szPhoneNum,sizeof(UserLogonResult.pUserInfoStruct.szPhoneNum));
	sqlGetValue(hDatabase,hRecord,"AdrProvince",UserLogonResult.pUserInfoStruct.szProvince,sizeof(UserLogonResult.pUserInfoStruct.szProvince));
	sqlGetValue(hDatabase,hRecord,"AdrCity",UserLogonResult.pUserInfoStruct.szCity,sizeof(UserLogonResult.pUserInfoStruct.szCity));
	sqlGetValue(hDatabase,hRecord,"AdrZone",UserLogonResult.pUserInfoStruct.szZone,sizeof(UserLogonResult.pUserInfoStruct.szZone));
	//--
	// duanxiaohui 20111111 ��ȡ����ǩ��
	sqlGetValue(hDatabase,hRecord,"SignDescr",UserLogonResult.pUserInfoStruct.szSignDescr,sizeof(UserLogonResult.pUserInfoStruct.szSignDescr));
	// end duanxiaohui
	//wushuqun 2009.6.6
	//��¼����ʱ��ʱ��ȡ�����������
	sqlGetValue(hDatabase,hRecord,"VirualUser",UserLogonResult.nVirtualUser);
	//
	sqlGetValue(hDatabase,hRecord,"PresentCoinNum",UserLogonResult.nPresentCoinNum); ///< ����ʱ���ͽ�� Y.C.YAN
	///< ��¼����ʱ��,������
	CBcfFile f(CBcfFile::GetAppPath() + "Function.bcf");
	CString strValue = f.GetKeyVal("OnlineCoin","Available","0");
	if (0 == atoi(strValue))
	{
		UserLogonResult.nPresentCoinNum = 0;
	}

	//{Added by Zxd 20100629
	/// �ô洢���̵ķ���ֵ�����û����ݽṹ��isVirtual�ֶθ�ֵ��ָʾ���û��Ƿ�Ϊ�������ʺ�
	sqlGetValue(hDatabase,hRecord,"IsRobot", UserLogonResult.pUserInfoStruct.isVirtual);
	//Added by Zxd 20100629}

	//wushuqun 2009.6.26
	//��̨���õ��������,0Ϊ��ͨ���,1Ϊ�������
	sqlGetValue(hDatabase,hRecord,"UserType",UserLogonResult.pUserInfoStruct.userType);
	//VIP��ϵͳ�Զ�ʶ�𣬶������ɺ�̨�趨
	CString stimeTmp;
	stimeTmp.Format("%d",CTime::GetCurrentTime());
	int curtimeTmp=atoi(stime);
	if(UserLogonResult.pUserInfoStruct.iVipTime>curtimeTmp)
	{
		//vip��Ա

		if (UserLogonResult.pUserInfoStruct.userType == 0)
		{
			//��Ϊ��ͨVIP���
			UserLogonResult.pUserInfoStruct.userType = 2;
		}
		else if (UserLogonResult.pUserInfoStruct.userType == 1)
		{
			//��Ϊ����VIP���
			UserLogonResult.pUserInfoStruct.userType = 3;
		}
		else
		{
			//����̨���ò���0,1,Ĭ��Ϊ��ͨ���
			UserLogonResult.pUserInfoStruct.userType = 0;
		}
	}
	//{{ added by zxd 20100806 �жϱ��û��Ƿ�Ϊ����״̬�����ǣ����¼ʱ��
	int nStatusByGM = 0;
	sqlGetValue(hDatabase,hRecord,"StatusByGM", nStatusByGM);
	if (nStatusByGM == 2)
	{
		COleDateTime CurrentTime = COleDateTime::GetCurrentTime(); ///< ��ǰʱ��
		COleDateTime timeNoTalk;
		
		sqlGetValue(hDatabase,hRecord,"StatusTime", timeNoTalk);
		if(timeNoTalk>CurrentTime)
		{
			SYSTEMTIME systime;
			VariantTimeToSystemTime(timeNoTalk, &systime);
			CTime timeTmp(systime);

			CString strTimeNoTalk;
			strTimeNoTalk.Format("%d", timeTmp);
			UserLogonResult.pUserInfoStruct.userInfoEx2 = atoi(strTimeNoTalk);
			//bool bOutOfDate = curTime > UserLogonResult.pUserInfoStruct.userInfoEx2;
		}
	}
	else
	{
		UserLogonResult.pUserInfoStruct.userInfoEx2 = 0;
	}

	// added by zxd 20100806 �жϱ��û��Ƿ�Ϊ����״̬�����ǣ����¼ʱ��}}

	//��ȡ����Ƿ������� lxl 2010-12-10
	sqlGetValue(hDatabase,hRecord,"Trader",UserLogonResult.pUserInfoStruct.bTrader);

	sqlGetValue(hDatabase,hRecord,"ContestScore",UserLogonResult.pUserInfoStruct.i64ContestScore);

// 	CString str;
// 	str.Format("DServer:Read Score From DB User = %s ContestScore = %I64d",
// 		UserLogonResult.pUserInfoStruct.szName,
// 		UserLogonResult.pUserInfoStruct.i64ContestScore);
// 	OutputDebugString(str);

	sqlGetValue(hDatabase,hRecord,"ContestCount",UserLogonResult.pUserInfoStruct.iContestCount);
	sqlGetValue(hDatabase,hRecord,"RankNum",UserLogonResult.pUserInfoStruct.iRankNum);
	int nYear = 0, nMonth = 0, nDate = 0, nHour = 0, nMin = 0, nSec = 0, nss = 0;  
	CString temp;
	sqlGetValue(hDatabase, hRecord, TEXT("StartTime"), temp);
	if (temp != "")
	{
		sscanf(temp.GetBuffer(), TEXT("%d-%d-%d %d:%d:%d.%d"), &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec, &nss);

		CTime _t(nYear, nMonth, nDate, nHour, nMin, nSec); 
		UserLogonResult.pUserInfoStruct.timeLeft = _t.GetTime();
	}
	else
	{
		UserLogonResult.pUserInfoStruct.timeLeft = 0;
	}


	sqlCloseRecord(hDatabase,hRecord);
	UserLogonResult.pUserInfoStruct.i64Money=i64Money;
	UserLogonResult.dwGamePower|=dwGamePower;
	UserLogonResult.dwGamePower|=m_pInitInfo->dwUserPower;//��Ϸ����Ȩ�ޣ����е��˶���
	UserLogonResult.dwMasterPower|=dwMasterPower;//�ܵĹ���Ȩ��
	CopyMemory(UserLogonResult.szMD5Pass, pLogonInfo->LogonInfo.szMD5Pass, sizeof(UserLogonResult.szMD5Pass));

	//�����½
	m_pRusultService->OnDataBaseResultEvent(&UserLogonResult.ResultHead,DTK_GR_LOGON_BY_ID,iHandleResult,
		sizeof(UserLogonResult),pSourceData->uIndex,pSourceData->dwHandleID);

	sqlSPClose(hDatabase, hRecord);

	return 0;
}

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
UINT CGameDataBaseHandle::OnGetNickNameOnID(DataBaseLineHead * pSourceData)
{
    // Ч������
    if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_GetNickNameOnID)) throw DTR_GR_ERROR_UNKNOW;
    DL_GR_I_GetNickNameOnID * pGetNickNameOnID =(DL_GR_I_GetNickNameOnID *)pSourceData;

    // ��ʼ�����
    DL_GR_O_GetNickNameOnIDResult GetNickNameOnIDResult;
    memset(&GetNickNameOnIDResult,0,sizeof(GetNickNameOnIDResult));

    int iHandleResult=DTR_GR_LOGON_SUCCESS;

    HANDLE hDatabase=m_pDataBaseManage->hDBNative;
    HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GetNickNameOnID",true);
    if(hRecord<(HANDLE)0x1000)
    {
        sqlCloseRecord(hDatabase,hRecord);
        iHandleResult=DTR_GR_DATABASE_CUT;
		m_pRusultService->OnDataBaseResultEvent(&GetNickNameOnIDResult.ResultHead,pSourceData->uHandleKind,
                            iHandleResult,sizeof(GetNickNameOnIDResult),pSourceData->uIndex,pSourceData->dwHandleID);
        return 0;
    }

    sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pGetNickNameOnID->stGetNickNameOnID.iUserID);
    sqlSPExec(hDatabase,hRecord,true);
    int ret=sqlSPGetReturnValue(hDatabase,hRecord);
    sqlGetValue(hDatabase,hRecord,"NickName",GetNickNameOnIDResult.szNickName,sizeof(GetNickNameOnIDResult.szNickName));
    sqlCloseRecord(hDatabase,hRecord);

    // ����ʧ��
    if(ret!=0)
    {
        iHandleResult = DTR_GR_ERROR_UNKNOW;
        if(ret==1)      // ID ������
            iHandleResult = DTR_GR_GETNICKNAME_NOTEXIST;
        if(ret==2)      // �ǳ�Ϊ��
            iHandleResult = DTR_GR_GETNICKNAME_ISNULL;

		m_pRusultService->OnDataBaseResultEvent(&GetNickNameOnIDResult.ResultHead,pSourceData->uHandleKind,
                            iHandleResult,sizeof(GetNickNameOnIDResult),pSourceData->uIndex,pSourceData->dwHandleID);
        return 0;
    }

    // �����ɹ�
    iHandleResult = DTR_GR_GETNICKNAME_SUCCESS;
    GetNickNameOnIDResult.iUserID = pGetNickNameOnID->stGetNickNameOnID.iUserID;    
	m_pRusultService->OnDataBaseResultEvent(&GetNickNameOnIDResult.ResultHead,pSourceData->uHandleKind,iHandleResult,
                            sizeof(GetNickNameOnIDResult),pSourceData->uIndex,pSourceData->dwHandleID);
    return 0;
}

// PengJiLin, 2010-9-10, ʹ�����˿������߿�����
UINT CGameDataBaseHandle::OnUseKickProp(DataBaseLineHead * pSourceData, BOOL bIsKickUser)
{
    // Ч������
    if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UseKickProp)) throw DTR_GR_ERROR_UNKNOW;
    DL_GR_I_UseKickProp* pUseKickProp =(DL_GR_I_UseKickProp *)pSourceData;

    // �����˿����Ƿ��߿�
    int iHandleIDNum = DTK_GR_USE_KICK_PROP;
    if(FALSE == bIsKickUser)
    {
        iHandleIDNum = DTK_GR_USE_ANTI_KICK_PROP;
    }

    // ��ʼ�����
    DL_GR_O_UseKickProp UseKickProp;
    memset(&UseKickProp,0,sizeof(UseKickProp));

    UseKickProp.dwUserID = pUseKickProp->dwUserID;
    UseKickProp.iPropID = pUseKickProp->iPropID;

    HANDLE hDatabase = m_pDataBaseManage->hDBNative;
    HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_UseProp",true);
    if(hRecord<(HANDLE)0x1000) return 0;

    sqlSPAddPara(hDatabase,hRecord,"@UserID", pUseKickProp->dwUserID);
    sqlSPAddPara(hDatabase,hRecord,"@PropID", pUseKickProp->iPropID);
    sqlSPAddPara(hDatabase,hRecord,"@TargetID", 0);

    //���NameID�����ڸ�������
    sqlSPAddPara(hDatabase,hRecord,"@GameNameID",0);
    char chNotUse[10] = {0};
    sqlSPAddPara(hDatabase,hRecord,"@GameTable",chNotUse,sizeof(chNotUse));
    //�Ƿ񸺷�����
    sqlSPAddPara(hDatabase,hRecord,"@IsClearMinus", 0);
    //�Ƿ��С���� // PengJiLin, 2010-9-10, ��1����ֱ�ӷ��ز�����������
    sqlSPAddPara(hDatabase,hRecord,"@IsBoard",1);
    //˫�����ֱ仯

    sqlSPAddPara(hDatabase,hRecord,"@DoubleTime",0);
    //������仯
    sqlSPAddPara(hDatabase,hRecord,"@ProtectTime",0);
    //����ֵ�仯
    sqlSPAddPara(hDatabase,hRecord,"@FasciChange",0);
    sqlSPExec(hDatabase,hRecord,false);

    int ret=sqlSPGetReturnValue(hDatabase,hRecord);
    sqlCloseRecord(hDatabase,hRecord);

    int iHandleResult = DTR_GR_USE_KICKPROP_SUCCESS;

    // ����ʧ�ܣ�û�е�����
    if(0 != ret)
    {
        iHandleResult = DTR_GR_USE_KICKPROP_NULL;
        m_pRusultService->OnDataBaseResultEvent(&UseKickProp.ResultHead, iHandleIDNum, 
                                                iHandleResult, sizeof(DL_GR_O_UseKickProp), 
                                                pSourceData->uIndex, pSourceData->dwHandleID);
        return 0;
    }

    // �����ɹ���������ֵ
    hRecord=sqlSPSetNameEx(hDatabase,"SP_UseKickProp",true);
    if(hRecord<(HANDLE)0x1000)
        return 0;
    
    sqlSPAddPara(hDatabase,hRecord,"@UserID", pUseKickProp->dwUserID);
    sqlSPAddPara(hDatabase,hRecord,"@PropID", pUseKickProp->iPropID);
    sqlSPAddPara(hDatabase,hRecord,"@bIsKick", bIsKickUser?1:0);  // 0 = ���߿�, 1 = ���˿�

    sqlSPExec(hDatabase,hRecord,true);

    ret=sqlSPGetReturnValue(hDatabase,hRecord);
    sqlGetValue(hDatabase,hRecord,"TotalTime", UseKickProp.iTotalTime);
    sqlCloseRecord(hDatabase,hRecord);
    
    iHandleResult = DTR_GR_USE_KICKPROP_SET_SUCCESS;

    // ����ʧ��
    if(0 != ret)
    {
        iHandleResult = DTR_GR_USE_KICKPROP_SET_ERROR;
    }

    m_pRusultService->OnDataBaseResultEvent(&UseKickProp.ResultHead, iHandleIDNum, 
                                            iHandleResult, sizeof(DL_GR_O_UseKickProp), 
                                            pSourceData->uIndex, pSourceData->dwHandleID);
    return 0;
}

// PengJiLin, 2010-9-13, ʹ�����˿����˹���
UINT CGameDataBaseHandle::OnNewKickUserProp(DataBaseLineHead * pSourceData)
{
    // Ч������
    if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_NewKickUserProp)) throw DTR_GR_ERROR_UNKNOW;
    DL_GR_I_NewKickUserProp* pUseKickProp =(DL_GR_I_NewKickUserProp *)pSourceData;

    // ��ʼ�����
    DL_GR_O_UseNewKickUserProp UseKickProp;
    memset(&UseKickProp,0,sizeof(UseKickProp));

    UseKickProp.dwDestIndex = pUseKickProp->dwDestIndex;
    UseKickProp.dwDestHandleID = pUseKickProp->dwDestHandleID;
    UseKickProp.dwUserID = pUseKickProp->dwUserID;
    UseKickProp.dwDestID = pUseKickProp->dwDestID;

    HANDLE hDatabase = m_pDataBaseManage->hDBNative;
    HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_NewKickUserProp",true);
    if(hRecord<(HANDLE)0x1000) return 0;

    sqlSPAddPara(hDatabase,hRecord,"@UserID", pUseKickProp->dwUserID);
    sqlSPAddPara(hDatabase,hRecord,"@TargetID", pUseKickProp->dwDestID);
    sqlSPExec(hDatabase,hRecord);

    int ret=sqlSPGetReturnValue(hDatabase,hRecord);
    sqlCloseRecord(hDatabase,hRecord);

    int iHandleResult = DTR_GR_NEW_KICKUSER_SUCCESS;
    switch(ret)
    {
    case 1:
        iHandleResult = DTR_GR_NEW_KICKUSER_NOTIME; // �Լ��ĵ����ѹ��ڣ�����û�е���
        break;
    case 2:
        iHandleResult = DTR_GR_NEW_KICKUSER_HAD_ANTI; // �Է��з��߿�ʱ��
        break;
    case 3:
        iHandleResult = DTR_GR_NEW_KICKUSER_HAD_ANTI; // �Է���VIPʱ��
        break;
    }

    m_pRusultService->OnDataBaseResultEvent(&UseKickProp.ResultHead, DTK_GR_USER_NEW_KICK_USER_PROP, 
                                        iHandleResult, sizeof(DL_GR_O_UseNewKickUserProp), 
                                        pSourceData->uIndex, pSourceData->dwHandleID);
}


UINT CGameDataBaseHandle::OnBankOperateNormal(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode = DTR_GR_ERROR_UNKNOW;
		db_bank_op_normal_in* _p = (db_bank_op_normal_in*)pSourceData;
		db_bank_op_normal_out _out;
		ZeroMemory(&_out, sizeof(_out));
		_out._data = _p->_data;

		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankOpNormal", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_data._user_id);
		b = sqlSPAddPara(hDatabase,hRecord,"@GameID", _p->_data._game_id);
		b = sqlSPAddPara(hDatabase,hRecord,"@OperateType", _p->_data._operate_type);
		b = sqlSPAddPara_i64(hDatabase,hRecord,"@Money", _p->_data._money);
		b = sqlSPAddPara(hDatabase,hRecord,"@MD5Pass", _p->_data._szMD5Pass,sizeof(_p->_data._szMD5Pass));

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		if (iHandleCode == HC_BANK_OP_SUC)
		{
			sqlGetValue(hDatabase,hRecord, "MoneyOp", _out._data._money);
		}

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_NORMAL, iHandleCode,
			sizeof(db_bank_op_normal_out), pSourceData->uIndex, pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

UINT CGameDataBaseHandle::OnBankOperateTransfer(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode = DTR_GR_ERROR_UNKNOW;
		db_bank_op_transfer_in* _p = (db_bank_op_transfer_in*)pSourceData;
		db_bank_op_transfer_out _out;
		ZeroMemory(&_out, sizeof(_out));
		_out._data = _p->_data;

		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankOpTransfer", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID1", _p->_data._user_id1);
		b = sqlSPAddPara(hDatabase,hRecord,"@UserID2", _p->_data._user_id2);
		CString s;
		s.Format("%I64d", _p->_data._money);
		sqlSPAddPara(hDatabase,hRecord,"@Money", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		if (iHandleCode == 0)
		{
			//sqlGetValue(hDatabase,hRecord, "MoneyOp", _out._data._money);
		}

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_TRANSFER, iHandleCode,
			sizeof(db_bank_op_normal_out), pSourceData->uIndex, iRet);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

UINT CGameDataBaseHandle::OnBankOperateDeposit(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode = DTR_GR_ERROR_UNKNOW;
		db_bank_op_deposit_in* _p = (db_bank_op_deposit_in*)pSourceData;
		db_bank_op_deposit_out _out;
		ZeroMemory(&_out, sizeof(_out));
		_out._data = _p->_data;

		//HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		//HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankOpDeposit", true);

		//bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_data._user_id);
		//b = sqlSPAddPara(hDatabase,hRecord,"@OperateType", _p->_data._operate_type);
		//CString s;
		//s.Format("%I64d", _p->_data._money);
		//sqlSPAddPara(hDatabase,hRecord,"@Money", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());

		//sqlSPExec(hDatabase,hRecord, true);
		//int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		//iHandleCode = iRet;

		//if (iHandleCode == 0)
		//{
		//	CString tmp;
		//	sqlGetValue(hDatabase,hRecord, "DepositID", tmp);
		//	strcpy(_out._data._deposit_id, tmp.GetBuffer());
		//	sqlGetValue(hDatabase,hRecord, "DepositPassWord", tmp);
		//	strcpy(_out._data._deposit_password, tmp.GetBuffer());

		//}

		HANDLE hDatabase;
		HANDLE hRecord;
		int iRet;

		if (_p->_data._operate_type == 1)
		{
			hDatabase=m_pDataBaseManage->hDBNative;
			hRecord=sqlSPSetNameEx(hDatabase,"SP_Bank_NewDeposit", true);

			bool b=sqlSPAddPara(hDatabase,hRecord,"@CreaterUserID", _p->_data._user_id);
			b = sqlSPAddPara_i64(hDatabase,hRecord,"@Money", _p->_data._money);
			sqlSPAddPara(hDatabase,hRecord,"@MD5Pass",_p->_data._szMD5Pass,sizeof(_p->_data._szMD5Pass));

			sqlSPExec(hDatabase,hRecord, true);
			iRet = sqlSPGetReturnValue(hDatabase, hRecord);
			iHandleCode = iRet;

			if (iHandleCode == 0)
			{
				CString tmp;
				sqlGetValue(hDatabase,hRecord, "DepositID", tmp);
				strcpy(_out._data._deposit_id, tmp.GetBuffer());
				sqlGetValue(hDatabase,hRecord, "DepositPassWord", tmp);
				strcpy(_out._data._deposit_password, tmp.GetBuffer());
				_out._data._state = 0;
				sqlGetValue(hDatabase,hRecord, "CDSC", _out._data._csds);
				sqlGetValue(hDatabase,hRecord, "Money", _out._data._money);
			}
		}
		else if (_p->_data._operate_type == 2)
 		{
			hDatabase=m_pDataBaseManage->hDBNative;
			hRecord=sqlSPSetNameEx(hDatabase,"SP_Bank_UseDeposit", true);

			bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_data._user_id);
			CString s = _p->_data._deposit_id;
			sqlSPAddPara(hDatabase,hRecord,"@DepositID", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());
			s = _p->_data._deposit_password;
			sqlSPAddPara(hDatabase,hRecord,"@DepositPwd", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());

			sqlSPExec(hDatabase,hRecord, true);
			iRet = sqlSPGetReturnValue(hDatabase, hRecord);
			iHandleCode = iRet;

			if (iHandleCode == 0)
			{
				_out._data._state = 1;
				sqlGetValue(hDatabase, hRecord, "UserID", _out._data._user_id);
				sqlGetValue(hDatabase, hRecord, "NickName", _out._data._szNickName,sizeof(_out._data._szNickName));
				sqlGetValue(hDatabase, hRecord, "Money", _out._data._money);
			}
		}

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_DEPOSIT, iHandleCode,
			sizeof(db_bank_op_deposit_out), pSourceData->uIndex, iRet);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

UINT CGameDataBaseHandle::OnBankQueryDeposits(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode = DTR_GR_ERROR_UNKNOW;
		db_bank_op_query_deposit_in* _p = (db_bank_op_query_deposit_in*)pSourceData;
		db_bank_op_query_deposit_out _out;
		ZeroMemory(&_out, sizeof(_out));

		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_Bank_QueryDeposits", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_UserID);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		int nSeriNo = 0;

		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(db_bank_op_query_deposit_out));

			_out._data._serial_no = nSeriNo++;
			sqlGetValue(hDatabase,hRecord,"DepositID",_out._data._deposit_id,sizeof(_out._data._deposit_id));
			sqlGetValue(hDatabase,hRecord,"DepositPassWord",_out._data._deposit_password,sizeof(_out._data._deposit_password));
			sqlGetValue(hDatabase,hRecord,"Money",_out._data._money);
			sqlGetValue(hDatabase,hRecord,"UserID",_out._data._user_id);
			sqlGetValue(hDatabase,hRecord,"GreateTime",_out._data._create_time);
			sqlGetValue(hDatabase,hRecord,"CDSC",_out._data._csds);
			sqlGetValue(hDatabase,hRecord,"State",_out._data._state);
			sqlGetValue(hDatabase,hRecord,"UsedUserID",_out._data._used_user_id);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_QUERY_DEPOSIT, 0, sizeof(db_bank_op_query_deposit_out),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}

		ZeroMemory(&_out,sizeof(db_bank_op_query_deposit_out));
		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_QUERY_DEPOSIT, 1, sizeof(db_bank_op_query_deposit_out), pSourceData->uIndex, pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

UINT CGameDataBaseHandle::OnBankGetWallet(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode = DTR_GR_ERROR_UNKNOW;
		db_bank_op_wallet_in* _p = (db_bank_op_wallet_in*)pSourceData;
		db_bank_game_wallet_out _out;
		ZeroMemory(&_out, sizeof(_out));

		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankOpWallet", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_UserID);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(db_bank_game_wallet_out));

			//sqlGetValue(hDatabase,hRecord,"UserID", _out._data._user_id);
			sqlGetValue(hDatabase,hRecord,"GameID",_out._data._game_id);
			sqlGetValue(hDatabase,hRecord,"ComName",_out._data._game_name,sizeof(_out._data._game_name));
			sqlGetValue(hDatabase,hRecord,"WalletMoney",_out._data._money);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_WALLET, 0, sizeof(db_bank_game_wallet_out),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}

		ZeroMemory(&_out,sizeof(db_bank_game_wallet_out));
		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GR_BANK_WALLET, 1, sizeof(db_bank_game_wallet_out), pSourceData->uIndex, pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//vipʱ����
//UINT CGameDataBaseHandle::OnVipTimerCheck(DataBaseLineHead *pSourceData)
//{//����������Բ�Ҫ�˰�
//	return TRUE;
//
//}

//ȡ��
UINT CGameDataBaseHandle::OnCheckOutMoney(DataBaseLineHead *pSourceData, UINT uCode)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_CheckOutMoneyWithPwd_t)) throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_CheckOutMoneyWithPwd_t * pCheckOutMoney=(DL_GR_I_CheckOutMoneyWithPwd_t * )pSourceData;
	DL_GR_O_CheckOutMoney pCheckOutMoneyResult;///���
	memset(&pCheckOutMoneyResult,0,sizeof(pCheckOutMoneyResult));
	if(pCheckOutMoney->i64Count<0) return 0; 

	try
	{
		__int64 i64Bank=0,i64Money=0,i64CheckInMoney = 0;

		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankMoneyOpera",true);
		if(hRecord<(HANDLE)0x1000)
			return 0;

		sqlSPAddPara(hDatabase,hRecord,"@UserID",pCheckOutMoney->dwUserID);
		CString s;
		s.Format("%I64d", pCheckOutMoney->i64Count);
		sqlSPAddPara(hDatabase,hRecord,"@MoneyQuantitys", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());
		sqlSPAddPara(hDatabase,hRecord,"@RoomID",m_pInitInfo->uRoomID);
		sqlSPAddPara(hDatabase,hRecord,"@IsSave",0);	//0-ȡ��,1-���
		sqlSPAddPara(hDatabase,hRecord,"@MD5Pass",pCheckOutMoney->szMD5Pass, sizeof(pCheckOutMoney->szMD5Pass));//����

		sqlSPExec(hDatabase,hRecord,true);

		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		
		if(ret!=0)
		{
			sqlCloseRecord(hDatabase,hRecord);	// zxj dfw 2009-11-12

			if (ret == 5)
			{
				throw DTR_GR_CHECKOUT_MONEY_INV;
			}
			else
			{
				throw DTR_GR_ERROR_UNKNOW;
			}
			
		}
		 
		sqlGetValue(hDatabase,hRecord,"MoneyInBank",i64Bank);
		sqlGetValue(hDatabase,hRecord,"MoneyChange",i64CheckInMoney);
		sqlGetValue(hDatabase,hRecord,"MoneyInWallet",i64Money);

		sqlCloseRecord(hDatabase,hRecord);


		pCheckOutMoneyResult.dwUserID=pCheckOutMoney->dwUserID;
		pCheckOutMoneyResult.i64CheckOut=i64CheckInMoney;
		pCheckOutMoneyResult.i64MoneyInBank=i64Bank - i64CheckInMoney;
		pCheckOutMoneyResult.i64MoneyInRoom=i64Money + i64CheckInMoney;
		throw DTR_GR_CHECKOUT_MONEY_SUC;

	}
	catch (int iHandleResult)
	{TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	m_pRusultService->OnDataBaseResultEvent(&pCheckOutMoneyResult.ResultHead, uCode, iHandleResult,
		sizeof(pCheckOutMoneyResult),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	return 0;
}

//���
UINT CGameDataBaseHandle::OnCheckInMoney(DataBaseLineHead *pSourceData, UINT uCode)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_CheckOutMoney)) throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_CheckOutMoney * pCheckInMoney=(DL_GR_I_CheckOutMoney * )pSourceData;
	DL_GR_O_CheckOutMoney pCheckInMoneyResult;///���
	memset(&pCheckInMoneyResult,0,sizeof(pCheckInMoneyResult));
	if(pCheckInMoney->i64Count<0) return 0; 


	try
	{
		__int64 i64Bank=0,i64Money=0, i64CheckInMoney = 0;

		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankMoneyOpera",true);
		if(hRecord<(HANDLE)0x1000)
			return 0;

		sqlSPAddPara(hDatabase,hRecord,"@UserID",pCheckInMoney->dwUserID);
		//TCHAR _num[32];
		//ZeroMemory(_num, sizeof(_num));
		//_i64tow(pCheckInMoney->i64Count, _num, 10);
		CString s;
		s.Format("%I64d", pCheckInMoney->i64Count);
		sqlSPAddPara(hDatabase,hRecord,"@MoneyQuantitys", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());
		sqlSPAddPara(hDatabase,hRecord,"@RoomID",m_pInitInfo->uRoomID);
		sqlSPAddPara(hDatabase,hRecord,"@IsSave",1);	//0-ȡ��,1-���
		sqlSPAddPara(hDatabase,hRecord,"@MD5Pass", "", sizeof(""));//����

		sqlSPExec(hDatabase,hRecord,true);

		int ret=sqlSPGetReturnValue(hDatabase,hRecord);

		if(ret!=0)
		{
			sqlCloseRecord(hDatabase,hRecord);	// zxj dfw 2009-11-12

			if (ret == 5)
			{
				throw DTR_GR_CHECKIN_MONEY_INV;
			}
			else
			{
				throw DTR_GR_ERROR_UNKNOW;
			}
		}

		sqlGetValue(hDatabase,hRecord,"MoneyInBank",i64Bank);
		sqlGetValue(hDatabase,hRecord,"MoneyChange",i64CheckInMoney);
		sqlGetValue(hDatabase,hRecord,"MoneyInWallet",i64Money);

		s.Format("CheckIn MoneyInBank = %I64d, MoneyChange = %I64d, MoneyInWallet = %I64d", i64Bank, i64CheckInMoney, i64Money);
		OutputDebugString(s);

		sqlCloseRecord(hDatabase,hRecord);


		pCheckInMoneyResult.dwUserID=pCheckInMoney->dwUserID;
		pCheckInMoneyResult.i64CheckOut=i64CheckInMoney;
		pCheckInMoneyResult.i64MoneyInBank=i64Bank + i64CheckInMoney;
		pCheckInMoneyResult.i64MoneyInRoom=i64Money - i64CheckInMoney;
		throw DTR_GR_CHECKIN_MONEY_SUC;

	}
	catch (int iHandleResult)
	{TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		m_pRusultService->OnDataBaseResultEvent(&pCheckInMoneyResult.ResultHead, uCode, iHandleResult,
			sizeof(pCheckInMoneyResult),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	return 0;
}
//ת�ʼ�¼
UINT CGameDataBaseHandle::OnTransferRecord(DataBaseLineHead *pSourceData, UINT uCode)
{
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_TransferRecord_t)) throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_TransferRecord_t *pTransferRecord = (DL_GR_I_TransferRecord_t *)pSourceData;
	DL_GR_O_TransferRecord_t stTransferRecordResult;
	ZeroMemory(&stTransferRecordResult, sizeof(DL_GR_O_TransferRecord_t));
	if (pTransferRecord->stTransfer.dwUserID == 0)
	{
		return 0;
	}
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GetTransferRecord",true);
		if(hRecord<(HANDLE)0x1000)
			return 0;
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pTransferRecord->stTransfer.dwUserID);
		sqlSPExec(hDatabase,hRecord,true);
		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		if (ret>0)
		{
			sqlCloseRecord(hDatabase,hRecord);
			return 0;
		}
		/// ÿ��ȡһ����¼���ͷ���һ�Σ�ֱ����ȡ������smalldatetime���͵������ڿͻ��˽���
		int nSeriNo = 0;
		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			stTransferRecordResult.transferRecord.nSeriNo = nSeriNo++;
			sqlGetValue(hDatabase,hRecord,"UserID", stTransferRecordResult.transferRecord.dwUserID);
			sqlGetValue(hDatabase,hRecord,"DestUserID", stTransferRecordResult.transferRecord.destUserID);
			sqlGetValue(hDatabase,hRecord,"TransferMoney", stTransferRecordResult.transferRecord.i64Money);
			sqlGetValue(hDatabase,hRecord,"ActualTransfer", stTransferRecordResult.transferRecord.i64ActualTransfer);
			sqlGetValue(hDatabase,hRecord,"TransTime", stTransferRecordResult.transferRecord.oleDateTime);

			m_pRusultService->OnDataBaseResultEvent(&stTransferRecordResult.ResultHead,
				uCode, DTR_GR_OPEN_WALLET_SUC,
				sizeof(stTransferRecordResult),
				pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}
		sqlCloseRecord(hDatabase,hRecord);
	}

	return 0;
}
//�޸�����
UINT CGameDataBaseHandle::OnChangePasswd(DataBaseLineHead *pSourceData, UINT uCode)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_ChangePasswd_t))
	{
		return 0;
	}
	DL_GR_I_ChangePasswd_t *pChangePasswd = (DL_GR_I_ChangePasswd_t *)pSourceData;
	DL_GR_O_ChangePasswd_t changePasswdResult;

	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_ChangeBankPasswd",true);
		if(hRecord<(HANDLE)0x1000)
			return 0;
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pChangePasswd->stChangePasswd.UserID);
		sqlSPAddPara(hDatabase,hRecord,"@MD5PassOld",pChangePasswd->stChangePasswd.szMD5PassOld, sizeof(pChangePasswd->stChangePasswd.szMD5PassOld));//������
		sqlSPAddPara(hDatabase,hRecord,"@MD5PassNew",pChangePasswd->stChangePasswd.szMD5PassNew, sizeof(pChangePasswd->stChangePasswd.szMD5PassNew));//������
		sqlSPExec(hDatabase,hRecord,true);

		int nRet = sqlSPGetReturnValue(hDatabase,hRecord);
		changePasswdResult.nResult = nRet;
		sqlCloseRecord(hDatabase,hRecord);
		UINT uResult = DTR_GR_OPEN_WALLET_SUC;
		if (nRet != 0)
		{
			uResult = DTR_GR_OPEN_WALLET_ERROR_PSW;
		}
		// ֻ�гɹ���ʧ�����ֽ��
		m_pRusultService->OnDataBaseResultEvent(&changePasswdResult.ResultHead,
			uCode, uResult,
			sizeof(changePasswdResult),
			pSourceData->uIndex,pSourceData->dwHandleID);
	}

	return 0;
}
//ת��
UINT CGameDataBaseHandle::OnTransferMoney(DataBaseLineHead *pSourceData, UINT uCode)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_TransferMoney))
	{
		return 0;
	}
	DL_GR_I_TransferMoney * pTransferMoney=(DL_GR_I_TransferMoney * )pSourceData;
	DL_GR_O_TransferMoney stTransferMoneyResult;///���
	memset(&stTransferMoneyResult,0,sizeof(stTransferMoneyResult));
	if(pTransferMoney->stTransfer.i64Money <= 0) return 0; 

    // PengJiLin, 2010-8-23, �������С�ȫת������

    // PengJiLin, 2011-7-13, 32λ����ת��64λ����
    UINT u32Data = 0;

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord;
        if(1 == pTransferMoney->stTransfer.uHandleCode)
        {
            hRecord=sqlSPSetNameEx(hDatabase,"SP_TransferAllMoney",true);
        }
        else
        {
            hRecord=sqlSPSetNameEx(hDatabase,"SP_TransferMoney",true);
        }
        
		if(hRecord<(HANDLE)0x1000)
			return 0;
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pTransferMoney->stTransfer.UserID);
		sqlSPAddPara(hDatabase,hRecord,"@UseDestID",pTransferMoney->stTransfer.bUseDestID?1:0);
		sqlSPAddPara(hDatabase,hRecord,"@DestUserID",pTransferMoney->stTransfer.destUserID);
		sqlSPAddPara(hDatabase,hRecord,"@DestNickName",pTransferMoney->stTransfer.szDestNickName, sizeof(pTransferMoney->stTransfer.szDestNickName));//����
		//sqlSPAddPara(hDatabase,hRecord,"@DestUser",pTransferMoney->stTransfer.szDestName,sizeof(pTransferMoney->stTransfer.szDestName));
		if(0 == pTransferMoney->stTransfer.uHandleCode)     // PengJiLin, 2010-8-23, ȫת����
           sqlSPAddPara_i64(hDatabase,hRecord,"@TranMoney",pTransferMoney->stTransfer.i64Money);
		sqlSPAddPara(hDatabase,hRecord,"@TranTax",pTransferMoney->stTransfer.bTranTax);
		sqlSPAddPara(hDatabase,hRecord,"@MD5Pass",pTransferMoney->stTransfer.szMD5Pass, sizeof(pTransferMoney->stTransfer.szMD5Pass));//����

		sqlSPExec(hDatabase,hRecord,true);
		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		UINT uResult = DTR_GR_TRANSFER_MONEY_SUC;
		switch(ret)
		{
		case 0: ///<ת�ʳɹ�
			{
			sqlGetValue(hDatabase,hRecord,"DestUserID",pTransferMoney->stTransfer.destUserID);
			sqlGetValue(hDatabase,hRecord,"TranMoney",pTransferMoney->stTransfer.i64Money);
			sqlGetValue(hDatabase,hRecord,"ActualTransfer",pTransferMoney->stTransfer.i64ActualTransfer);
			break;
			}
		case 1:	///<�������
			{
			uResult = DTR_GR_OPEN_WALLET_ERROR_PSW;
			break;
			}
		case 2: ///<������Ǯ���ڶ��ٲ���ת��
			{
			uResult = DTR_GR_TRANSFER_TOTAL_NOT_ENOUGH;
			sqlGetValue(hDatabase,hRecord,"LessAllMoney", pTransferMoney->stTransfer.i64ActualTransfer);
			break;
			}
		case 3: ///<����ת�ʱ�����ڶ���
			uResult = DTR_GR_TRANSFER_TOO_LESS;
			sqlGetValue(hDatabase,hRecord,"LessMoney", pTransferMoney->stTransfer.i64ActualTransfer);
			break;
		case 4: ///<����ת����ĳ����������
			uResult = DTR_GR_TRANSFER_MULTIPLE;
			sqlGetValue(hDatabase,hRecord,"Multiple", u32Data);
            pTransferMoney->stTransfer.i64ActualTransfer = u32Data;
			break;
		case 5: ///<������ֻ�ж���Ǯ��������Ҫת����Ǯ������ת
			uResult = DTR_GR_TRANSFER_NO_ENOUGH_MONEY;
			sqlGetValue(hDatabase,hRecord,"TransMoney", pTransferMoney->stTransfer.i64Money);
			sqlGetValue(hDatabase,hRecord,"BankMoney", pTransferMoney->stTransfer.i64ActualTransfer);
			break;
		case 6: ///<Ŀ���ʻ�������
			uResult = DTR_GR_TRANSFER_MONEY_NO_DEST;
			break;
		case 7:
			uResult = DTR_GR_TRANSFER_TOO_MANY_TIME;
			break;
		default:
			return 0;
		}
		sqlCloseRecord(hDatabase,hRecord);

		::memcpy(&(stTransferMoneyResult.stTransfer),&(pTransferMoney->stTransfer),sizeof(MSG_GR_S_TransferMoney));
		m_pRusultService->OnDataBaseResultEvent(&stTransferMoneyResult.ResultHead,
			 uCode, uResult, sizeof(stTransferMoneyResult), pSourceData->uIndex,pSourceData->dwHandleID);
	}
	catch (...)
	{
		return 0;
	}
	return 0;
}
//��Ǯ��
UINT CGameDataBaseHandle::OnOpenWallet(DataBaseLineHead *pSourceData, UINT uCode)
{
	//Ч������

	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UnRegisterUser)) throw DTR_GR_OPEN_WALLET_ERROR;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_UnRegisterUser * pOpenWallet=(DL_GR_I_UnRegisterUser * )pSourceData;
	DL_GR_O_OpenWalletResult OpenWallet;///�򿪽��
	memset(&OpenWallet,0,sizeof(OpenWallet));

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GetMoney",true);
		if(hRecord<(HANDLE)0x1000)
			return 0;

		sqlSPAddPara(hDatabase,hRecord,"@UserID",pOpenWallet->dwUserID);
	
		sqlSPExec(hDatabase,hRecord,true);
		
		int ret=sqlSPGetReturnValue(hDatabase,hRecord);

		if(ret!=0)
		{
			sqlCloseRecord(hDatabase,hRecord);	// zxj dfw 2009-11-12
			throw DTR_GR_OPEN_WALLET_ERROR;
		}

		//CHAR	szMD5Pass[50];						//��������
		//ZeroMemory(szMD5Pass,sizeof(szMD5Pass));

		//sqlGetValue(hDatabase,hRecord,"TwoPassword",szMD5Pass,sizeof(szMD5Pass));
		sqlGetValue(hDatabase,hRecord,"MoneyInBank",OpenWallet.stUserBank.i64Bank);
		sqlGetValue(hDatabase,hRecord,"TwoPassword",OpenWallet.stUserBank.szTwoPassword,sizeof(OpenWallet.stUserBank.szTwoPassword));
		sqlGetValue(hDatabase,hRecord,"BankVer",OpenWallet.stUserBank.nVer);

		sqlCloseRecord(hDatabase,hRecord);

		//if (lstrcmp(szMD5Pass,pOpenWallet->szMD5Pass)!=0) 
		//	throw DTR_GR_OPEN_WALLET_ERROR_PSW;
		OpenWallet.stUserBank.dwUserID=pOpenWallet->dwUserID;
		throw DTR_GR_OPEN_WALLET_SUC;

	}
	catch (int iHandleResult)
	{TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		m_pRusultService->OnDataBaseResultEvent(&OpenWallet.ResultHead, uCode, iHandleResult,
			sizeof(OpenWallet),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	return 0;
}


UINT CGameDataBaseHandle::OnGMWarningUserRecord(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_Warn_User_Record)) throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_Warn_User_Record * pWarningUser=(DL_GR_I_Warn_User_Record *)pSourceData;//�ṹ��ֵ

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GMWarring",true);
	if(hRecord<(HANDLE)0x1000)
		return 0;

	sqlSPAddPara(hDatabase,hRecord,TEXT("@GMID"),pWarningUser->dwGMID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pWarningUser->dwTargetID);

	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;
}


//����Ա���˶�����¼
UINT CGameDataBaseHandle::OnGMKickUserRecord(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_Kick_User_Record))
		throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_Kick_User_Record * pKickUser=(DL_GR_I_Kick_User_Record *)pSourceData;//�ṹ��ֵ

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GMKickUser",true);
	if(hRecord<(HANDLE)0x1000)
		return 0;

	
	sqlSPAddPara(hDatabase,hRecord,TEXT("@GMID"),pKickUser->GMID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserName"),pKickUser->szTargetUserName,sizeof(pKickUser->szTargetUserName));
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;
}
//����Ȩ���޸�
UINT CGameDataBaseHandle::OnGamePower(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_PowerSet)) throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_PowerSet * pPowerSet=(DL_GR_I_PowerSet *)pSourceData;//�ṹ��ֵ


	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_SetGamePower");
	if(hRecord<(HANDLE)0x1000)
		return 0;

	int nEveryValid=0,nOnlyRoomValid=0;
	if(pPowerSet->bBindKind&0x04)
		nEveryValid=1;
	else if(pPowerSet->bBindKind&0x01)
		nOnlyRoomValid=1;

	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pPowerSet->dwUserID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@GMID"),pPowerSet->dwGMID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@PowerValue"),pPowerSet->dwGamePower);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@PowerTime"),pPowerSet->uLockTime);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@NameID"),m_pKernelInfo->uNameID);

	sqlSPAddPara(hDatabase,hRecord,TEXT("@EveryValid"),nEveryValid);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@OnlyRoomValid"),nOnlyRoomValid);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTable"),m_pInitInfo->szGameTable,sizeof(m_pInitInfo->szGameTable));

	sqlSPExec(hDatabase,hRecord);

	sqlCloseRecord(hDatabase,hRecord);

	int iHandleResult=DTK_GR_GAME_POWER_SUCCESS;
	if(pPowerSet->dwGamePower==0) 
		iHandleResult= DTK_GR_GAME_POWER_RELEASE;


	DataBaseResultLine DT_GamePowerSetResult;
	::memset(&DT_GamePowerSetResult,0,sizeof(DT_GamePowerSetResult));
	m_pRusultService->OnDataBaseResultEvent(&DT_GamePowerSetResult,DTK_GR_GAME_POWER_RESULT,iHandleResult,
		sizeof(DT_GamePowerSetResult),pSourceData->uIndex,pSourceData->dwHandleID);


	return 0;

}
//����GM����
UINT CGameDataBaseHandle::OnCallGM(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_CallGM)) throw DTR_GR_ERROR_UNKNOW;//////���ܵĽṹ�Ͷ�����Ƿ�һ��
	DL_GR_I_CallGM * pCallGM=(DL_GR_I_CallGM *)pSourceData;//�ṹ��ֵ

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_RecordCallGM",true);
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pCallGM->dwUserID);
		sqlSPAddPara(hDatabase,hRecord,"@UserName",pCallGM->szName,sizeof(pCallGM->szName));
		sqlSPAddPara(hDatabase,hRecord,"@RoomID",m_pInitInfo->uRoomID);
		sqlSPAddPara(hDatabase,hRecord,"@NameID",m_pKernelInfo->uNameID);
		sqlSPAddPara(hDatabase,hRecord,"@RoomType",m_pInitInfo->uComType);
		sqlSPAddPara(hDatabase,hRecord,"@DeskNumber",pCallGM->uDeskNO+1);
		sqlSPAddPara(hDatabase,hRecord,"@DeskStation",pCallGM->uDeskStation);
		sqlSPAddPara(hDatabase,hRecord,"@CallType",pCallGM->bCallReason);
		sqlSPAddPara(hDatabase,hRecord,"@CallMsg",pCallGM->szMsg,sizeof(pCallGM->szMsg));

		sqlSPExec(hDatabase,hRecord,false);
		int ret=sqlSPGetReturnValue(hDatabase,hRecord);

		sqlCloseRecord(hDatabase,hRecord);

		if(ret==1)
			throw DTK_GR_CALL_GM_IN_ONE_MI;
		throw DTK_GR_CALL_GM_SUCCESS;
	}
	catch (int iHandleResult)
	{TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		DataBaseResultLine DT_CallGMResult;
		::memset(&DT_CallGMResult,0,sizeof(DT_CallGMResult));
		m_pRusultService->OnDataBaseResultEvent(&DT_CallGMResult,DTK_GR_CALL_GM_RESULT,iHandleResult,
			sizeof(DT_CallGMResult),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	return 0;
}

///
/// ��¼�޸ķ�������
///
/// @param pSourceData
///
/// @ return 0�ɹ�����0

UINT CGameDataBaseHandle::OnRoomPWChangeRecord(DataBaseLineHead * pSourceData)
{
	///< Ч������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GR_RoomPWChange))
		throw DTR_GR_ERROR_UNKNOW;
	///< �ṹ��ֵ
	DL_GR_RoomPWChange * pRoomPW =(DL_GR_RoomPWChange *)pSourceData;

	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord   = sqlSPSetNameEx(hDatabase,"SP_RoomPWChangeRecord",true);

		sqlSPAddPara(hDatabase,hRecord,"@UserID",pRoomPW->dwUserID);
		sqlSPAddPara(hDatabase,hRecord,"@RoomID",pRoomPW->dwRoomID);

		sqlSPExec(hDatabase,hRecord,false);
		sqlSPGetReturnValue(hDatabase,hRecord);

		sqlCloseRecord(hDatabase,hRecord);
	}
	catch (...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}
	return 0;
}

UINT CGameDataBaseHandle::OnUserLeft(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UserLeft)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_UserLeft * pUserLeft=(DL_GR_I_UserLeft *)pSourceData;

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_RecordUserLeft");
		if(hRecord<(HANDLE)0x1000)
			return 0;

		int IsRoomRecord=0;
		UINT uAllCount=pUserLeft->uWinCount+pUserLeft->uLostCount+pUserLeft->uMidCount+pUserLeft->uCutCount;

		if((pUserLeft->dwChangePoint!=0L)||(uAllCount>0L)||(pUserLeft->dwChangeMoney!=0L))
			if ((m_pInitInfo->dwRoomRule|GRR_RECORD_GAME)!=0)
				IsRoomRecord=1;

		//�޸ķ���

		sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pUserLeft->dwUserID);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTable"),m_pInitInfo->szGameTable,sizeof(m_pInitInfo->szGameTable));
		sqlSPAddPara(hDatabase,hRecord,TEXT("@OnlineTimeCount"),pUserLeft->dwOnLineTimeCount);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@UserIsMaster"),pUserLeft->uMaster);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@NameID"),m_pKernelInfo->uNameID);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@RoomID"),m_pInitInfo->uRoomID);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@IsRoomRecord"),IsRoomRecord);

		sqlSPAddPara(hDatabase,hRecord,TEXT("@SrcPoint"),pUserLeft->dwScrPoint);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangePoint"),pUserLeft->dwChangePoint);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangeMoney"),pUserLeft->dwChangeMoney);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangeTax"),pUserLeft->dwChangeTaxCom);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ResultPoint"),pUserLeft->dwResultPoint);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangeWin"),pUserLeft->uWinCount);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangeLost"),pUserLeft->uLostCount);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangeMid"),pUserLeft->uMidCount);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ChangeCut"),pUserLeft->uCutCount);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ClearLogonInfo"),1);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@LockTable"),m_pInitInfo->szLockTable,sizeof(m_pInitInfo->szLockTable));
	
		sqlSPExec(hDatabase,hRecord);

		sqlCloseRecord(hDatabase,hRecord);

		///< ��¼����ʱ��,������
		CBcfFile f(CBcfFile::GetAppPath() + "Function.bcf");
		CString strValue = f.GetKeyVal("OnlineCoin","Available","0");
		if (atoi(strValue))
		{
			hRecord = sqlSPSetNameEx(hDatabase,"SP_RecordOnlineTime");
			if(hRecord<(HANDLE)0x1000)
				return 0;
			sqlSPAddPara(hDatabase,hRecord,"@UserID",pUserLeft->dwUserID);
			sqlSPExec(hDatabase,hRecord);
			sqlSPClose(hDatabase,hRecord);
		}
	}
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}

	return 0;
}
///һ���Ը���ͬ�������û���Ϣ
UINT CGameDataBaseHandle::OnUpdateAllUserResult(DataBaseLineHead * pSourceData)
{
	
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_AllUserData_t)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_AllUserData_t * pUserResult=(DL_GR_I_AllUserData_t *)pSourceData;

	// duanxiaohui 20111103 ���鳡(��ѳ�)�����н�ҡ�������Ϸ��¼
	if (m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		DebugPrintf("SP_UserInfoUpdateAllִ��ʧ�ܷ���ֵ���鳡(��ѳ�)�����н�� m_pInitInfo->dwRoomRule=%d",m_pInitInfo->dwRoomRule);
		delete []pUserResult->pUpdateUserInfo;
		pUserResult->pUpdateUserInfo = NULL;
		return 0;
	}
	// end duanxiaohui

	UpdateUserInfo_t *pUserInfo = pUserResult->pUpdateUserInfo;
	int nUserCount = pUserResult->nUserCount;
	int moneyGame=0;
	if(m_pInitInfo->uComType==TY_MONEY_GAME)
		moneyGame=1;

	


	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord= NULL;
	//����
	if (!sqlExec(hDatabase,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_pDataBaseManage->SQLConnectReset();
		
		DebugPrintf("SP_UserInfoUpdateAll ����");
		hDatabase=m_pDataBaseManage->hDBNative;

	}
	
	CString strParam = "";
	CString strParam1 = "";
	CString strParam2 = "";
	CString strParam3 = "";
	int nUserCount0 = 0;
	int nUserCount1 = 0;
	int nUserCount2 = 0;
	int nUserCount3 = 0;
	TCHAR szTmp[256];
	//__int64 temp = 1000000000000000000;
	int tempUserCount=0;
	CString TempStrParam="";
	//zht2012-03-31 ��������ʱ�����һ��
	for (int i=0; i<nUserCount; i++)
	{
			wsprintf(szTmp, TEXT("%d,%d,%I64d,%I64d,%I64d,%d,%d,%d,%d,")
			, pUserInfo->dwUserID
			, pUserInfo->dwOnLineTimeCount
			, pUserInfo->dwChangePoint
			, pUserInfo->dwChangeMoney
			, pUserInfo->dwChangeTaxCom
			, pUserInfo->uWinCount
			, pUserInfo->uLostCount
			, pUserInfo->uMidCount
			, pUserInfo->uCutCount
			);
		
			strParam += szTmp;
			
		++pUserInfo;	///< ָ��ǰ��
	}

	bool bRet  = FALSE;
	int bHadSendMoney = FALSE; 

	int i = 0;
	//for (int i=0; i<4 && nUserCount>i*50;i++ )
	{

		hRecord=sqlSPSetNameEx(hDatabase,"SP_UserInfoUpdateAll", true);
		if(hRecord<(HANDLE)0x1000)
		{
			DebugPrintf("SP_UserInfoUpdateAll hRecord<(HANDLE)0x1000");
			delete []pUserResult->pUpdateUserInfo;
			pUserResult->pUpdateUserInfo = NULL;
			return 0;
		}
	
		tempUserCount = nUserCount;
		TempStrParam = strParam;
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@UserCount"), tempUserCount );
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@NameID"),m_pKernelInfo->uNameID);
		//��ս������
		char szGameTableTmp[31];
		ZeroMemory(szGameTableTmp, sizeof(szGameTableTmp));
		if (m_pInitInfo->uBattleRoomID > 0)
		{
			//�ǻ�ս����
			memcpy(szGameTableTmp,m_pInitInfo->szBattleGameTable,sizeof(szGameTableTmp));
		}
		else
		{
			//��ͨ����
			memcpy(szGameTableTmp,m_pInitInfo->szGameTable,sizeof(szGameTableTmp));
		}
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTable"),szGameTableTmp,sizeof(szGameTableTmp));
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@PlayTimeCount"),pUserResult->nPlayTime);
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@IsExperciseRoom"),(m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM)?1:0);
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@IsMoneyGame"),(m_pInitInfo->uComType==TY_MONEY_GAME)?1:0);
		bRet = sqlSPAddPara(hDatabase, hRecord, TEXT("@Param"), TempStrParam.GetBuffer(), sizeof(TCHAR) * TempStrParam.GetLength());
	
	
		// PengJiLin, 2011-4-14, ���ӷ���ID
		bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@RoomID"),m_pInitInfo->uRoomID);


		int IsExperciseRoom = (m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM)?1:0;
		CString szSQL;

		szSQL.Format(TEXT("nUserCount = %d EXEC SP_UserInfoUpdateAll @UserCount=%d,@NameID=%d,@GameTable='%s',@PlayTimeCount=%d,@IsExperciseRoom=%d,@IsMoneyGame=%d,@Param='%s',@RoomID=%d"),nUserCount,tempUserCount,m_pKernelInfo->uNameID,szGameTableTmp,pUserResult->nPlayTime,IsExperciseRoom
			,(m_pInitInfo->uComType==TY_MONEY_GAME)?1:0,TempStrParam.GetBuffer(),m_pInitInfo->uRoomID);

		bRet = sqlSPExec(hDatabase,hRecord, true);



		// PengJiLin, 2011-4-14, �Ƿ��������ͽ�ҹ���
		sqlGetValue(hDatabase,hRecord,"SendMoneyFlag",bHadSendMoney);
		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		if (ret != 0 || hRecord == NULL || bRet == FALSE)
		{
			DebugPrintf("SP_UserInfoUpdateAllִ��ʧ�ܷ���ֵ%d hRecord=%d bRet=%d",ret,hRecord,bRet);
			DebugPrintf("SP_UserInfoUpdateAll����%s m_ErrorSQLCount=%d i=%d",szSQL.GetBuffer(),m_ErrorSQLCount,i);


			if (m_ErrorSQLCount < 5)
			{
				sqlCloseRecord(hDatabase,hRecord);

				sqlSPClose(hDatabase, hRecord);

				Sleep(20);
				m_ErrorSQLCount++;
				OnUpdateAllUserResult(pSourceData);
				
				return 0;
			}

			delete []pUserResult->pUpdateUserInfo;
			pUserResult->pUpdateUserInfo = NULL;

			DebugPrintf("SP_UserInfoUpdateAll����%s m_ErrorSQLCount=%d i=%d ��������",szSQL.GetBuffer(),m_ErrorSQLCount,i);


		}

		m_ErrorSQLCount = 0;
		sqlCloseRecord(hDatabase,hRecord);

		sqlSPClose(hDatabase, hRecord);

	}

    // PengJiLin, 2011-4-15, �Ƿ���ʱ���������ͽ��
    if(FALSE != bHadSendMoney || 
		(m_pInitInfo->iTimeLength >= 0 && m_pInitInfo->iSendCount > 0))
    {
        pUserInfo = pUserResult->pUpdateUserInfo;
        DWORD dwUserID = 0;
        for (int i=0; i<nUserCount; ++i)
        {
            // ���ݿ�
            hDatabase=m_pDataBaseManage->hDBNative;
            hRecord=sqlSPSetNameEx(hDatabase,"SP_SendMoney");
            if(hRecord<(HANDLE)0x1000) break;

            dwUserID = pUserInfo->dwUserID;

            DL_GR_O_SendMoney_On_TC stSendMoney;
            memset(&stSendMoney, 0, sizeof(stSendMoney));

            bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@RoomID"), m_pInitInfo->uRoomID );
            bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"), dwUserID );

			__int64 iroundmoney = 0;
			if (m_pInitInfo->iTimeLength >= 0 && m_pInitInfo->iSendCount > 0)
			{
				if (m_pInitInfo->iTimeLength <= pUserResult->iRoundPlayTime && pUserInfo->dwChangePoint > 0)
				{
					iroundmoney = m_pInitInfo->iSendCount;
				}
			}

			bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@RoundSendMoney"), iroundmoney);
			bRet = sqlSPAddPara(hDatabase,hRecord,TEXT("@IsRoundSend"), bHadSendMoney?0:1);

			++pUserInfo;

            sqlSPExec(hDatabase,hRecord, true);

			if (bHadSendMoney)
			{
				stSendMoney.dwUserID = dwUserID;
				// �����Ľ����
				sqlGetValue(hDatabase,hRecord,"SendMoney",stSendMoney.dwGetMoney);
				// ���ö���ʱ�����͵Ľ��
				sqlGetValue(hDatabase,hRecord,"PlayTimesMoney",stSendMoney.dwMoneyOnTimes);
				// ���ö��پ����͵Ľ��
				sqlGetValue(hDatabase,hRecord,"PlayCountsMoney",stSendMoney.dwMoneyOnCounts);
				// ��һ�β����ʱ��
				sqlGetValue(hDatabase,hRecord,"PlayTimes_Dist",stSendMoney.dwTimesNeed);
				// ��һ�β���پ�
				sqlGetValue(hDatabase,hRecord,"PlayCounts_Dist",stSendMoney.dwCountsNeed);

				// test
				CString strTemp = "";
				strTemp.Format("PengJiLin, Serve User[%d], Money[%d], PTM[%d], PCM[%d], PTD[%d], PCD[%d]",
								stSendMoney.dwUserID,
								stSendMoney.dwGetMoney,
								stSendMoney.dwMoneyOnTimes,
								stSendMoney.dwMoneyOnCounts,
								stSendMoney.dwTimesNeed,
								stSendMoney.dwCountsNeed);
         

				m_pRusultService->OnDataBaseResultEvent(&stSendMoney.ResultHead,DTK_GR_SEND_MONEY_ON_TIMES_COUNTS,
														0,sizeof(stSendMoney),0,0);
			}
			sqlSPClose(hDatabase, hRecord);
        }
    }
	else
	{
		sqlSPClose(hDatabase, hRecord);
	}

	delete []pUserResult->pUpdateUserInfo;
	pUserResult->pUpdateUserInfo = NULL;

	OutputDebugString("rqf OnUpdateAllUserResult����");

	return 0;
}


///�����û�������Ϣ
UINT CGameDataBaseHandle::OnUpdateContestInfo(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UserContestData)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_UserContestData* _p=(DL_GR_I_UserContestData *)pSourceData;

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetName(hDatabase,"SP_ContestUpdateRecord");
		if(hRecord<(HANDLE)0x1000)
			return 0;

		//�޸ķ���
		sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"), _p->iUserID);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ContestID"), _p->iContestID);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ContestScore"), _p->ChangeScore);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ContestPoint"), _p->ChangePoint);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@DeskNo"), _p->iDeskNo);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@SitNO"), _p->iSitNo);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@RoomID"), _p->iRoomID);

		sqlSPExec(hDatabase,hRecord);
		sqlCloseRecord(hDatabase,hRecord);
		sqlSPClose(hDatabase,hRecord);

	}
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}

	return 0;
}

///�õ��û�������Ϣ
UINT CGameDataBaseHandle::OnGetContestInfo(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UserContestRank)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_UserContestRank* _p = (DL_GR_I_UserContestRank*)pSourceData;

	if (_p->iIDNums == 0 || !_p->pUserIDs)
	{
		return 0;
	}

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetName(hDatabase,"SP_Contest_GetSomebodyRank");
		if(hRecord<(HANDLE)0x1000)
			return 0;

		CString strIDs;
		for (int i = 0; i < _p->iIDNums; ++i)
		{
			if (_p->pUserIDs[i] > 0)
			{
				strIDs.Format("%s%d,",strIDs,_p->pUserIDs[i]);
			}
		}

		delete[] _p->pUserIDs;
		_p->pUserIDs = NULL;

		//��ȡ����
		sqlSPAddPara(hDatabase,hRecord,TEXT("@ContestID"),m_pInitInfo->iContestID);
		sqlSPAddPara(hDatabase,hRecord,TEXT("@UserIDs"),strIDs.GetBuffer(),strIDs.GetLength());

		sqlSPExec(hDatabase,hRecord,true);

		DL_GR_O_UserContestRank _out;

		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(DL_GR_O_UserContestRank));

			sqlGetValue(hDatabase,hRecord,"UserID",_out._data.dwUserID);
			sqlGetValue(hDatabase,hRecord,"RankNum",_out._data.iRankNum);

			if (_out._data.iRankNum > 0)
			{
				m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GR_GET_CONTEST_RESULT,0,sizeof(DL_GR_O_UserContestRank),pSourceData->uIndex,pSourceData->dwHandleID);
			}
			
			sqlMoveNext(hDatabase,hRecord);
		}

		sqlCloseRecord(hDatabase,hRecord);
		sqlSPClose(hDatabase,hRecord);

	}
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}

	return 0;
}


UINT CGameDataBaseHandle::OnUpdateUserResult(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UserLeft)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_UserLeft * pUserResult=(DL_GR_I_UserLeft *)pSourceData;

	// duanxiaohui 20111103 ���鳡(��ѳ�)�����н�ҡ�������Ϸ��¼
	if (m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM || m_pInitInfo->dwRoomRule & GRR_CONTEST)
	{
		return 0;
	}
	// end duanxiaohui
	
	int moneyGame=0;
	if(m_pInitInfo->uComType==TY_MONEY_GAME)
		moneyGame=1;

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_UserInfoUpdate");
	if(hRecord<(HANDLE)0x1000)
		return 0;
	//DebugPrintf("dwUserID=[%d],Point = [%d]ChangeMoney = [%d]",
	//	pUserResult->dwUserID,
	//	pUserResult->dwChangePoint,
	//	pUserResult->dwChangeMoney
	//	);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pUserResult->dwUserID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@NameID"),m_pKernelInfo->uNameID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@OnlineTime"),pUserResult->dwOnLineTimeCount);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@IsMoneyGame"),moneyGame);
	//wushuqun 2009.6.5
	//��ս������
	char szGameTableTmp[31];
	memset(szGameTableTmp,0,sizeof(szGameTableTmp));
	if (m_pInitInfo->uBattleRoomID > 0)
	{
		//�ǻ�ս����
		memcpy(szGameTableTmp,m_pInitInfo->szBattleGameTable,sizeof(szGameTableTmp));
	}
	else
	{
		//��ͨ����
		memcpy(szGameTableTmp,m_pInitInfo->szGameTable,sizeof(szGameTableTmp));
	}
    sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTable"),szGameTableTmp,sizeof(szGameTableTmp));
	///////////////////////////////////

	//sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTable"),m_pInitInfo->szGameTable,sizeof(m_pInitInfo->szGameTable));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Point"),pUserResult->dwChangePoint);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Money"),pUserResult->dwChangeMoney);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@TaxCom"),pUserResult->dwChangeTaxCom);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Win"),pUserResult->uWinCount);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Lost"),pUserResult->uLostCount);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Mid"),pUserResult->uMidCount);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Cut"),pUserResult->uCutCount);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@PlayTimeCount"),pUserResult->dwPlayTimeCount);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@IsExperciseRoom"),(m_pInitInfo->dwRoomRule & GRR_EXPERCISE_ROOM || m_pInitInfo->dwRoomRule & GRR_CONTEST)?1:0);
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);
	return 0;
}


//��ʱ���·����ͽ��
UINT CGameDataBaseHandle::OnModifyUserMoney(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_Update_InstantMoney)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_Update_InstantMoney * pUserResult = (DL_GR_Update_InstantMoney *)pSourceData;

	int cut=0;
	if(pUserResult->bCutValue)
		cut=1;

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_ChangeMoneyInTime");
	if(hRecord<(HANDLE)0x1000)
		return 0;

	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pUserResult->dwUserID);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@GameTable"),m_pInitInfo->szGameTable,sizeof(m_pInitInfo->szGameTable));
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Wallet"),pUserResult->dwChangeMoney);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Point"),pUserResult->dwChangePoint);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@Cut"),cut);

	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;
}

//��������ֵ
UINT CGameDataBaseHandle::OnUpdateUserCharm(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_AddCharm)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_AddCharm * pCharm = (DL_GR_I_AddCharm *)pSourceData;

	TCHAR szSQL[1000];
	if(pCharm->iCharmValue != 0)
	{
		wsprintf(szSQL,TEXT("UPDATE TUserInfo SET Fascination=Fascination+%d WHERE UserID=%ld"),pCharm->iCharmValue,pCharm->dwUserID);
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		sqlExec(hDatabase,szSQL);
	}
	return 0;
}


UINT CGameDataBaseHandle::OnUseSmallBoardcast(DataBaseLineHead * pSourceData)
{
	PROP_GPR_I_BOARDCAST *iBoard=(PROP_GPR_I_BOARDCAST*)pSourceData;
	PROP_GPR_O_BOARDCAST oBoard;
	memcpy(&oBoard.boardCast,&iBoard->boardCast,sizeof(_TAG_BOARDCAST));

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_UsePropForSelf",true);
	if(hRecord<(HANDLE)0x1000)
		return 0;

	sqlSPAddPara(hDatabase,hRecord,"@UserID",iBoard->boardCast.dwUserID);
	sqlSPAddPara(hDatabase,hRecord,"@PropID",iBoard->boardCast.nPropID);
	sqlSPAddPara(hDatabase,hRecord,"@PropCount",1);

	sqlSPExec(hDatabase,hRecord,false);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);

	sqlCloseRecord(hDatabase,hRecord);

	int iHandleResult=DTK_GR_PROP_USE_SUCCEED;
	if(ret!=0)
		iHandleResult=DTR_GR_PROP_USE_ERR_NOPROP;

	m_pRusultService->OnDataBaseResultEvent(&oBoard.ResultHead,DTK_GPR_PROP_SMALL_BRD,iHandleResult,sizeof(PROP_GPR_O_BOARDCAST),pSourceData->uIndex,pSourceData->dwHandleID);

	return 0;
}

UINT CGameDataBaseHandle::OnSendUserMoenyByTimes(DataBaseLineHead *pSourceData)
{
	//QR

	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_SendMonet_By_Times)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_SendMonet_By_Times * pSendMoeny=(DL_GR_I_SendMonet_By_Times *)pSourceData;

	DL_GR_O_SendMonet_By_Times sSendMoenyResult;
	memset(&sSendMoenyResult,0,sizeof(sSendMoenyResult));

	//comroominfo���й��ڶ��ͽ�ҵ�����
	int iPerMoney = 0;       //һ���Ͷ���
	int iDayMoneyTimes = 0;  //һ���Ͷ��ٴ�
	//int iDayLessMoney = 0;   //������ڶ��پ���
	//ComRoomInfo�н�ҷ��������
	sSendMoenyResult.dwUserID = pSendMoeny->dwUserID;
	sSendMoenyResult.i64SrcGameMoney = pSendMoeny->i64SrcGameMoney;
	sSendMoenyResult.i64SrcMoneyInBank = pSendMoeny->i64SrcMoneyInBank;
	sSendMoenyResult.dwAllSendTImes = 0;
	sSendMoenyResult.dwSendFailedCause = 0;
	sSendMoenyResult.dwSendGameMoney = 0;
	sSendMoenyResult.dwSendTimes = pSendMoeny->dwGameFinishCount;

	int FinishCount = pSendMoeny->dwGameFinishCount;//��õ�ǰ�Ƕ��ٴ���Ϸ��

	HANDLE hDatabase=m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_SendMoney",true);
	if(hRecord<(HANDLE)0x1000)
		return 0;


	sqlSPAddPara(hDatabase,hRecord,"@RoomID",m_pInitInfo->uRoomID);
	sqlSPAddPara(hDatabase,hRecord,"@UserID",pSendMoeny->dwUserID);

	sqlSPExec(hDatabase,hRecord,true);

	sSendMoenyResult.dwSendFailedCause=sqlSPGetReturnValue(hDatabase,hRecord);

	if(sSendMoenyResult.dwSendFailedCause==0)
	{
		sqlGetValue(hDatabase,hRecord,"TMoney",sSendMoenyResult.dwSendGameMoney);
		sqlGetValue(hDatabase,hRecord,"DaySendTimes",sSendMoenyResult.dwSendTimes);
		sqlGetValue(hDatabase,hRecord,"DayMoneyTimes",sSendMoenyResult.dwAllSendTImes);
	}
	CString str;
	str.Format("һ���ʹ��� %d�����ʹ��� %d",sSendMoenyResult.dwAllSendTImes,sSendMoenyResult.dwSendTimes);
	::OutputDebugString(str);

	sqlCloseRecord(hDatabase,hRecord);


	m_pRusultService->OnDataBaseResultEvent(&sSendMoenyResult.ResultHead,DTK_GR_SENDMONEY_TIMES,sSendMoenyResult.dwSendFailedCause,
		sizeof(sSendMoenyResult),pSourceData->uIndex,pSourceData->dwHandleID);
	return 0;
}
//���ʹ�õ���
UINT CGameDataBaseHandle::OnUserUseOneProp(DataBaseLineHead *pSourceData)
{

	//////////////////////////////////////////////////////////////////////////
	//Ч������
	//2008-08-11
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GR_I_USEPROP))
		throw DTR_GR_PROP_USE_ERR_PARAM_INVALID;//////���ܵĽṹ�Ͷ�����Ƿ�һ��

	DL_GR_I_USEPROP	*useIProp = (DL_GR_I_USEPROP * )pSourceData;
	DL_GR_O_USEPROP useOProp;
	ZeroMemory(&useOProp,sizeof(DL_GR_O_USEPROP));
	memcpy(&useOProp.usingProp,&useIProp->usingProp,sizeof(_TAG_USINGPROP));

	HANDLE hDatabase = m_pDataBaseManage->hDBNative;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_UseProp",true);
	if(hRecord<(HANDLE)0x1000)
		return 0;

	//ȡֵ����
	int attribValue=useIProp->usingProp.nPropValueAttrib;
	//��������
	int attribAction=useIProp->usingProp.nPropActionAttrib;
	//���¼���������Ҫ����ʱ��
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curTime=atoi(stime);

	//Ӱ��ʱ���ʱ��ֵ������Ϊ��λ
	int		addSecond=attribValue & 0xFFFF;
	addSecond*=3600;//��������СʱΪ��λ�ģ�����Ҫ�ٳ���60*60=3600��
	//˫������
	if(attribAction & 0x4)
	{
		if(curTime>useOProp.usingProp.iDoubleTime)
			//�����ǰ��˫������ʱ���ѹ��ڣ���ֱ�������ڵĿ�ʼʱ���ϼ�����Ӧ��ʱ��
			useOProp.usingProp.iDoubleTime=curTime+addSecond;
		else
			//�����ǰ��˫������ʱ�仹δ���ڣ�����ԭ�еĵ���ʱ���ϼ�����Ӧ��ʱ��
			useOProp.usingProp.iDoubleTime+=addSecond;
	}
	//��������ߣ����˲��۷�
	if(attribAction & 0x8)
	{
		if(curTime>useOProp.usingProp.iProtectTime)
			//�����ǰ�Ļ����ʱ���ѹ��ڣ���ֱ�������ڵĿ�ʼʱ���ϼ�����Ӧ��ʱ��
			useOProp.usingProp.iProtectTime=curTime+addSecond;
		else
			//�����ǰ�Ļ����ʱ�仹δ���ڣ�����ԭ�еĵ���ʱ���ϼ�����Ӧ��ʱ��
			useOProp.usingProp.iProtectTime+=addSecond;
	}
	//����ֵ�仯
	int fasciChange=0;
	if(attribAction & 0x10)
	{
		//����ֵ�仯��
		fasciChange=(attribValue & 0xFF0000)>>16;
		//��������ֵ���Ǽ�������ֵ
		if(attribAction & 0x20)
			fasciChange=-fasciChange;
	}

	sqlSPAddPara(hDatabase,hRecord,"@UserID",useIProp->usingProp.dwUserID);
	sqlSPAddPara(hDatabase,hRecord,"@PropID",useIProp->usingProp.nPropID);
	sqlSPAddPara(hDatabase,hRecord,"@TargetID",useIProp->usingProp.dwTargetUserID);
	//���NameID�����ڸ�������
	sqlSPAddPara(hDatabase,hRecord,"@GameNameID",m_pKernelInfo->uNameID);
	sqlSPAddPara(hDatabase,hRecord,"@GameTable",m_pInitInfo->szGameTable,sizeof(m_pInitInfo->szGameTable));
	//�Ƿ񸺷�����
	sqlSPAddPara(hDatabase,hRecord,"@IsClearMinus", (attribAction & 0x2)?1:0);
	//�Ƿ��С���� // PengJiLin, 2010-6-1, ���� ������
	sqlSPAddPara(hDatabase,hRecord,"@IsBoard",(attribAction & 0x80 || attribAction & 0x100 || attribAction & 0x200)?1:0);
	//˫�����ֱ仯
	sqlSPAddPara(hDatabase,hRecord,"@DoubleTime",useOProp.usingProp.iDoubleTime);
	//������仯
	sqlSPAddPara(hDatabase,hRecord,"@ProtectTime",useOProp.usingProp.iProtectTime);
	//����ֵ�仯
	sqlSPAddPara(hDatabase,hRecord,"@FasciChange",fasciChange);
	sqlSPExec(hDatabase,hRecord,false);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);

	sqlCloseRecord(hDatabase,hRecord);


	int iHandleResult=DTK_GR_PROP_USE_SUCCEED;
	if(ret!=0)
		iHandleResult=DTR_GR_PROP_USE_ERR_NOPROP;

	m_pRusultService->OnDataBaseResultEvent(&useOProp.ResultHead,DTK_GR_PROP_USE,iHandleResult,sizeof(DL_GR_O_USEPROP),pSourceData->uIndex,pSourceData->dwHandleID);

	return 0;

}
//////////////////////////////
//20090203 Kylin ��ֹ�������ݿ���û���½ʧ��
//�����û�����
UINT CGameDataBaseHandle::OnUnRegisterOnLine(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_UnRegisterUser)) throw DTR_GR_ERROR_UNKNOW;
	DL_GR_I_UnRegisterUser * pUnRegisterUser=(DL_GR_I_UnRegisterUser *)pSourceData;

	//��������
	try	{ UnRegisterUserLogon(pUnRegisterUser->dwUserID,false); }
	catch (...) {}

	return 0;
}

//wushuqun 2009.5.22
//�޸Ľ�ֹ��¼���⣬�޸Ĳ���TWLoginRecord ��¼ʱ��
//ִ��ʱ������¼�����������Ϸ�����б�ִ��
UINT CGameDataBaseHandle::OnWriteTWLoginRecord(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_WriteWLoginRecord))
	{
		throw DTR_GR_ERROR_UNKNOW;
	}
	DL_GR_I_WriteWLoginRecord * pDlWriteWLogin = (DL_GR_I_WriteWLoginRecord *)pSourceData;
	//��������
	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_InsertWLoginRecord");
		if(hRecord>(HANDLE)0x1000)
		{
			sqlSPAddPara(hDatabase,hRecord,"@UserID", pDlWriteWLogin->dwUserID);
			sqlSPAddPara(hDatabase,hRecord,"@RoomID", pDlWriteWLogin->uRoomID);
			sqlSPExec(hDatabase,hRecord);
			sqlCloseRecord(hDatabase,hRecord);
		}
	}
	catch (...) {}

	return 0;
}

//wushuqun 2009.6.6
//��ʱ��������
//�����и÷���������������
UINT CGameDataBaseHandle::OnAloneDeskInTime(DataBaseLineHead * pSourceData)
{
	//��������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GR_I_ALone_Data))
	{
		throw DTR_GR_ERROR_UNKNOW;;
	}
	DL_GR_I_ALone_Data * pAloneDeskData = (DL_GR_I_ALone_Data * )pSourceData;
	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_AloneDeskInTime",true);
		if(hRecord>(HANDLE)0x1000)
		{
			sqlSPAddPara(hDatabase,hRecord,"@RoomID", pAloneDeskData->uRoomID);
			sqlSPAddPara(hDatabase,hRecord,"@RoomVirtualPeople", pAloneDeskData->bDeskPeople);
			sqlSPExec(hDatabase,hRecord);
			int bRet = ::sqlSPGetReturnValue(hDatabase,hRecord);
			//SP_AloneDeskInTime �洢���̷���ֵ˵��
			// 0 :��ӷ���������ʧ��
			// 1 :��ӷ��������˳ɹ�
			if (bRet == 0)
			{
				CString str;
				str.Format("����ID: %d,�����������ʧ��",pAloneDeskData->uRoomID);
				::OutputDebugString(str);
			}
			if (bRet == 1)
			{
				//������������ɹ�
			}
			sqlCloseRecord(hDatabase,hRecord);
		}
	}
	catch(...)
	{
	}
	return 0;
}

//ɾ����½��Ϣ
bool CGameDataBaseHandle::UnRegisterUserLogon(long int dwUserID, bool bAllLogonInfo)
{
	if (m_pInitInfo->szLockTable[0]!=0)
	{
		//ɾ������
		//TCHAR szSQL[1000];
		//if (bAllLogonInfo==true)
		//	wsprintf(szSQL,TEXT("DELETE from %s WHERE RoomID=%ld"),m_pInitInfo->szLockTable[0],m_pInitInfo->uRoomID);
		//else 
		//	wsprintf(szSQL,TEXT("DELETE from %s WHERE UserID=%ld AND RoomID=%ld"),m_pInitInfo->szLockTable,dwUserID,m_pInitInfo->uRoomID);
		//if (m_pDataBaseManage->m_DataBaseLogon.Execute(szSQL)==false)
		//{
		//	m_pDataBaseManage->m_DataBaseNative.ShowError(true);
		//}
		//////////////////////////////
		//20090203 Kylin ��ֹ�������ݿ���û���½ʧ��
		HANDLE hDatabase=m_pDataBaseManage->hDBNative;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_SetUserOnlineFlag");
		if(hRecord<(HANDLE)0x1000)
			return false;
		sqlSPAddPara(hDatabase,hRecord,"@UserID",dwUserID);
		///////////////////////////////////////////////
		///Kylin 20090201 ��ֹ�˺�ͬʱ��½
		sqlSPAddPara(hDatabase,hRecord,"@FlagID",-2);
		///////////////////////////////////////////////
		sqlSPExec(hDatabase,hRecord);
		sqlSPClose(hDatabase,hRecord);

		//////////////////////////////

	}

	return true;
}

HANDLE CGameDataBaseHandle::sqlSPSetNameEx(HANDLE & hHandle,LPCTSTR szSPName,bool bReturnValue)
{

	bool bIsDBNative = false;;
	if(hHandle == m_pDataBaseManage->hDBNative)
	{
		bIsDBNative = true;
	}

	if (bIsDBNative)
	{
		hHandle = m_pDataBaseManage->hDBNative;
	}
	else
	{
		hHandle = m_pDataBaseManage->hDBLogon;
	}

	if (!sqlExec(hHandle,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_pDataBaseManage->SQLConnectReset();

	}

	return  sqlSPSetName(hHandle,szSPName,bReturnValue);

}




//////////////////////////////
/*******************************************************************************************************/

