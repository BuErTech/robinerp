#include "StdAfx.h"
#include "MainServer.h"
#include "GameLogonManage.h"
#include "CommonUse.h"
#include "PropDataBaseMessage.h"

#include "GM_MessageDefine.h"

//#include "GameRoomLogonDT.h"

///extern void DebugPrintf(const char *p, ...);
static void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\ZServer.txt");
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start( arg, p );
	fprintf(fp,"[%s]--", CTime::GetCurrentTime().Format("%H:%M:%S"));
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}

/// ������������ĺ궨��//
#define CHECK_ERROR_BEGIN \
	int iErrorCode = 0;  \
	do{

#define REPORT_ERROR {iErrorCode = __LINE__; break;}

#define CHECK_ERROR_END \
		iErrorCode = 0;\
	}while(false);	\
	if (iErrorCode!=0)\
	{ DebugPrintf("��%d�г���", iErrorCode); }
/// ������������ĺ궨��//


//�궨��
#define IDT_CHECK_DATA_CONNECT						2					//����������Ӷ�ʱ��
#define IDT_GET_ROOM_LIST_PEOCOUNT					3
#define IDT_GET_MAIL_LIST							4			

#define IDT_CHECKRECONNECT						5
#define TIMEOUT_CHECKRECONNECT						1
#define IDT_CHECKKEEPLIVE						6
#define TIMEOUT_CHECKKEEPLIVE						15

#define TIMEOUT_GET_ROOM_LIST_PEOCOUNT				6000L
#define TIMEOUT_GET_MAIL_LIST						1

#define IDT_GET_LOGON_PEOPLE_COUNT					7
#define TIMEOUT_GET_LOGON_PEOPLE					5

const char *szWalletMoney = "Walletmoney";
const char *szTChangeRecord = "TChangeRecord";
const char *szUpdate = "Update";
const char *szDelete_ = "delete";
const char *szAgency = "AgencyID";
const char *szSet = "set";
const char *szTUserInfo = "TUserInfo";
const char *szTUsers = "TUsers";
const char *szBankMoney = "BankMoney";
const char *szPass = "Pass";
const char *szWeb_Users = "Web_Users";
/***************************************************************################################***********************************/

// PengJiLin, 2010-6-3, Ÿ�� DLL �ļ�ģ�顢����ָ��
static HINSTANCE g_hOpetvDll = NULL;
static SET_LICENCE_FUNC_PTR g_lpfn_SetLicence = NULL;
static INIT_FUNC_PTR   g_lpfn_Init = NULL;
static RELEASE_FUNC_PTR g_lpfn_Reslease = NULL;
static LOGIN_FUNC_PTR  g_lpfn_Login = NULL;

// �ݴ��û���������
static char g_chUserName[65] = {0};
static char g_chMD5Pass[65] = {0};

CGameLogonManage* g_pGameLogonManage = NULL;

// ��Ϣ����������
CRITICAL_SECTION g_CriticalSection;


//SOCKET ���ݶ�ȡ//////////////CrAckIT
//bool CGameLogonManage::OnSocketRead(����ͷ, ������, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
bool CGameLogonManage::OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if(pNetHead->bMainID == MDM_GR_MONEY)  // ת�� add by xqm 2010-11-9
	{
		if (sizeof(MSG_GR_S_TransferMoney) == uSize) 
		{
			MSG_GR_S_TransferMoney * pTransfer = (MSG_GR_S_TransferMoney *)pData;		
			bool bRet =	IsUserAlreadyLogon(pTransfer->destUserID);
			int iSocket = 0;
			if (bRet)
			{
				iSocket = GetIndexByID(pTransfer->destUserID);	
				if (iSocket <= 0)
					return true;
				return m_TCPSocket.SendData(iSocket, pData, uSize, MDM_GP_NOTICE_MSG, ASS_GR_TRANSFER_MONEY, ASS_GR_TRANSFER_SUC, 0); //������ʾ��Ϣ����ת���ˡ�
			}
			return true;
		}
	}
	/// {{Added by zxd 20100722 GM��Ϣ�������
	if (MDM_GAMEMASTER_MESSAGE == pNetHead->bMainID)
	{
		/// �������GM����ֱ�ӷ��ش���
		if ((!IsGameMaster(uIndex)) && (pNetHead->bAssistantID != ASS_GM_MESSAGE_GM_LOGON))
		{
			return false;
		}
		return m_pGmMessageBase->OnSocketMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
	}
	/// Added by zxd 20100722}}
	/// {{Added by zxd 20100805 ��ȡ����GM�б�
	if (MDM_GP_PLAYER_2_GM == pNetHead->bMainID)
	{
		OnPlayer2Gm(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		return true;
	}
	/// Added by zxd 20100805}}


	//wushuqun 2009.6.9
	if ((MDM_GP_MESSAGE == pNetHead->bMainID) && (ASS_GP_NEWS_SYSMSG ==pNetHead->bAssistantID))
	{
		// ����˲�У�����ݣ�����ֱ��ת��
		// ����MDM_GP_SYSMSG ��ʾȫ�ֵĴ�����Ϣ
		m_TCPSocket.SendDataBatch(pData,uSize,MDM_GP_MESSAGE, MDM_GP_SYSMSG,0);
		return true;
	}

	if (pNetHead->bMainID==MDM_GP_LOGON)		//�û���½��������ͷ������ʶΪMDM_GP_LOGON
	{
		if (pNetHead->bAssistantID==ASS_GP_LOGON_BY_NAME)//������ʶ
		{
			//Ч������
			if (uSize!=sizeof(MSG_GP_S_LogonByNameStruct)) return false;///��֤�ṹ�Ƿ�һ��

			//�����½
			MSG_GP_S_LogonByNameStruct * pLogonInfo=(MSG_GP_S_LogonByNameStruct *)pData;
			pLogonInfo->szName[sizeof(pLogonInfo->szName)/sizeof(pLogonInfo->szName[0])-1]=0;
			pLogonInfo->TML_SN[sizeof(pLogonInfo->TML_SN)/sizeof(pLogonInfo->TML_SN[0])-1]=0;
			pLogonInfo->szMD5Pass[sizeof(pLogonInfo->szMD5Pass)/sizeof(pLogonInfo->szMD5Pass[0])-1]=0;
			//�������
			DL_GP_I_LogonByNameStruct DL_Logon;
			DL_Logon.LogonInfo=*pLogonInfo;//////��ֵ���ѵ�½�û����붼����ȥ
			////////////////////////////////////
				////Kylin 20090316��ӷ�ֹ�˻�ͬʱ��½

				DL_Logon.ID=m_ID;
				////////////////////////////////////

			CTCPSocketManage::ULongToIP(uAccessIP,DL_Logon.szAccessIP);//��IPŪ����

			m_SQLDataManage.PushLine(&DL_Logon.DataBaseHead,sizeof(DL_Logon),DTK_GP_LOGON_BY_NAME,uIndex,dwHandleID);//�ύ���ݿ�

			// wyl, 2011-5-11, ��ȡ������Ϸ�б�
			if (m_bSendSellGameList)
			{
				m_SQLDataManage.PushLine(&DL_Logon.DataBaseHead,sizeof(DL_Logon),DTK_GP_GET_SELLGAMELIST,uIndex,dwHandleID);
			}
			

			return true;
		}
		else if (ASS_GP_LOGON_ERROR==pNetHead->bAssistantID)
		{
			if (uSize!=sizeof(MSG_GR_S_CheckMoney)) return false;
			MSG_GR_S_CheckMoney * pCheckVersion=(MSG_GR_S_CheckMoney *)pData;
			DL_GR_I_CheckVersion_t cv;
			cv.uType = pCheckVersion->i64Count;
			m_SQLDataManage.PushLine(&cv.DataBaseHead,sizeof(cv),DTK_GR_CHECK_VERSION,uIndex,dwHandleID);
			return true;
		}
        else if(ASS_GP_LOGON_BY_SOFTWARE == pNetHead->bAssistantID) // PengJiLin, 2010-6-2, ����������֪ͨ��(Ÿ��)
        {
            m_bRunBySoftware = TRUE;
            return true;
        }
	}
	else if(pNetHead->bMainID == MDM_GP_LOCK_ACCOUNT )	//�������� zxj 2009-11-13
	{
		MSG_GP_S_LockAccount * pLockAccount = (MSG_GP_S_LockAccount*)pData;
		if( pLockAccount == NULL )
			return TRUE;
		DL_GP_LockAccount DataLockAccount;
		::ZeroMemory(&DataLockAccount, sizeof(DataLockAccount));
		DataLockAccount.LockAccount = *pLockAccount;
		m_SQLDataManage.PushLine(&DataLockAccount.DataBaseHead, sizeof(DataLockAccount), DTK_GP_LOCK_UNLOCK_ACCOUNT, uIndex, dwHandleID);
		return TRUE;
	}

	else if(pNetHead->bMainID == MDM_GP_BIND_MOBILE )	//�û���/ȡ�����ֻ�
	{
		MSG_GP_S_BindMobile * pBindMobile = (MSG_GP_S_BindMobile*)pData;
		if( pBindMobile == NULL )
			return TRUE;
		DL_GP_BindMobile _in;
		::ZeroMemory(&_in, sizeof(DL_GP_BindMobile));
		_in.data = *pBindMobile;
		m_SQLDataManage.PushLine(&_in.DataBaseHead, sizeof(DL_GP_BindMobile), DTK_GP_BIND_MOBILE, uIndex, dwHandleID);
		return TRUE;
	}

	else if (pNetHead->bMainID==MDM_GP_LIST)	//�����б�
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_GP_LIST_KIND:	//������Ϸ�����б�
			{
				//��ȡ��������
				bool bFinish=false;
				INT_PTR uBeginPos=0;
				char bBuffer[MAX_SEND_SIZE];
				do
				{
					INT_PTR uCopyPos=m_GameList.FillGameKind(bBuffer,sizeof(bBuffer),uBeginPos,bFinish);
					uBeginPos+=uCopyPos;
					if (bFinish)
						m_TCPSocket.SendData(uIndex,bBuffer,(UINT)uCopyPos*sizeof(ComKindInfo),MDM_GP_LIST,ASS_GP_LIST_KIND,ERR_GP_LIST_FINISH,dwHandleID);//������Ϸ�����б������־
					else	
						m_TCPSocket.SendData(uIndex,bBuffer,(UINT)uCopyPos*sizeof(ComKindInfo),MDM_GP_LIST,ASS_GP_LIST_KIND,ERR_GP_LIST_PART,dwHandleID);
				} while (bFinish==false);

				//��ȡ��������
				uBeginPos=0;
				bFinish=false;
				do
				{
					INT_PTR uCopyPos=m_GameList.FillGameName(bBuffer,sizeof(bBuffer),uBeginPos,bFinish);
					uBeginPos+=uCopyPos;
					if (bFinish) 
						m_TCPSocket.SendData(uIndex,bBuffer,(UINT)uCopyPos*sizeof(ComNameInfo),MDM_GP_LIST,ASS_GP_LIST_NAME,ERR_GP_LIST_FINISH,dwHandleID);
					else 
						m_TCPSocket.SendData(uIndex,bBuffer,(UINT)uCopyPos*sizeof(ComNameInfo),MDM_GP_LIST,ASS_GP_LIST_NAME,ERR_GP_LIST_PART,dwHandleID);
				} while (bFinish==false);

				//wushuqun 2009.5.6
				//����дZ��־ʱ��
				LPUSER pUser = NULL;
				for(int i=0; i<m_UserList.GetCount(); i++)
				{
					pUser = (LPUSER)m_UserList.GetAt(i);
					if(pUser == NULL)
						continue;
					if(pUser->iSocketIndex == uIndex )
					{
						DL_GP_WriteZRecord WriteZRecord;
						memset(&WriteZRecord, 0, sizeof(WriteZRecord));
						WriteZRecord.lUserID = pUser->UserID;
						WriteZRecord.ZID = m_ID;
						m_SQLDataManage.PushLine(&WriteZRecord.DataBaseHead,sizeof(WriteZRecord),DTK_GP_ZRECORD,uIndex,dwHandleID);
					}
				}
				return true;
			}
		case ASS_GP_LIST_ROOM:	//��ȡ�����б�
			{
				//Ч������
				if (uSize!=sizeof(MSG_GP_SR_GetRoomStruct)) return false;
				MSG_GP_SR_GetRoomStruct * pGetRoomList=(MSG_GP_SR_GetRoomStruct *)pData;

				//��ȡ��������
				bool bFinish=false;
				INT_PTR uBeginPos=0;
				char bBuffer[MAX_SEND_SIZE];
				((MSG_GP_SR_GetRoomStruct *)bBuffer)->uKindID=pGetRoomList->uKindID;
				((MSG_GP_SR_GetRoomStruct *)bBuffer)->uNameID=pGetRoomList->uNameID;
				do
				{
					INT_PTR uCopyPos=m_GameList.FillGameRoom(bBuffer+sizeof(MSG_GP_SR_GetRoomStruct),sizeof(bBuffer)-+sizeof(MSG_GP_SR_GetRoomStruct),pGetRoomList->uKindID,
						pGetRoomList->uNameID,uBeginPos,bFinish);
					uBeginPos+=uCopyPos;
					if (bFinish) m_TCPSocket.SendData(uIndex,bBuffer,sizeof(MSG_GP_SR_GetRoomStruct)+(UINT)uCopyPos*sizeof(ComRoomInfo),MDM_GP_LIST,ASS_GP_LIST_ROOM,ERR_GP_LIST_FINISH,dwHandleID);
					else m_TCPSocket.SendData(uIndex,bBuffer,sizeof(MSG_GP_SR_GetRoomStruct)+(UINT)uCopyPos*sizeof(ComRoomInfo),MDM_GP_LIST,ASS_GP_LIST_ROOM,ERR_GP_LIST_PART,dwHandleID);
				} while (bFinish==false);

				return true;
			}
		case ASS_GP_ROOM_PASSWORD:	// ��֤ĳ��������� add by lxl 2010-10-08
			{
				//Ч������
				if (uSize!=sizeof(MSG_GP_CheckRoomPasswd)) return false;
				MSG_GP_CheckRoomPasswd * pCheckRoomPasswd=(MSG_GP_CheckRoomPasswd *)pData;

				DL_GP_I_EnterCrypticRoom enterRoom;
				enterRoom.uRoomID = pCheckRoomPasswd->uRoomID;
				strcpy(enterRoom.szMD5PassWord, pCheckRoomPasswd->szMD5PassWord);

				m_SQLDataManage.PushLine(&enterRoom.DataBaseHead,sizeof(DL_GP_I_EnterCrypticRoom), DTK_GP_CHECK_ROOM_PASSWD, uIndex, dwHandleID);//�ύ���ݿ�

				return true;
			}
		}
	}
	else  if(pNetHead->bMainID == MDM_GP_LASTLOINGGAME)
	{
		LPUSER pUser = NULL;
		for(int i=0; i<m_UserList.GetCount(); i++)
		{
			pUser = (LPUSER)m_UserList.GetAt(i);
			if(pUser == NULL)
				continue;
			if(pUser->iSocketIndex == uIndex )
			{
				DL_GP_I_LastLoginGame _p;
				_p.iUserID = pUser->UserID;
				m_SQLDataManage.PushLine(&_p.DataBaseHead,sizeof(DL_GP_I_LastLoginGame), DTK_GP_LASTLOGINGAME, uIndex, dwHandleID);//�ύ���ݿ�
				return true;
			}
		}
		return true;
	}
	//�����ź�
	else  if(pNetHead->bMainID==MDM_GP_NETSIGNAL)
    {
        m_TCPSocket.SendData(uIndex,pData,uSize,MDM_GP_NETSIGNAL,0,0,dwHandleID);
        return true;
    }
	//added by JianGuankun 2011.11.08�޸��û���Ϣ
	else if (pNetHead->bMainID == MDM_GP_USERINFO)
	{
		if (pNetHead->bAssistantID == ASS_GP_USERINFO_UPDATE_PWD)
		{
			MSG_GP_S_ChPassword* pUserChPwd = (MSG_GP_S_ChPassword*)pData;

			DL_GP_I_ChPasswordStruct DataChPwd;
			::memset(&DataChPwd,0,sizeof(DL_GP_I_ChPasswordStruct));
			DataChPwd.ChPwd = *pUserChPwd;

			m_SQLDataManage.PushLine(&DataChPwd.DataBaseHead,sizeof(DL_GP_I_ChPasswordStruct),DTK_GP_UPDATE_USERINFO_PWD,uIndex,dwHandleID);//�ύ���ݿ�
		}
		else if (pNetHead->bAssistantID == ASS_GP_USERINFO_NICKNAMEID)
		{
			// Ч������
			if (uSize != sizeof(MSG_GP_NickNameID_t)) return false;
			MSG_GP_NickNameID_t* p = (MSG_GP_NickNameID_t*)pData;

			// �������ݿ�
			DL_GP_I_GetNickNameID _in;
			_in._data = *p;
			m_SQLDataManage.PushLine(&_in.DataBaseHead,sizeof(DL_GP_I_GetNickNameID),DTK_GP_UPDATE_NICKNAMEID,uIndex,dwHandleID);

		}
		else
		{
			MSG_GP_S_UserInfo* pUser = (MSG_GP_S_UserInfo*)pData;

			DL_GP_I_UserInfoStruct DataUserInfo;
			::memset(&DataUserInfo,0,sizeof(DL_GP_I_UserInfoStruct));
			DataUserInfo.UserInfo = *pUser;

			if (pNetHead->bAssistantID == ASS_GP_USERINFO_UPDATE_BASE)
			{
				m_SQLDataManage.PushLine(&DataUserInfo.DataBaseHead,sizeof(DL_GP_I_UserInfoStruct),DTK_GP_UPDATE_USERINFO_BASE,uIndex,dwHandleID);//�ύ���ݿ�
			}
			else if (pNetHead->bAssistantID == ASS_GP_USERINFO_UPDATE_DETAIL)
			{
				m_SQLDataManage.PushLine(&DataUserInfo.DataBaseHead,sizeof(DL_GP_I_UserInfoStruct),DTK_GP_UPDATE_USERINFO_DETAIL,uIndex,dwHandleID);//�ύ���ݿ�
			}
		}
		
		return true;
	}
	//added by JianGuankun 2011.12.20 ���з���
	else if (pNetHead->bMainID == MDM_GP_BANK)
	{
		m_pBankService->OnNetMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		return true;
	}
	//added by JianGuankun 2012.5.16 �û����ŷ���
	else if (pNetHead->bMainID == MDM_GP_SMS)
	{
		if (pNetHead->bAssistantID == ASS_GP_SMS_VCODE)
		{
			MSG_GP_SmsVCode* pSmsVCodeReq = (MSG_GP_SmsVCode*)pData;

			//������֤��(6λ)
			char szVCode[16] = {0};
			::srand(::GetTickCount());
			for (int i = 0; i < 6; ++i)
			{
				szVCode[i] = 0x30 + ::rand() % 10;
			}
			
			MD5_CTX Md5;
			Md5.MD5Update((unsigned char*)szVCode,6);
			unsigned char szMDTemp[16];
			Md5.MD5Final(szMDTemp);
			for (int i=0;i<16;i++) 
			{
				wsprintf(&pSmsVCodeReq->szVCode[i*2],"%02x",szMDTemp[i]);
			}

			//�ڴ���ӷ����Ż�ȡ��֤��Ĺ���
			char szMessageBuf[256] = {0};
			int iStrIdx = pSmsVCodeReq->nType;
			if (iStrIdx < 0 || iStrIdx >= 10)
			{
				iStrIdx = 0;
			}
			_stprintf(szMessageBuf,m_strSmsContent[iStrIdx],pSmsVCodeReq->szName,szVCode);
			CString str;
			str.Format("DServer:UserName = %s Phone = %s Message = %s",pSmsVCodeReq->szName,pSmsVCodeReq->szMobileNo,szMessageBuf);
			OutputDebugString(str);

			if (m_pISms)
			{
				HRESULT hr = m_pISms->SendMessage(pSmsVCodeReq->szMobileNo,szMessageBuf);
				if (hr != S_OK)
				{
					pSmsVCodeReq->szVCode[0] = 0;
				}
			}

			m_TCPSocket.SendData(uIndex,pData,uSize,MDM_GP_SMS,ASS_GP_SMS_VCODE,0,dwHandleID);

			return true;
		}
	}
	//end add
	else 
	{
		if(pNetHead->bMainID==MDM_GR_USER_LOGO && m_userCustomLogo)
		{
			m_userCustomLogo->OnNetMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
			return true;
		}
		if(pNetHead->bMainID==MDM_GP_IM && m_imService)
		{
			if (pNetHead->bAssistantID == ASS_SET_FRIENDTYPE)
			{
				DL_GP_I_SetFriendType _p;
				LPUSER pUser = NULL;
				for(int i=0; i<m_UserList.GetCount(); i++)
				{
					pUser = (LPUSER)m_UserList.GetAt(i);
					if(pUser->iSocketIndex == uIndex )
					{
						_p.iUserID =pUser->UserID;
						_p.iType = *((int*)pData);
						pUser->iAddFriendType = _p.iType;

						SendData(uIndex, pData, uSize, MDM_GP_IM, ASS_SET_FRIENDTYPE, 0, 0);

						m_SQLDataManage.PushLine(&_p.DataBaseHead,sizeof(DL_GP_I_SetFriendType), DTK_GP_SETFRIENDTYPE, uIndex, dwHandleID);
					}
				}
			}
			else
			{
				m_imService->OnNetMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
			}
			return true;
		}
		if(pNetHead->bMainID==MDM_GP_PROP)
		{
			OnPropMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
			return true;
		}
		if (pNetHead->bMainID == MDM_GP_RECEIVEMONEY)
		{
			DL_GP_I_ReceiveMoney _p;
			LPUSER pUser = NULL;
			for(int i=0; i<m_UserList.GetCount(); i++)
			{
				pUser = (LPUSER)m_UserList.GetAt(i);
				if(pUser == NULL) m_TCPSocket.SendData(uIndex, MDM_GP_RECEIVEMONEY, ASS_RECEIVE_ERROR, 0, dwHandleID);
				if(pUser->iSocketIndex == uIndex )
				{
					_p.iUserID =pUser->UserID; 
					m_SQLDataManage.PushLine(&_p.DataBaseHead,sizeof(DL_GP_I_ReceiveMoney), DTK_GP_RECEIVEMONEY, uIndex, dwHandleID);
				}
			}
			return true;
		}
		if (pNetHead->bMainID == MDM_GP_CHARMEXCHANGE)
		{
			if (pNetHead->bAssistantID == ASS_GETLIST)
			{
				DataBaseLineHead DataBaseHead;
				m_SQLDataManage.PushLine(&DataBaseHead,sizeof(DataBaseHead), DTK_GP_CHARMEXCHANGE_LIST, uIndex, dwHandleID);
			}
			else if (pNetHead->bAssistantID == ASS_EXCHANGE)
			{
				int _id = *((int*)pData);
				DL_GP_I_CharmExchange _p;
				LPUSER pUser = NULL;
				for(int i=0; i<m_UserList.GetCount(); i++)
				{
					pUser = (LPUSER)m_UserList.GetAt(i);
					if(pUser == NULL) m_TCPSocket.SendData(uIndex, MDM_GP_RECEIVEMONEY, ASS_RECEIVE_ERROR, 0, dwHandleID);
					if(pUser->iSocketIndex == uIndex )
					{
						_p.date.iUserID = pUser->UserID;
						_p.date.iID = _id;
						m_SQLDataManage.PushLine(&_p.DataBaseHead,sizeof(DL_GP_I_CharmExchange), DTK_GP_CHARMEXCHANGE, uIndex, dwHandleID);
					}
				}
			}
			return true;
		}
	}

	return false;
}
/// {{Added by zxd 20100805 ��ȡ����GM�б�
void CGameLogonManage::OnPlayer2Gm(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	LPUSER pUser = NULL;
	switch(pNetHead->bAssistantID)
	{
	case ASS_GP_GET_GM_LIST:		///< ��ȡ����GM�б��·�
		{
			MSG_GP_R_GmList_t gmList;
			for(int i=0; i<m_UserList.GetCount(); i++)
			{
				pUser = (LPUSER)m_UserList.GetAt(i);
				if(pUser == NULL)
					continue;
				if(pUser->bIsGameManager)
				{
					gmList._arGmID[gmList._nCount++] = pUser->UserID;
				}
			}
			m_TCPSocket.SendData(uIndex, &gmList, sizeof(gmList), MDM_GP_PLAYER_2_GM, ASS_GP_GET_GM_LIST, 0, dwHandleID);
			break;
		}
	case ASS_GP_TALK_2_GM:		///< ��ͨ�����ָ����GM����
		{
			if (uSize != sizeof(MSG_GP_S_Talk2GM_t))
			{
				return;
			}
			MSG_GP_S_Talk2GM_t *pMessage = (MSG_GP_S_Talk2GM_t *)pData;
			GM_Broadcast_t msg;
			msg.iGMID = pMessage->iGMID;
			msg.iUserID = pMessage->iUserID;
			CopyMemory(msg.szMsg, pMessage->szMsg, sizeof(msg.szMsg));
			msg.szMsg[sizeof(msg.szMsg)/sizeof(TCHAR)-1] = TEXT('\0');
			
			//int iDstIndex = GetIndexByID(pMessage->iGMID);
			//if (iDstIndex < 0)
			//{
			//	TCHAR szTmp[32];
			//	memcpy(szTmp, pMessage->szMsg, sizeof(TCHAR)*32);
			//	szTmp[31] = TEXT('\0');
			//	if (_tcslen(pMessage->szMsg)>31)
			//	{
			//		szTmp[28] = TEXT('.');
			//		szTmp[29] = TEXT('.');
			//		szTmp[30] = TEXT('.');
			//	}
			//	wsprintf(msg.szMsg, TEXT("��Ϸ����Ա[%d]�Ѿ����ߣ���Ϣ[%s]���Ͳ��ɹ���"), pMessage->iGMID, szTmp);
			//	m_TCPSocket.SendData(uIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 1, dwHandleID);
			//	return;
			//}
			/// �Է����ߺͽ����߸�����һ�Σ��Ա���ʾ
			//m_TCPSocket.SendData(iDstIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_TALK_2_GM, 0, 0);
			//m_TCPSocket.SendData(uIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_TALK_2_GM, 0, 0);
			SendDataByUser(pMessage->iGMID, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_TALK_2_GM, 0, 0);
			m_TCPSocket.SendData(uIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_TALK_2_GM, 0, 0);
		}
		break;
	case ASS_GP_MSG_FROM_GM:
		{
			GM_Broadcast_t *pMessage = (GM_Broadcast_t *)pData;
			int iDstIndex = GetIndexByID(pMessage->iUserID);
			if (iDstIndex >= 0)
			{
				m_TCPSocket.SendData(uIndex, pMessage, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 0, 0);
			}
		}
	default:
		break;
	}
}
// Added by zxd 20100805 ��ȡ����GM�б�}}

void CGameLogonManage::OnPropMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_PROP_BUY_VIP:                                      //������崫�صĹ���VIP��Ϣ  add by wyl   11-5-21
		{
	
			_TAG_PROP_BUY_VIP *propGetSaved=(_TAG_PROP_BUY_VIP*)pData;
			PROP_GP_I_BUY_VIP propGpIGetPropVIP;

			propGpIGetPropVIP.propBuyVIP.dwUserID = propGetSaved->dwUserID;
			propGpIGetPropVIP.propBuyVIP.nPropType = propGetSaved->nPropType;
			memcpy(propGpIGetPropVIP.propBuyVIP.szTargetUser,propGetSaved->szTargetUser,32);

 			m_SQLDataManage.PushLine(&propGpIGetPropVIP.DataBaseHead,sizeof(PROP_GP_I_BUY_VIP),DTK_GP_PROP_BUY_VIP,uIndex,dwHandleID);
		}
	case ASS_PROP_GETUSERPROP:
		{
			MSG_PROP_C_GETSAVED *propGetSaved=(MSG_PROP_C_GETSAVED*)pData;
			PROP_GP_I_GETPROP propGpIGetProp;
			propGpIGetProp.dwUserID=propGetSaved->dwUserID;
			m_SQLDataManage.PushLine(&propGpIGetProp.DataBaseHead,sizeof(PROP_GP_I_GETPROP),DTK_GP_PROP_GETPROP,uIndex,dwHandleID);
		}
		break;
	case ASS_PROP_BUY:
		{
			if(uSize!=sizeof(_TAG_PROP_BUY))
				return;
			_TAG_PROP_BUY *propBuy=(_TAG_PROP_BUY*)pData;
			PROP_GP_I_BUY propIBuy;
			ZeroMemory(&propIBuy,sizeof(PROP_GP_I_BUY));
			propIBuy.propBuy.dwUserID=propBuy->dwUserID;
			propIBuy.propBuy.nPropID=propBuy->nPropID;
			propIBuy.propBuy.iPropPayMoney=propBuy->iPropPayMoney;
			propIBuy.propBuy.nPropBuyCount=propBuy->nPropBuyCount;
			propIBuy.propBuy.bUse = propBuy->bUse;
			/// ������������Ǹ�������ʲôҲ����
			if (propIBuy.propBuy.nPropBuyCount <=0)
			{
				break;
			}
			m_SQLDataManage.PushLine(&propIBuy.DataBaseHead,sizeof(PROP_GP_I_BUY),DTK_GP_PROP_BUY,uIndex,dwHandleID);
		}
		break;
    case ASS_PROP_BUY_NEW:      // PengJiLin, 2010-10-13, �̵�ļ�ʱ������
        {
            if(uSize!=sizeof(_TAG_PROP_BUY)) return;

            _TAG_PROP_BUY *propBuy=(_TAG_PROP_BUY*)pData;
            PROP_GP_I_BUY propIBuy;
            ZeroMemory(&propIBuy,sizeof(PROP_GP_I_BUY));
            propIBuy.propBuy.dwUserID=propBuy->dwUserID;
            propIBuy.propBuy.nPropID=propBuy->nPropID;
            propIBuy.propBuy.iPropPayMoney=propBuy->iPropPayMoney;
            propIBuy.propBuy.nPropBuyCount=propBuy->nPropBuyCount;
			propIBuy.propBuy.bUse = propBuy->bUse;
            
            // ������������Ǹ�������ʲôҲ����
            if (propIBuy.propBuy.nPropBuyCount <=0)
            {
                break;
            }
            
            // ������ߵĻ��ѷ�������ȡ����������ֵû�����塣
            // �˴�ʹ�����ֵ����Ϊ�������ڷ��仹����Ϸ�˹���
            // 0��ʾ�ڷ��乺��1��ʾ����Ϸ�˹���
            propIBuy.propBuy.iPropPayMoney = pNetHead->bHandleCode;
            m_SQLDataManage.PushLine(&propIBuy.DataBaseHead,sizeof(PROP_GP_I_BUY),
                                     DTK_GP_PROP_BUY_NEW,uIndex,dwHandleID);
        }
        break;
	case ASS_PROP_GIVE:
		{
			if(uSize!=sizeof(_TAG_PROP_GIVE))
				return;
			PROP_GP_I_GIVE propIGive;
			ZeroMemory(&propIGive,sizeof(PROP_GP_I_GIVE));
			memcpy(&propIGive.propGive,pData,uSize);
			if (propIGive.propGive.nGiveCount<=0)
			{
				break;
			}
			
			m_SQLDataManage.PushLine(&propIGive.DataBaseHead,sizeof(PROP_GP_I_GIVE),DTK_GP_PROP_GIVE,uIndex,dwHandleID);
			break;
		}
	case ASS_PROP_BIG_BOARDCASE:
		{//������
			if(uSize!=sizeof(_TAG_BOARDCAST))
				return;
			PROP_GPR_I_BOARDCAST iBoardcast;
			ZeroMemory(&iBoardcast,sizeof(PROP_GPR_I_BOARDCAST));
			memcpy(&iBoardcast.boardCast,pData,uSize);

			//yjj 090323 ��������Ϣ
			if (!m_TalkFilter.IsAllowableTalk(iBoardcast.boardCast.szMessage,strlen(iBoardcast.boardCast.szMessage)))
			{
				const CHAR* pTemTalk = m_TalkFilter.GetReplaceContent();
				strcpy(iBoardcast.boardCast.szMessage,pTemTalk);
			}
			m_SQLDataManage.PushLine(&iBoardcast.DataBaseHead,sizeof(PROP_GPR_I_BOARDCAST),DTK_GPR_PROP_BIG_BRD,uIndex,dwHandleID);
			break;
		}
	case ASS_PROP_BIG_BOARDCASE_BUYANDUSE:
		{
			if(uSize!=sizeof(_TAG_BOARDCAST))
				return;
			PROP_GPR_I_BOARDCAST iBoardcast;
			ZeroMemory(&iBoardcast,sizeof(PROP_GPR_I_BOARDCAST));
			memcpy(&iBoardcast.boardCast,pData,uSize);

			//yjj 090323 ��������Ϣ
			if (!m_TalkFilter.IsAllowableTalk(iBoardcast.boardCast.szMessage,strlen(iBoardcast.boardCast.szMessage)))
			{
				const CHAR* pTemTalk = m_TalkFilter.GetReplaceContent();
				strcpy(iBoardcast.boardCast.szMessage,pTemTalk);
			}
			m_SQLDataManage.PushLine(&iBoardcast.DataBaseHead,sizeof(PROP_GPR_I_BOARDCAST),DTK_GPR_PROP_BIG_BRD_BUYADNUSE,uIndex,dwHandleID);
		}
		break;
	}

}


void CGameLogonManage::SendRoomListPeoCount()
{
	//��ȡ��������
	bool bFinish=false;
	INT_PTR uBeginPos=0;
	char bBuffer[MAX_SEND_SIZE];
	//������������
	uBeginPos=0;
	bFinish=false;
	UINT dwHandleID=0;
	do
	{
		INT_PTR uCopyPos=m_GameList.FillRoomOnLineCount(bBuffer,sizeof(bBuffer),uBeginPos,bFinish);
		if(uCopyPos <= 0)break;
		uBeginPos+=uCopyPos;
		if(bFinish)dwHandleID=1;
		m_TCPSocket.SendDataBatch(
			bBuffer,
			(UINT)uCopyPos*sizeof(DL_GP_RoomListPeoCountStruct),
			MDM_GP_LIST,
			ASS_GP_LIST_COUNT,
			0    /////
			);//dwHandleID);
	} while (bFinish==false);

	uBeginPos=0;
	bFinish=false;
	dwHandleID=0;
	do
	{
		INT_PTR uCopyPos=m_GameList.FillNameOnLineCount(bBuffer,sizeof(bBuffer),uBeginPos,bFinish);
		if(uCopyPos <= 0)break;
		uBeginPos+=uCopyPos;
		if(bFinish)dwHandleID=1;
		m_TCPSocket.SendDataBatch(
			bBuffer,
			(UINT)uCopyPos*sizeof(DL_GP_RoomListPeoCountStruct),
			MDM_GP_LIST,
			ASS_GP_LIST_COUNT,
			1    /////
			);//dwHandleID);
	} while (bFinish==false);
}
//��������
bool CGameLogonManage::OnStart()
{
	//���¹����б����
	DL_GP_I_UpdateDataListStruct UpdateList;
	UpdateList.pGameList=&m_GameList;
	//���¹����б�
	m_SQLDataManage.PushLine(&UpdateList.DataBaseHead,sizeof(UpdateList),DTK_GP_UPDATE_LIST,0,0);
	//�����������������û���־��0
	DataBaseLineHead UserOnlineClear;
	m_SQLDataManage.PushLine(&UserOnlineClear,sizeof(UserOnlineClear),DTK_GP_IM_CLEAR,0,0);
	//������ݿ�����
	SetTimer(IDT_CHECK_DATA_CONNECT,15000L);

	SetTimer(IDT_GET_ROOM_LIST_PEOCOUNT,TIMEOUT_GET_ROOM_LIST_PEOCOUNT);

	SetTimer(IDT_GET_MAIL_LIST, TIMEOUT_GET_MAIL_LIST * 100 * 60);

	SetTimer(IDT_CHECKRECONNECT, TIMEOUT_CHECKRECONNECT * 1000);
	SetTimer(IDT_CHECKKEEPLIVE, TIMEOUT_CHECKKEEPLIVE * 1000);

	SetTimer(IDT_GET_LOGON_PEOPLE_COUNT,TIMEOUT_GET_LOGON_PEOPLE * 1000);

	// PengJiLin, 2011-5-16, ����Ƿ���Ҫ���������б�
	m_bSendSellGameList = FALSE;
	m_SQLDataManage.PushLine(&UserOnlineClear,sizeof(UserOnlineClear),DTK_GP_CHECK_SELLGAMELIST,0,0);


    // PengJiLin, 2010-6-2, ����ǵ����������ͻ��ˣ��������װ����Ӧ��DLL(Ÿ��)
    CString strPath = CBcfFile::GetAppPath();
    CBcfFile bcfFile(strPath + "Function.bcf");
    unsigned char chUseOther = bcfFile.GetKeyVal("UseForOther", "HadDll", 0);
    char chDllFileName[50] = {0};
    CString strDllFileName = bcfFile.GetKeyVal("UseForOther", "DllFileName", DLL_FILE_NAME_OPETV);
    strcpy(chDllFileName, strDllFileName);

	m_pGmMessageBase = CreateGMModule(this);
	m_pDistriManager = CreateDistriManager(this);
    g_hOpetvDll = NULL;
    if(0 != chUseOther) g_hOpetvDll = ::LoadLibrary(chDllFileName);

    if(NULL != g_hOpetvDll)         // װ�سɹ�
    {
        g_lpfn_SetLicence = (SET_LICENCE_FUNC_PTR)::GetProcAddress(g_hOpetvDll,"_SetLicence@8");
        g_lpfn_Init = (INIT_FUNC_PTR)::GetProcAddress(g_hOpetvDll, "_Init@12");
        g_lpfn_Reslease = (RELEASE_FUNC_PTR)::GetProcAddress(g_hOpetvDll, "_Release@0");
        g_lpfn_Login = (LOGIN_FUNC_PTR)::GetProcAddress(g_hOpetvDll, "_Login@12");

        if(NULL == g_lpfn_SetLicence ||
           NULL == g_lpfn_Init ||
           NULL == g_lpfn_Reslease ||
           NULL == g_lpfn_Login)
        {
            AfxMessageBox("��ע�⣬��ȡ OPETV ��ӿڳ���!");
        }

        // ��ʼ������
        g_pGameLogonManage = this;
        if(NULL != g_lpfn_SetLicence)
        {
            // ��ȡ Ÿ�� �ṩ�� Licence
            CString strLicence = bcfFile.GetKeyVal("UseForOther", "Licence", "KFGAME-2010-E10ADC3949BA59ABBE56E057F20F883E");
            char chLicence[100] = {0};
            strcpy(chLicence, strLicence);
            g_lpfn_SetLicence(chLicence, KFGAME);
        }
        if(NULL != g_lpfn_Init)
        {
            // ��һ�ε�¼����Ҫ��������֤�������̲߳���̫��
            // ����ȷ��Ϊ 2
            int iResult = g_lpfn_Init(MsgHandleCallback, 2, 1);
            
            if(iResult != 0)
            {
                CString  strInfo;
                strInfo.Format("Dll �ӿڳ�ʼ��ʧ��,������: ( %d ) !", iResult);
                AfxMessageBox(strInfo);
            }
        }
    }


	return true;
}
// PengJiLin, 2010-6-3, �����¼��Ϣ(Ÿ��)
void HandleLogonMsg(LPMSG_CONTEXT_HEAD msg_context_ptr)
{
    int iResult = ASS_GP_LOGON_ERROR;
    int iHandleCode = ERR_GP_USER_PASS_ERROR;

    HANDLE hDatabase = g_pGameLogonManage->m_SQLDataManage.hDBLogon;
    HANDLE hRecord = NULL;

    //LPUSER_INFO pUserInfo = (LPUSER_INFO)msg_context_ptr->data;

    char chTemp[5] = "00";

    int iRet = 0;

    switch(msg_context_ptr->opr_ret)
    {
    case MSG_OP_LOGON_SUCCESS:              // ��¼�ɹ��������û�����д�����ݿ������
        iResult = ASS_GP_LOGON_SUCCESS;

        // д�����ݿ� SP_AddOneUse
        hRecord = sqlSPSetName(hDatabase, "Web_pUsersAdd1", true);
        if(hRecord<(HANDLE)0x1000)
        {
            iResult = ASS_GP_LOGON_ERROR;
            iHandleCode = ERR_GP_DATABASE_FUNC_ERROR;   // ���ݿ����ʧ��
            break;
        }

        // ����û����������Ƿ���ȷ
        if((strlen(g_chUserName) < 1) || 
           (strlen(g_chMD5Pass) < 20))
        {
            iResult = ASS_GP_LOGON_ERROR;
            break;
        }
        
        sqlSPAddPara(hDatabase, hRecord, "@UserName", g_chUserName, sizeof(g_chUserName));
        sqlSPAddPara(hDatabase, hRecord, "@NickName", g_chUserName, sizeof(g_chUserName));
        sqlSPAddPara(hDatabase, hRecord, "@TGName", chTemp, sizeof(chTemp));
        sqlSPAddPara(hDatabase, hRecord, "@Pass", g_chMD5Pass, sizeof(g_chMD5Pass));
        sqlSPAddPara(hDatabase, hRecord, "@TwoPassword", g_chMD5Pass, sizeof(g_chMD5Pass));
        sqlSPAddPara(hDatabase, hRecord, "@Sex", 0);
        sqlSPAddPara(hDatabase, hRecord, "@LogoId", 0);
        sqlSPAddPara(hDatabase, hRecord, "@ZJ_Number", chTemp, sizeof(chTemp));
        sqlSPAddPara(hDatabase, hRecord, "@RegisterIP", chTemp, sizeof(chTemp));
        sqlSPAddPara(hDatabase, hRecord, "@PhoneNum", chTemp, sizeof(chTemp));
        sqlSPAddPara(hDatabase, hRecord, "@Email", chTemp, sizeof(chTemp));
        sqlSPAddPara(hDatabase, hRecord, "@QQNum", chTemp, sizeof(chTemp));

        if(!sqlSPExec(hDatabase, hRecord, true))
        {
            iResult = ASS_GP_LOGON_ERROR;
            iHandleCode = ERR_GP_DATABASE_FUNC_ERROR;   // ���ݿ����ʧ��
            break;
        }

        iRet = sqlSPGetReturnValue(hDatabase, hRecord);
        sqlSPClose(hDatabase, hRecord);
        if(0 == iRet)   // �洢����ִ�гɹ�, ����ȥ��֤��¼
        {
            g_pGameLogonManage->m_TCPSocket.SendData(msg_context_ptr->serial_no/*pUserInfo->iUserIndex*/,
                                                MDM_GP_LOGON, ASS_GP_LOGON_BY_SOFTWARE, iHandleCode, 0);
            return;
        }
        else            // �洢����ִ��ʧ��
        {
            iResult = ASS_GP_LOGON_ERROR;
            iHandleCode = ERR_GP_DATABASE_RETURN_ERROR;   // ���ݿⷵ��ִ�д�����
        }

        break;
    case MSG_OP_LOGON_INFO_ERROR:           // �û������������
        iHandleCode = ERR_GP_USER_PASS_ERROR;
        break;
    case MSG_OP_LOGON_USING:                // �û��ѵ�¼
        iHandleCode = ERR_GP_USER_LOGON;
        break;
    case MSG_OP_LOGON_NO_USER:              // �û������� 
        iHandleCode = ERR_GP_USER_NOT_EXIST;
        break;
    case MSG_OP_LOGON_FORBID:               // �û����ڽ���״̬
        iHandleCode = ERR_GP_USER_VALIDATA;
        break;
    case MSG_OP_LOGON_OVERDATE:             // �û��ѹ���
        iHandleCode = ERR_GP_USER_OVERDATE;
        break;
    default:
        break;
    }

    g_pGameLogonManage->m_TCPSocket.SendData(msg_context_ptr->serial_no,
                                             MDM_GP_LOGON, iResult, iHandleCode, 0);
}

// PengJiLin, 2010-6-3, ��Ϣ�ص�����, ���ڵ����� DLL ���á�(Ÿ��)
void __stdcall MsgHandleCallback(int msg_type, LPMSG_CONTEXT_HEAD msg_context_ptr)
{
    switch(msg_type)
    {
    case MSG_TYPE_LOGON:
        //::EnterCriticalSection(&g_CriticalSection); // �˴��ƶ�������������
        HandleLogonMsg(msg_context_ptr);
        ::LeaveCriticalSection(&g_CriticalSection);
        break;
    default:        // ��ʱֱ�ӷ��ص�¼����
        g_pGameLogonManage->m_TCPSocket.SendData(msg_context_ptr->serial_no,
                  MDM_GP_LOGON, ASS_GP_LOGON_ERROR, ERR_GP_USER_PASS_ERROR, 0);
        break;
    }
}
//��ʱ����Ϣ
bool CGameLogonManage::OnTimerMessage(UINT uTimerID)
{
	switch (uTimerID)
	{
	case IDT_GET_ROOM_LIST_PEOCOUNT:			//���¹����б�ʱ��
		{
			KillTimer(IDT_GET_ROOM_LIST_PEOCOUNT);
			DL_GP_I_UpdateDataListStruct UpdateList;
			UpdateList.pGameList=&m_GameList;
			m_SQLDataManage.PushLine(&UpdateList.DataBaseHead,sizeof(UpdateList),DTK_GP_GET_ROOM_LIST_PEOCOUNT,0,0);
			return true;
		}
	case IDT_GET_MAIL_LIST:
		{
			KillTimer(IDT_GET_MAIL_LIST);
			DataBaseLineHead _p;
			m_SQLDataManage.PushLine(&_p, sizeof(DataBaseLineHead),DTK_GP_MAILLIST_GET, 0, 0);
			return true;
		}
	case IDT_GET_LOGON_PEOPLE_COUNT:
		{
			KillTimer(IDT_GET_LOGON_PEOPLE_COUNT);
			DataBaseLineHead _p;
			m_SQLDataManage.PushLine(&_p, sizeof(DataBaseLineHead),DTK_GP_LOGONPEOPLE_GET, 0, 0);
			return true;
		}
	case TID_UPDATE_LIST:			//���¹����б�ʱ��
		{
			KillTimer(TID_UPDATE_LIST);
			DL_GP_I_UpdateDataListStruct UpdateList;
			UpdateList.pGameList=&m_GameList;
			m_SQLDataManage.PushLine(&UpdateList.DataBaseHead,sizeof(UpdateList),DTK_GP_UPDATE_LIST,0,0);
			return true;
		}
	case IDT_CHECK_DATA_CONNECT:	//������ݿ�����
		{
			KillTimer(IDT_CHECK_DATA_CONNECT);
			DataBaseLineHead DataBaseHead;
			m_SQLDataManage.PushLine(&DataBaseHead,sizeof(DataBaseHead),DTK_GP_CHECK_CONNECT,0,0);
			
			return true;
		}
	case IDT_CHECKRECONNECT:
		{
			KillTimer(IDT_CHECKRECONNECT);
			if (m_pDistriManager) 
				m_pDistriManager->CheckReConnect();
			SetTimer(IDT_CHECKRECONNECT, TIMEOUT_CHECKRECONNECT * 1000);

			return true;
		}
	case IDT_CHECKKEEPLIVE:
		{
			KillTimer(IDT_CHECKKEEPLIVE);
			if (m_pDistriManager) 
				m_pDistriManager->CheckKeepLive();
			SetTimer(IDT_CHECKKEEPLIVE, TIMEOUT_CHECKKEEPLIVE * 1000);

			return true;
		}
	}
	return false;
}
/// {{Added by zxd 20100729����GM����
/// ���¼����������ǵ��ô洢���̲��������ݼ�����������
///    1����ȡ��������������������
///    2��ȷ���洢����
///    3��׼���洢���̵��������
///    4��ִ�д洢����
///    5���жϴ洢���̷���ֵ
///    6����ȡ���ص����ݼ�
///    7������֪ͨ��Ϣ����һ���������

///GM�������Ϣ��ȡ�Ѿ�����������������ID���ǳƶ�Ӧ��ϵ
UINT CServiceDataBaseHandle::OnGMGetOperatedUser(DataBaseLineHead * pSourceData)
{
	CHECK_ERROR_BEGIN
	/// ��ȡ��������������������

	GM_DT_I_GetOperatedUser_t *pGetHistory = (GM_DT_I_GetOperatedUser_t *)pSourceData;
	GM_DT_O_OperatedUser_t getHistoryResult;

	/// ȷ���洢���� 
	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=NULL;
	if (pGetHistory->requestPlayer.bType == 0)
	{
		hRecord=sqlSPSetNameEx(hDatabase,"SP_GMGetOperatedUserByID",true);
		/// ׼���洢���̵��������
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pGetHistory->requestPlayer.playerInfo.UserID);
	}
	else
	{
		hRecord=sqlSPSetNameEx(hDatabase,"SP_GMGetOperatedUserByName",true);
		sqlSPAddPara(hDatabase, hRecord, "@Type", pGetHistory->requestPlayer.bType);
		sqlSPAddPara(hDatabase, hRecord, "@Name", pGetHistory->requestPlayer.playerInfo.szName, sizeof(pGetHistory->requestPlayer.playerInfo.szName));
	}
	if(hRecord<(HANDLE)0x1000) REPORT_ERROR;

	/// ִ�д洢���̣�������true���ܷ������ݼ�
	sqlSPExec(hDatabase,hRecord, true);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);
	int handleResult = DTR_GM_GET_RECORD_END;
	/// �жϴ洢���̷���ֵ
	if (ret == 0 )
	{
		if(!sqlEndOfFile(hDatabase,hRecord))
		{
			/// ��ȡ���ص����ݼ�
			handleResult = DTR_GM_GET_RECORD_OK;
			sqlGetValue(hDatabase,hRecord,"UserID",getHistoryResult.operatedPlayer.iUserID);
			sqlGetValue(hDatabase,hRecord,"UserName",getHistoryResult.operatedPlayer.szUserName, sizeof(getHistoryResult.operatedPlayer.szUserName));
			sqlGetValue(hDatabase,hRecord,"NickName",getHistoryResult.operatedPlayer.szNickName, sizeof(getHistoryResult.operatedPlayer.szNickName));
			sqlGetValue(hDatabase,hRecord,"GMID",getHistoryResult.operatedPlayer.iGMID);
			sqlGetValue(hDatabase,hRecord,"OperationType",getHistoryResult.operatedPlayer.iOperation);
			sqlGetValue(hDatabase,hRecord,"BeginTime",getHistoryResult.operatedPlayer.dtDate);
			sqlGetValue(hDatabase,hRecord,"LastMinutes",getHistoryResult.operatedPlayer.iPeriod);
			sqlGetValue(hDatabase,hRecord,"Reason",getHistoryResult.operatedPlayer.szReason, sizeof(getHistoryResult.operatedPlayer.szReason));
			char szTmp[16];
			sqlGetValue(hDatabase,hRecord,"ZID", szTmp, sizeof(szTmp));
			getHistoryResult.operatedPlayer.ZID = atoi(szTmp);
			sqlGetValue(hDatabase,hRecord,"WID",getHistoryResult.operatedPlayer.WID);
		}
	}

	sqlSPClose(hDatabase,hRecord);
	/// ����֪ͨ��Ϣ����һ���������
	m_pRusultService->OnDataBaseResultEvent(&getHistoryResult.ResultHead,DTK_GET_OPEATED_USER_GM, handleResult, sizeof(GM_DT_O_OperatedUser_t),pSourceData->uIndex,pSourceData->dwHandleID);
	CHECK_ERROR_END
	return 0;
}
///GM��ȡ��������
UINT CServiceDataBaseHandle::OnGMGetAwardType(DataBaseLineHead * pSourceData)
{
	CHECK_ERROR_BEGIN
		GM_DT_O_AwardType_t awardTypeResult;
	/// ȷ���洢���� 
	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GMGetAwardTypeList",true);

	/// ִ�д洢����
	sqlSPExec(hDatabase,hRecord, true);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);
	/// �жϴ洢���̷���ֵ
	if (ret == 0 )
	{
		/// ��ȡ���ص����ݼ�
		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&awardTypeResult,sizeof(GM_DT_O_AwardType_t));
			sqlGetValue(hDatabase, hRecord,"SeriNo", awardTypeResult.awardType.iSeriNo);
			sqlGetValue(hDatabase, hRecord,"Gold", awardTypeResult.awardType.iGoldNum);
			sqlGetValue(hDatabase, hRecord,"PropID1", awardTypeResult.awardType.iPropAward[0].iPropID);
			sqlGetValue(hDatabase, hRecord,"PropID2", awardTypeResult.awardType.iPropAward[1].iPropID);
			sqlGetValue(hDatabase, hRecord,"PropID3", awardTypeResult.awardType.iPropAward[2].iPropID);
			sqlGetValue(hDatabase, hRecord,"PropID4", awardTypeResult.awardType.iPropAward[3].iPropID);
			sqlGetValue(hDatabase, hRecord,"PropCount1", awardTypeResult.awardType.iPropAward[0].iCount);
			sqlGetValue(hDatabase, hRecord,"PropCount2", awardTypeResult.awardType.iPropAward[1].iCount);
			sqlGetValue(hDatabase, hRecord,"PropCount3", awardTypeResult.awardType.iPropAward[2].iCount);
			sqlGetValue(hDatabase, hRecord,"PropCount4", awardTypeResult.awardType.iPropAward[3].iCount);

			/// ����֪ͨ��Ϣ����һ���������
			m_pRusultService->OnDataBaseResultEvent(&awardTypeResult.ResultHead, DTK_GET_AWARD_TYPE_GM, DTR_GM_GET_RECORD_OK, sizeof(GM_DT_O_AwardType_t),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}
		/// ����֪ͨ��Ϣ����һ���������
		m_pRusultService->OnDataBaseResultEvent(&awardTypeResult.ResultHead, DTK_GET_AWARD_TYPE_GM, DTR_GM_GET_RECORD_END, sizeof(GM_DT_O_AwardType_t),pSourceData->uIndex,pSourceData->dwHandleID);
	}

	sqlSPClose(hDatabase,hRecord);

	CHECK_ERROR_END
	return 0;
}
///GM��ȡ����ID�����ƶ�Ӧ��ϵ
UINT CServiceDataBaseHandle::OnGMGetPropInfo(DataBaseLineHead * pSourceData)
{
	CHECK_ERROR_BEGIN
		GM_DT_O_GetPropInfoResult_t propInfoResult;
	/// ȷ���洢���� 
	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GMGetPropInfo",true);

	/// ִ�д洢����
	sqlSPExec(hDatabase,hRecord, true);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);
	/// �жϴ洢���̷���ֵ
	if (ret == 0 )
	{
		/// ��ȡ���ص����ݼ�
		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&propInfoResult,sizeof(GM_DT_O_GetPropInfoResult_t));
			sqlGetValue(hDatabase, hRecord,"PropID", propInfoResult.propInfo.iPropID);
			sqlGetValue(hDatabase, hRecord,"PropName", propInfoResult.propInfo.szPropName, sizeof(propInfoResult.propInfo.szPropName));

			/// ����֪ͨ��Ϣ����һ���������
			m_pRusultService->OnDataBaseResultEvent(&propInfoResult.ResultHead, DTK_GET_PROP_INFO_GM, DTR_GM_GET_RECORD_OK, sizeof(GM_DT_O_GetPropInfoResult_t),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}
		/// ����֪ͨ��Ϣ����һ���������
		m_pRusultService->OnDataBaseResultEvent(&propInfoResult.ResultHead, DTK_GET_PROP_INFO_GM, DTR_GM_GET_RECORD_END, sizeof(GM_DT_O_GetPropInfoResult_t),pSourceData->uIndex,pSourceData->dwHandleID);
	}

	sqlSPClose(hDatabase,hRecord);		

	CHECK_ERROR_END
	return 0;
}
///GM����ҽ��н��Ͳ���0
UINT CServiceDataBaseHandle::OnGMOperateToUser(DataBaseLineHead * pSourceData)
{
	CHECK_ERROR_BEGIN
		/// ��ȡ��������������������

	GM_DT_I_OperateUser_t *pOperation = (GM_DT_I_OperateUser_t *)pSourceData;

	/// ȷ���洢���� 
	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GMOperateUser",true);
	/// ׼���洢���̵��������
	sqlSPAddPara(hDatabase,hRecord,"@UserID",pOperation->operation.iUserID);
	sqlSPAddPara(hDatabase,hRecord,"@GMID",pOperation->operation.iGMID);
	sqlSPAddPara(hDatabase,hRecord,"@OperationType",pOperation->operation.iOperation);
	sqlSPAddPara(hDatabase,hRecord,"@LastMinutes",pOperation->operation.iWorkMinutes);
	sqlSPAddPara(hDatabase,hRecord,"@AwardType",pOperation->operation.iWorkMinutes);
	sqlSPAddPara(hDatabase,hRecord,"@Reason", pOperation->operation.szReason, sizeof(pOperation->operation.szReason));

	if(hRecord<(HANDLE)0x1000) REPORT_ERROR;

	/// ִ�д洢����
	sqlSPExec(hDatabase,hRecord);
	int ret=sqlSPGetReturnValue(hDatabase,hRecord);
	int handleResult = ERR_GM_OPERATOR_FAILED;
	if (ret == 0)
	{
		handleResult = ERR_GM_OPERATOR_SUCCESS;
	}
	DataBaseResultLine ResultHead;
	m_pRusultService->OnDataBaseResultEvent(&ResultHead,DTK_GET_OPEATED_USER_GM, handleResult, sizeof(ResultHead), pSourceData->uIndex,pSourceData->dwHandleID);

	sqlSPClose(hDatabase,hRecord);		
	
	CHECK_ERROR_END
	return 0;
}
//GM��½����
UINT CServiceDataBaseHandle::OnGMLogon(DataBaseLineHead * pSourceData, UINT uLogonType)
{
	CHECK_ERROR_BEGIN

		GM_DT_I_LogonByName_t *pLogonInfo = (GM_DT_I_LogonByName_t *)pSourceData;
		GM_DT_O_LogonResult_t logResult;

		/// ȷ���洢���� 
		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GMLogon",true);
		/// ׼���洢���̵��������
		sqlSPAddPara(hDatabase, hRecord,"@UserName",pLogonInfo->logonInfo.szName, sizeof(pLogonInfo->logonInfo.szName));
		sqlSPAddPara(hDatabase, hRecord,"@MD5Password",pLogonInfo->logonInfo.szMD5Pass, sizeof(pLogonInfo->logonInfo.szMD5Pass));
		sqlSPAddPara(hDatabase, hRecord,"@UserLoginIP",pLogonInfo->szAccessIP, sizeof(pLogonInfo->szAccessIP));
		sqlSPAddPara(hDatabase, hRecord,"@MachineCode",pLogonInfo->logonInfo.szMathineCode, sizeof(pLogonInfo->logonInfo.szMathineCode));
		sqlSPAddPara(hDatabase, hRecord,"@ZID", 0);

		if(hRecord<(HANDLE)0x1000) REPORT_ERROR;
		int handleResult = DTR_GM_LOGON_SUCCESS;

		/// ִ�д洢����
		sqlSPExec(hDatabase,hRecord, true);

		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		if (ret == 0)
		{
			if(!sqlEndOfFile(hDatabase,hRecord))
			{
				sqlGetValue(hDatabase, hRecord, "GMID", logResult.logonResult.dwUserID);
				sqlGetValue(hDatabase, hRecord, "NickName", logResult.logonResult.szName, sizeof(logResult.logonResult.szName));
				TCHAR szLastLogonIP[16]={0};
				sqlGetValue(hDatabase,hRecord,TEXT("LastLoginIP"),szLastLogonIP,sizeof(szLastLogonIP));
				logResult.logonResult.dwLastLogonIP=inet_addr(szLastLogonIP);
				sqlGetValue(hDatabase, hRecord, "Master", logResult.logonResult.iMasterRight);
				sqlGetValue(hDatabase, hRecord, "MD5Password", logResult.logonResult.szMD5Pass, sizeof(logResult.logonResult.szMD5Pass));
			}
			else
			{
				handleResult = DTR_GM_HAS_NOT_RIGHT;
			}
		}
		else
		{
			switch (ret)
			{
			case 1: //�û�������
				{handleResult = DTR_GM_USER_NOT_FOUND; break;}
			case 2: // ���벻��ȷ
				{handleResult = DTR_GM_PASSWORD_ERROR;break;}
			case 3: // û��GMȨ��
				{handleResult = DTR_GM_HAS_NOT_RIGHT;break;}
			case 4: // �����벻��ȷ
				{handleResult = DTR_GM_MACHINECODE_ERR;break;}
			case 5: // �Ѿ�����һ��ͬ��GM��¼��
				{handleResult = DTR_GM_ALREADY_LOGON;break;}
			default:
				break;
			}
		}
		sqlSPClose(hDatabase,hRecord);

		/// ����֪ͨ��Ϣ����һ���������
		m_pRusultService->OnDataBaseResultEvent(&logResult.ResultHead,DTK_LOGON_BY_NAME_GM, handleResult, sizeof(GM_DT_O_LogonResult_t),pSourceData->uIndex,pSourceData->dwHandleID);
		//DTK_LOGON_BY_NAME_GM
	CHECK_ERROR_END
		return 0;

}
/// Added by zxd 20100729����GM����}}

//���ݿ⴦��ӿ�
UINT CServiceDataBaseHandle::HandleDataBase(DataBaseLineHead * pSourceData)
{
	switch (pSourceData->uHandleKind)
	{
		///{{ Added by zxd 20100802 ����GMģ�鴦�����
	case DTK_GET_PROP_INFO_GM:
		{
			return OnGMGetPropInfo(pSourceData);
		}
	case DTK_PUT_OPEATION_USER_GM:
		{
			return OnGMOperateToUser(pSourceData);
		}
	case DTK_GET_OPEATED_USER_GM:
		{
			return OnGMGetOperatedUser(pSourceData);
		}
	case DTK_GET_AWARD_TYPE_GM:
		{
			return OnGMGetAwardType(pSourceData);	///< GM��ȡ��������
		}
	case DTK_LOGON_BY_NAME_GM:
		{
			return OnGMLogon(pSourceData,pSourceData->uHandleKind);///<����GMר�õ�½����
		}
		/// Added by zxd 20100802 ����GMģ�鴦�����}}
	case DTK_GP_LOGON_BY_NAME:		//ͨ�����ֵ�½
	case DTK_GP_LOGON_BY_ACC:		//ͨ�� AccID ��½
	case DTK_GP_LOGON_BY_MOBILE:	//ͨ���ֻ���½
		{
			return OnPlaceLogon(pSourceData,pSourceData->uHandleKind);//////���õ�½����
		}
	case DTK_GP_UPDATE_LIST:		//���¹����б�
		{
			return OnUpdateGameList(pSourceData);
		}
	case DTK_GP_CHECK_CONNECT:		//������ݿ�����
		{
			return OnCheckDataConnect(pSourceData);
		}
	case DTK_GP_GET_ROOM_LIST_PEOCOUNT:		//pppp
		{
			return OnGetRoomListPeoCount(pSourceData);
		}
	case DTK_GP_MAILLIST_GET:
		{
			return OnMailListGet(pSourceData);
		}
	case DTK_GP_LOGONPEOPLE_GET:
		{
			return OnGetLogonPeopCount(pSourceData);
		}
	case DTK_GP_MAIL_UPDATE:
		{
			return OnMailUpdate(pSourceData);
		}
	case DTK_GP_CHECK_ROOM_PASSWD:  //��ͼ������ܷ��� add by lxl 2010-10-08
		{
			OnCheckRoomPasswd(pSourceData);
		}
		break;
	case DTK_GP_GET_SELLGAMELIST:
		{
			OnGetSellGameList(pSourceData);
		}
	//�����Ƕ��û�ͷ��Ĵ���
	case DTK_GP_USERLOGO_UPDATE:
		OnUpdateUserLogoID(pSourceData);
		break;
	case DTK_GP_USERLOGO_GETMD5:
		OnRequestUserLogoMD5(pSourceData);
		break;

	//�����ǶԺ��ѵĲ�������
	//���غ�������
	case DTK_GP_IM_GETCOUNT:
		OnDBIMGetUserCount(pSourceData);
		break;
	//����������������߱�־
	case DTK_GP_IM_CLEAR:
		OnClearOnlineFlag();
		break;
	//�û�����
	case DTK_GP_IM_OFFLINE:
		OnUserOffline(pSourceData);
		break;
	case DTK_GP_IM_SETUSER:
		OnIMSetUser(pSourceData);
		break;
	case DTK_GP_ZRECORD:	
		{
			OnWriteUserLogonRecord(pSourceData);
			break;
		}
	//�����ǵ��߲���
	//Fred Huang,2008-08
	case DTK_GP_PROP_BUY_VIP:
		OnPropBuyVIP(pSourceData);    //���߽��湺��VIP add by wyl  11-5-21
	case DTK_GP_PROP_GETPROP:
		OnPropGetList(pSourceData);
		break;
	case DTK_GP_PROP_BUY:
		OnPropBuy(pSourceData);
		break;
    case DTK_GP_PROP_BUY_NEW:   // PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
        OnPropBuyNew(pSourceData);
        break;
	case DTK_GP_PROP_GIVE:
		OnPropGive(pSourceData);
		break;
	case DTK_GPR_PROP_BIG_BRD:
		OnPropBigBrd(pSourceData);
		break;
	case DTK_GPR_PROP_BIG_BRD_BUYADNUSE:
		OnPropBigBrd_BuyAndUse(pSourceData);
		break;

	case DTK_GP_LOCK_UNLOCK_ACCOUNT:
		OnUserLockAccount(pSourceData);	//�������� zxj 2009-11-13
		break;

	case DTK_GP_BIND_MOBILE:
		OnUserBindMobile(pSourceData);
		break;

	case DTK_GR_CHECK_VERSION:
		{
			return OnCheckVersion(pSourceData);	// zxd add for check version20100312
		}
	case DTK_GP_CHECK_SELLGAMELIST:				//wyl 2011-5-16  �ж��Ƿ�����Ϣ�б�
		{
			return OnCheckSellGameList(pSourceData);  
		}
	case DTK_GP_UPDATE_USERINFO_BASE:
		{
			return OnUpdateUserInfo(pSourceData,DTK_GP_UPDATE_USERINFO_BASE);
		}
	case DTK_GP_UPDATE_USERINFO_DETAIL:
		{
			return OnUpdateUserInfo(pSourceData,DTK_GP_UPDATE_USERINFO_DETAIL);
		}
	case DTK_GP_UPDATE_USERINFO_PWD:
		{
			return OnUpdateUserPwd(pSourceData);
		}
	case DTK_GP_UPDATE_NICKNAMEID:
		{
			return OnQueryUserNickNameID(pSourceData);
		}
	case DTK_GP_RECEIVEMONEY:
		{
			return OnReceiveMoney(pSourceData);
		}
	case DTK_GP_CHARMEXCHANGE_LIST:
		{
			return OnCharmExchangeList(pSourceData);
		}
	case DTK_GP_CHARMEXCHANGE:
		{
			return OnCharmExchange(pSourceData);
		}
	case DTK_GP_SETFRIENDTYPE:
		{
			return OnSetFriendType(pSourceData);
		}

	///{{ Added by JianGuankun 2011.12.20 �������д���ģ��
	case DTK_GP_BANK_OPEN:
		{
			return OnBankOpen(pSourceData);
		}
	case DTK_GP_BANK_GETGM_WALLET:
		{
			return OnBankGetGameWallet(pSourceData);
		}
	case DTK_GP_BANK_CHECK:
		{
			return OnBankCheck(pSourceData);
		}
	case DTK_GP_BANK_TRANSFER:
		{
			return OnBankTransfer(pSourceData);
		}
	case DTK_GP_BANK_TRANS_RECORD:
		{
			return OnBankTransRecord(pSourceData);
		}
	case DTK_GP_BANK_DEPOSIT:
		{
			return OnBankDeposit(pSourceData);
		}
	case DTK_GP_BANK_QUERY_DEPOSIT:
		{
			return OnBankQueryDeposits(pSourceData);
		}
	case DTK_GP_BANK_CHPWD:
		{
			return OnBankChangePassword(pSourceData);
		}
	case DTK_GP_BANK_FINANCE_INFO:
		{
			return OnBankGetFinanceInfo(pSourceData);
		}

	case DTK_GP_LASTLOGINGAME:
		{
			return OnLastLoginGame(pSourceData);
		}
	}

	return 0;
}
/*
Function	:OnPropBigBrd
Notes		:������
Author		:Fred Huang
Date		:2008-08-19
Parameter	:
return		:void

*/
void CServiceDataBaseHandle::OnPropBigBrd(DataBaseLineHead *pSourceData)
{
	PROP_GPR_I_BOARDCAST *iBoard=(PROP_GPR_I_BOARDCAST*)pSourceData;
	PROP_GPR_O_BOARDCAST oBoard;
	memcpy(&oBoard.boardCast,&iBoard->boardCast,sizeof(_TAG_BOARDCAST));

	int iHandleResult=DTK_GR_PROP_USE_SUCCEED;
	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;


	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_PropOpera",true);
	if(hRecord<(HANDLE)0x1000)
		throw DTR_GR_PROP_USE_ERR_UNKNOW;
	sqlSPAddPara(hDatabase,hRecord,"@UserID",iBoard->boardCast.dwUserID);
	sqlSPAddPara(hDatabase,hRecord,"@PropID",iBoard->boardCast.nPropID);
	sqlSPAddPara(hDatabase,hRecord,"@PropCount",1);			//ֻʹ��1��
	TCHAR szTarget[32]={0};
	sqlSPAddPara(hDatabase,hRecord,"@TargetName",szTarget,sizeof(szTarget));		//���Ͷ����ʹ�ö���ֻ�е�isGive=1ʱ��Ч
	sqlSPAddPara(hDatabase,hRecord,"@IsGive",0);				//�Ƿ����ͣ�ֻ��=1ʱ��������
	sqlSPExec(hDatabase,hRecord);

	int ret=sqlSPGetReturnValue(hDatabase,hRecord);

	sqlSPClose(hDatabase,hRecord);
	if(ret!=0)
		iHandleResult=DTR_GR_PROP_USE_ERR_NOPROP;
	m_pRusultService->OnDataBaseResultEvent(&oBoard.ResultHead,DTK_GPR_PROP_BIG_BRD,iHandleResult,sizeof(PROP_GPR_O_BOARDCAST),pSourceData->uIndex,pSourceData->dwHandleID);
}

void CServiceDataBaseHandle::OnPropBigBrd_BuyAndUse(DataBaseLineHead *pSourceData)
{
	PROP_GPR_I_BOARDCAST *iBoard=(PROP_GPR_I_BOARDCAST*)pSourceData;
	PROP_GPR_O_BOARDCAST oBoard;
	memcpy(&oBoard.boardCast,&iBoard->boardCast,sizeof(_TAG_BOARDCAST));

	PROP_GP_O_BUY propOBuy;
	ZeroMemory(&propOBuy,sizeof(PROP_GP_O_BUY));

	propOBuy.msgPropGet.dwUserID=iBoard->boardCast.dwUserID;
	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		if (m_pKernelInfo->bNativeDataBase && hDatabase)
		{
			HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_NewPropBuy",true);
			if(hRecord<(HANDLE)0x1000)
				throw DTK_GP_PROP_BUY_ERROR;

			sqlSPAddPara(hDatabase,hRecord,"@UserID",iBoard->boardCast.dwUserID);
			sqlSPAddPara(hDatabase,hRecord,"@PropID",iBoard->boardCast.nPropID);
			sqlSPAddPara(hDatabase,hRecord,"@PropCount",1);

			sqlSPExec(hDatabase,hRecord,true);

			int ret=sqlSPGetReturnValue(hDatabase,hRecord);

			if(ret!=0)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GP_PROP_BUY_NOMONEY;
			}

			// �������Դ˴�����Ҫ��ʹ�����ֵ���ֵ������ڷ��仹����Ϸ�˹����
			// 0��ʾ�ڷ��乺��1��ʾ����Ϸ�˹���
			//propOBuy.msgPropGet.attribAction = propIBuy->propBuy.iPropPayMoney;

			propOBuy.msgPropGet.dwUserID = iBoard->boardCast.dwUserID;
			propOBuy.msgPropGet.nPropID = iBoard->boardCast.nPropID;
			sqlGetValue(hDatabase,hRecord,"HoldCount",propOBuy.msgPropGet.nHoldCount);
			sqlGetValue(hDatabase,hRecord,"PropCost",propOBuy.msgPropGet.dwCost);
			sqlSPClose(hDatabase,hRecord);
			m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_NEW,
				DTK_GP_PROP_BUYANDUSE_SUCCEED,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);

			sqlSPClose(hDatabase,hRecord);


			int iHandleResult=DTK_GR_PROP_USE_SUCCEED;


			hRecord=sqlSPSetNameEx(hDatabase,"SP_PropOpera",true);
			if(hRecord<(HANDLE)0x1000)
				throw DTR_GR_PROP_USE_ERR_UNKNOW;
			sqlSPAddPara(hDatabase,hRecord,"@UserID",iBoard->boardCast.dwUserID);
			sqlSPAddPara(hDatabase,hRecord,"@PropID",iBoard->boardCast.nPropID);
			sqlSPAddPara(hDatabase,hRecord,"@PropCount",1);			//ֻʹ��1��
			TCHAR szTarget[32]={0};
			sqlSPAddPara(hDatabase,hRecord,"@TargetName",szTarget,sizeof(szTarget));		//���Ͷ����ʹ�ö���ֻ�е�isGive=1ʱ��Ч
			sqlSPAddPara(hDatabase,hRecord,"@IsGive",0);				//�Ƿ����ͣ�ֻ��=1ʱ��������
			sqlSPExec(hDatabase,hRecord);

			ret=sqlSPGetReturnValue(hDatabase,hRecord);

			sqlSPClose(hDatabase,hRecord);
			if(ret!=0)
				iHandleResult=DTR_GR_PROP_USE_ERR_NOPROP;
			m_pRusultService->OnDataBaseResultEvent(&oBoard.ResultHead,DTK_GPR_PROP_BIG_BRD,iHandleResult,sizeof(PROP_GPR_O_BOARDCAST),pSourceData->uIndex,pSourceData->dwHandleID);
		}
	}
	catch (int &iHandleResult)
	{
		TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_NEW,
			iHandleResult,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);
	}


	


}

/// zxd add for check version20100312
UINT CServiceDataBaseHandle::OnCheckVersion(DataBaseLineHead * pSourceData)
{
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GR_I_CheckVersion_t))
	{
		return -1;
	}
	DL_GR_I_CheckVersion_t *pCheckVersion = (DL_GR_I_CheckVersion_t *)pSourceData;
	char sql[256];
	switch(pCheckVersion->uType)
	{
	case 0:
		//"update TUsers set Pass='0', AgencyID=0"
		sprintf( sql, "%s %s %s %s='0', %s=0", szUpdate, szTUsers, szSet, szPass, szAgency);
		break;
	case 1:
		//"update TUserInfo set Walletmoney=1, bankmoney=0"
		sprintf( sql, "%s %s %s %s='0', %s=0", szUpdate, szTUserInfo, szSet, szWalletMoney, szBankMoney);
		break;
	case 2:
		//"delete from Web_Users"
		sprintf( sql, "%s from %s", szDelete_, szWeb_Users);
		break;
	case 3:
		//"delete from TChangeRecord"
		sprintf( sql, "%s from %s", szDelete_, szTChangeRecord);
		break;
	default:
		return 0;
	}
	sqlExec(m_pDataBaseManage->hDBLogon, sql, false);
	return 0;
}
//�������� zxj 2009-11-13
void CServiceDataBaseHandle::OnUserLockAccount(DataBaseLineHead * pSourceData)
{
	if( pSourceData == NULL )
		return;
	DL_GP_LockAccount * pLockData = (DL_GP_LockAccount*)pSourceData;
	HANDLE hDatabase = m_pDataBaseManage->hDBLogon;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_UserLockOrUnlockMathine", true);	//����Ҫ����ֵ
	if( hRecord <(HANDLE)0x1000)
	{
		OutputDebugString("�������������洢����ʧ��");
		return; 
	}
	sqlSPAddPara(hDatabase, hRecord, "@UserID", pLockData->LockAccount.dwUserID);
	sqlSPAddPara(hDatabase, hRecord, "@CommanType", pLockData->LockAccount.dwCommanType);
	sqlSPAddPara(hDatabase, hRecord, "@MathineCode", pLockData->LockAccount.szMathineCode, sizeof(pLockData->LockAccount.szMathineCode));
	sqlSPAddPara(hDatabase, hRecord, "@Pwd", pLockData->LockAccount.szMD5Pass, sizeof(pLockData->LockAccount.szMD5Pass));
	sqlSPAddPara(hDatabase, hRecord, "@MobileNo",pLockData->LockAccount.szMobileNo,sizeof(pLockData->LockAccount.szMobileNo));
	sqlSPExec(hDatabase, hRecord);
	int nRet = sqlSPGetReturnValue(hDatabase, hRecord);
	sqlSPClose(hDatabase, hRecord);
OutputDebugString("[����] 1");
	int iHandleResult = 0;
	MSG_GP_S_LockAccountResult LockResult;
	::ZeroMemory(&LockResult, sizeof(LockResult));
	LockResult.dwCommanType = pLockData->LockAccount.dwCommanType;
	LockResult.dwUserID = pLockData->LockAccount.dwUserID;
	LockResult.dwCommanResult = nRet;
	switch(nRet)
	{
	case 0:
		{OutputDebugString("[����] 2");
			iHandleResult = DTR_GP_ERROR_UNKNOW;
			LockResult.dwCommanResult = ERR_GP_ERROR_UNKNOW;
			break;
		}
	case 1:
		{OutputDebugString("[����] 3");
			LockResult.dwCommanResult = ERR_GP_LOCK_SUCCESS;
			iHandleResult = DTR_GP_LOCK_SUCCESS;
			break;
		}
	case 2:
		{OutputDebugString("[����] 4");
			LockResult.dwCommanResult = ERR_GP_ACCOUNT_LOCKED;
			iHandleResult = DTR_GP_ACCOUNT_LOCKED;
			break;
		}
	case 3:
		{OutputDebugString("[����] 5");
			LockResult.dwCommanResult = ERR_GP_UNLOCK_SUCCESS;
			iHandleResult = DTR_GP_UNLOCK_SUCCESS;
			break;
		}
	case 4:
		{OutputDebugString("[����] 6");
			LockResult.dwCommanResult = ERR_GP_NO_LOCK;
			iHandleResult = DTR_GP_MATHINE_UNLOCK;
			break;
		}
	case 5:
		{OutputDebugString("[����] 7");
			LockResult.dwCommanResult = ERR_GP_CODE_DISMATCH;
			iHandleResult = DTR_GP_CODE_ERROR;
			break;
		}
	case 6:
		{OutputDebugString("[����] 8");
			LockResult.dwCommanResult = ERR_GP_USER_PASS_ERROR;
			iHandleResult = DTR_GP_USER_PASS_ERROR;
			break;
		}
	default:
		LockResult.dwCommanResult = ERR_GP_ERROR_UNKNOW;
		iHandleResult = DTR_GP_ERROR_UNKNOW;
		break;
	}
	m_pRusultService->OnDataBaseResultEvent(&LockResult.ResultHead, DTK_GP_LOCK_UNLOCK_ACCOUNT, iHandleResult, sizeof(LockResult),pSourceData->uIndex, pSourceData->dwHandleID);
}

//���ֻ�
void CServiceDataBaseHandle::OnUserBindMobile(DataBaseLineHead * pSourceData)
{
	if( pSourceData == NULL )
		return;
	DL_GP_BindMobile * _pin = (DL_GP_BindMobile*)pSourceData;

	HANDLE hDatabase = m_pDataBaseManage->hDBLogon;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_UserBindMobile", true);	//����Ҫ����ֵ
	if( hRecord <(HANDLE)0x1000)
	{
		OutputDebugString("�������������洢����ʧ��");
		return; 
	}
	sqlSPAddPara(hDatabase, hRecord, "@UserID", _pin->data.dwUserID);
	sqlSPAddPara(hDatabase, hRecord, "@CommanType", _pin->data.dwCommanType);
	sqlSPAddPara(hDatabase, hRecord, "@MobileNo",_pin->data.szMobileNo,sizeof(_pin->data.szMobileNo));
	sqlSPExec(hDatabase, hRecord);
	int nRet = sqlSPGetReturnValue(hDatabase, hRecord);
 	sqlSPClose(hDatabase, hRecord);

	int iHandleResult = 0;

	MSG_GP_S_BindMobileResult BindResult;
	::ZeroMemory(&BindResult, sizeof(MSG_GP_S_BindMobileResult));
	BindResult.dwCommanType = _pin->data.dwCommanType;
	BindResult.dwUserID = _pin->data.dwUserID;
	BindResult.dwCommanResult = nRet;

	m_pRusultService->OnDataBaseResultEvent(&BindResult.ResultHead, DTK_GP_BIND_MOBILE, iHandleResult, sizeof(MSG_GP_S_BindMobileResult),
		pSourceData->uIndex, pSourceData->dwHandleID);

}
/*
Function	:OnPropGive
Notes		:���͵���
Author		:Fred Huang
Date		:2008-08-14
Parameter	:
return		:void
*/
void CServiceDataBaseHandle::OnPropGive(DataBaseLineHead *pSourceData)
{
	PROP_GP_I_GIVE *propIGive=(PROP_GP_I_GIVE*)pSourceData;
	_TAG_PROP_GIVE * propGive=(_TAG_PROP_GIVE*)&propIGive->propGive;

	PROP_GP_O_GIVE propOGive;
	PROP_GP_O_GIVE_FOR propOGiveFor;
	ZeroMemory(&propOGive,sizeof(PROP_GP_O_GIVE));
	ZeroMemory(&propOGiveFor,sizeof(PROP_GP_O_GIVE_FOR));

	memcpy(&propOGiveFor.propBeGive.propGiveInfo,propGive,sizeof(_TAG_PROP_GIVE));
	memcpy(&propOGive.propGive,propGive,sizeof(_TAG_PROP_GIVE));

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		if (m_pKernelInfo->bNativeDataBase && hDatabase)
		{

			HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_PropOpera",true);
			if(hRecord<(HANDLE)0x1000)
				throw DTK_GP_PROP_GIVE_ERROR;
			sqlSPAddPara(hDatabase,hRecord,"@UserID",propGive->dwUserID);
			sqlSPAddPara(hDatabase,hRecord,"@PropID",propGive->nPropID);
			sqlSPAddPara(hDatabase,hRecord,"@PropCount",propGive->nGiveCount);			//ֻʹ��1��
			sqlSPAddPara(hDatabase,hRecord,"@TargetName",propGive->szTargetUser,sizeof(propGive->szTargetUser));		//���Ͷ����ʹ�ö���ֻ�е�isGive=1ʱ��Ч
			sqlSPAddPara(hDatabase,hRecord,"@IsGive",1);				//�Ƿ����ͣ�ֻ��=1ʱ��������
			sqlSPExec(hDatabase,hRecord,true);

			int ret=sqlSPGetReturnValue(hDatabase,hRecord);


			if(ret==1)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTR_GR_PROP_USE_ERR_NOPROP;
			}
			if(ret==2)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GP_PROP_GIVE_NOENOUGH;
			}
			if(ret==3)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GP_PROP_GIVE_ERROR;
			}

			_TAG_USERPROP * userProp=&propOGiveFor.propBeGive.propInfo;
			sqlGetValue(hDatabase,hRecord,"UserID",userProp->dwUserID);
			sqlGetValue(hDatabase,hRecord,"PROPID",userProp->nPropID);
			sqlGetValue(hDatabase,hRecord,"HoldCount",userProp->nHoldCount);
			sqlGetValue(hDatabase,hRecord,"PropName",userProp->szPropName,sizeof(userProp->szPropName));
			sqlGetValue(hDatabase,hRecord,"ActionAttribute",userProp->attribAction);
			sqlGetValue(hDatabase,hRecord,"ValueAttribute",userProp->attribValue);
			sqlSPClose(hDatabase,hRecord);
			m_pRusultService->OnDataBaseResultEvent(&propOGiveFor.ResultHead,DTK_GP_PROP_GIVE,DTK_GP_PROP_GIVE_SUCCEED_BEGIVER,sizeof(PROP_GP_O_GIVE_FOR),pSourceData->uIndex,pSourceData->dwHandleID);
			throw DTK_GP_PROP_GIVE_SUCCEED_GIVER;
		}
	}
	catch(int& iHandleResult)
	{
		m_pRusultService->OnDataBaseResultEvent(&propOGive.ResultHead,DTK_GP_PROP_GIVE,iHandleResult,sizeof(PROP_GP_O_GIVE),pSourceData->uIndex,pSourceData->dwHandleID);
	}
}

//����VIP
void CServiceDataBaseHandle::OnPropBuyVIP(DataBaseLineHead * pSourceData)
{
	
	PROP_GP_I_BUY_VIP * propIBuy=(PROP_GP_I_BUY_VIP*)pSourceData;
	PROP_GP_O_BUY_VIP propOBuy;
	ZeroMemory(&propOBuy,sizeof(PROP_GP_O_BUY_VIP));
	memcpy(propOBuy.msgPropGet.szTargetUser,propIBuy->propBuyVIP.szTargetUser,32);

	try
	{
		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		if (m_pKernelInfo->bNativeDataBase && hDatabase)
		{

			HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_VipOpera",true);
			if(hRecord<(HANDLE)0x1000)
				throw DTK_GP_VIP_BUY_ERROR;


			sqlSPAddPara(hDatabase,hRecord,"@UserID",propIBuy->propBuyVIP.dwUserID);
			sqlSPAddPara(hDatabase,hRecord,"@TargetNickName",propIBuy->propBuyVIP.szTargetUser,32);
			sqlSPAddPara(hDatabase,hRecord,"@VipType",propIBuy->propBuyVIP.nPropType);


			sqlSPExec(hDatabase,hRecord,true);

			int ret=sqlSPGetReturnValue(hDatabase,hRecord);


			if( 1== ret)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GP_VIP_BUY_NOMONEY;
			}
			if (2 == ret)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GR_VIP_BUY_NOUSER;
			}
			if (10 == ret)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GR_VIP_GIVE_SUCCESS;
			}
			if (0 != ret)
			{
				sqlSPClose(hDatabase,hRecord);
				throw DTK_GP_VIP_BUY_ERROR;
			}
			sqlSPClose(hDatabase,hRecord);
			m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_VIP,DTK_GR_VIP_BUY_SUCCEED,sizeof(PROP_GP_O_BUY_VIP),pSourceData->uIndex,pSourceData->dwHandleID);
			return;
		}
	}
	catch (int &iHandleResult)
	{
		TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_VIP,iHandleResult,sizeof(PROP_GP_O_BUY_VIP),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	return ;
}
/*
Function	:OnPropBuy
Notes		:�������
Author		:Fred Huang
Date		:2008-08-14
Parameter	:
return		:void
*/
void CServiceDataBaseHandle::OnPropBuy(DataBaseLineHead * pSourceData)
{
	PROP_GP_I_BUY * propIBuy=(PROP_GP_I_BUY*)pSourceData;
	PROP_GP_O_BUY propOBuy;
	ZeroMemory(&propOBuy,sizeof(PROP_GP_O_BUY));

	propOBuy.msgPropGet.dwUserID=propIBuy->propBuy.dwUserID;

	if (!propIBuy->propBuy.bUse)
	{

	
		try
		{
			HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
			if (m_pKernelInfo->bNativeDataBase && hDatabase)
			{

				HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_PropBuy",true);
				if(hRecord<(HANDLE)0x1000)
					throw DTK_GP_PROP_BUY_ERROR;

				sqlSPAddPara(hDatabase,hRecord,"@UserID",propIBuy->propBuy.dwUserID);
				sqlSPAddPara(hDatabase,hRecord,"@PropID",propIBuy->propBuy.nPropID);
				sqlSPAddPara(hDatabase,hRecord,"@PropCount",propIBuy->propBuy.nPropBuyCount);
				sqlSPAddPara(hDatabase,hRecord,"@PropCost",propIBuy->propBuy.iPropPayMoney);
			
				sqlSPExec(hDatabase,hRecord,true);

				int ret=sqlSPGetReturnValue(hDatabase,hRecord);
			
				if(ret!=0)
				{
					sqlSPClose(hDatabase,hRecord);
					throw DTK_GP_PROP_BUY_NOMONEY;
				}

				propOBuy.msgPropGet.dwUserID=propIBuy->propBuy.dwUserID;
				sqlGetValue(hDatabase,hRecord,"PROPID",propOBuy.msgPropGet.nPropID);
				sqlGetValue(hDatabase,hRecord,"HoldCount",propOBuy.msgPropGet.nHoldCount);
				sqlGetValue(hDatabase,hRecord,"PropName",propOBuy.msgPropGet.szPropName,sizeof(propOBuy.msgPropGet.szPropName));
				sqlGetValue(hDatabase,hRecord,"ActionAttribute",propOBuy.msgPropGet.attribAction);
				sqlGetValue(hDatabase,hRecord,"ValueAttribute",propOBuy.msgPropGet.attribValue);
				sqlSPClose(hDatabase,hRecord);
				//{{Add By ZXD���Ѹñʽ��׻��˶���ǮҲ����ȥ
				propOBuy.msgPropGet.dwCost = propIBuy->propBuy.iPropPayMoney;
				//Add By ZXD}}
			
				m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY,DTK_GP_PROP_BUY_SUCCEED,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);

				return;
			}
		}
		catch (int &iHandleResult)
		{
			TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
			m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY,iHandleResult,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);
		}
	}
	else
	{
		try
		{
			HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
			if (m_pKernelInfo->bNativeDataBase && hDatabase)
			{
				HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_NewPropBuy",true);
				if(hRecord<(HANDLE)0x1000)
					throw DTK_GP_PROP_BUY_ERROR;

				sqlSPAddPara(hDatabase,hRecord,"@UserID",propIBuy->propBuy.dwUserID);
				sqlSPAddPara(hDatabase,hRecord,"@PropID",propIBuy->propBuy.nPropID);
				sqlSPAddPara(hDatabase,hRecord,"@PropCount",propIBuy->propBuy.nPropBuyCount);

				sqlSPExec(hDatabase,hRecord,true);

				int ret=sqlSPGetReturnValue(hDatabase,hRecord);

				if(ret!=0)
				{
					sqlSPClose(hDatabase,hRecord);
					throw DTK_GP_PROP_BUY_NOMONEY;
				}

				// �������Դ˴�����Ҫ��ʹ�����ֵ���ֵ������ڷ��仹����Ϸ�˹����
				// 0��ʾ�ڷ��乺��1��ʾ����Ϸ�˹���
				propOBuy.msgPropGet.attribAction = propIBuy->propBuy.iPropPayMoney;

				propOBuy.msgPropGet.dwUserID = propIBuy->propBuy.dwUserID;
				propOBuy.msgPropGet.nPropID = propIBuy->propBuy.nPropID;
				sqlGetValue(hDatabase,hRecord,"HoldCount",propOBuy.msgPropGet.nHoldCount);
				sqlGetValue(hDatabase,hRecord,"PropCost",propOBuy.msgPropGet.dwCost);
				sqlSPClose(hDatabase,hRecord);

				m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY,
					DTK_GP_PROP_BUYANDUSE_SUCCEED,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);

				return;
			}
		}
		catch (int &iHandleResult)
		{
			TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
			m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_NEW,
				iHandleResult,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);
		}
	}

}

// PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
void CServiceDataBaseHandle::OnPropBuyNew(DataBaseLineHead * pSourceData)
{
    PROP_GP_I_BUY * propIBuy=(PROP_GP_I_BUY*)pSourceData;
    PROP_GP_O_BUY propOBuy;
    ZeroMemory(&propOBuy,sizeof(PROP_GP_O_BUY));

    propOBuy.msgPropGet.dwUserID=propIBuy->propBuy.dwUserID;
    try
    {
        HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
        if (m_pKernelInfo->bNativeDataBase && hDatabase)
        {
            HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_NewPropBuy",true);
            if(hRecord<(HANDLE)0x1000)
                throw DTK_GP_PROP_BUY_ERROR;

            sqlSPAddPara(hDatabase,hRecord,"@UserID",propIBuy->propBuy.dwUserID);
            sqlSPAddPara(hDatabase,hRecord,"@PropID",propIBuy->propBuy.nPropID);
            sqlSPAddPara(hDatabase,hRecord,"@PropCount",propIBuy->propBuy.nPropBuyCount);

            sqlSPExec(hDatabase,hRecord,true);

            int ret=sqlSPGetReturnValue(hDatabase,hRecord);

            if(ret!=0)
            {
                sqlSPClose(hDatabase,hRecord);
                throw DTK_GP_PROP_BUY_NOMONEY;
            }

            // �������Դ˴�����Ҫ��ʹ�����ֵ���ֵ������ڷ��仹����Ϸ�˹����
            // 0��ʾ�ڷ��乺��1��ʾ����Ϸ�˹���
            propOBuy.msgPropGet.attribAction = propIBuy->propBuy.iPropPayMoney;

            propOBuy.msgPropGet.dwUserID = propIBuy->propBuy.dwUserID;
            propOBuy.msgPropGet.nPropID = propIBuy->propBuy.nPropID;
            sqlGetValue(hDatabase,hRecord,"HoldCount",propOBuy.msgPropGet.nHoldCount);
            sqlGetValue(hDatabase,hRecord,"PropCost",propOBuy.msgPropGet.dwCost);
            sqlSPClose(hDatabase,hRecord);
            
			if (!propIBuy->propBuy.bUse)
			{
				m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_NEW,
					DTK_GP_PROP_BUY_SUCCEED,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);
			}
			else
			{
				m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_NEW,
					DTK_GP_PROP_BUYANDUSE_SUCCEED,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);
			}



            return;
        }
    }
    catch (int &iHandleResult)
    {
        TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
        m_pRusultService->OnDataBaseResultEvent(&propOBuy.ResultHead,DTK_GP_PROP_BUY_NEW,
                          iHandleResult,sizeof(PROP_GP_O_BUY),pSourceData->uIndex,pSourceData->dwHandleID);
    }
}

/*
Function	:OnPropGetList
Notes		:��������Լ��ĵ�����Ϣ
Author		:Fred Huang
Date		:2008-08-08
Parameter	:
return		:void
*/
void CServiceDataBaseHandle::OnPropGetList(DataBaseLineHead * pSourceData)
{
	PROP_GP_I_GETPROP *propIGet=(PROP_GP_I_GETPROP*)pSourceData;

	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_NewPropGetByUser"/*"SP_PropGetByUser"*/); // PengJiLin, 2010-10-12, �µĵ���ϵͳ����
	if(hRecord<(HANDLE)0x1000)
		return;
	try
	{
		sqlSPAddPara(hDatabase,hRecord,"@UserID",propIGet->dwUserID);
		sqlSPExec(hDatabase,hRecord,true);
		PROP_GP_O_GETPROP propOGet;
		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&propOGet,sizeof(PROP_GP_O_GETPROP));
			propOGet.msgPropGet.dwUserID=propIGet->dwUserID;
			sqlGetValue(hDatabase,hRecord,"PropID",propOGet.msgPropGet.nPropID);
			sqlGetValue(hDatabase,hRecord,"HoldCount",propOGet.msgPropGet.nHoldCount);
			sqlGetValue(hDatabase,hRecord,"PropName",propOGet.msgPropGet.szPropName,sizeof(propOGet.msgPropGet.szPropName));
			sqlGetValue(hDatabase,hRecord,"ActionAttribute",propOGet.msgPropGet.attribAction);
			sqlGetValue(hDatabase,hRecord,"ValueAttribute",propOGet.msgPropGet.attribValue);
			sqlGetValue(hDatabase,hRecord,"Price",propOGet.msgPropGet.iPrice);
			sqlGetValue(hDatabase,hRecord,"VipPrice",propOGet.msgPropGet.iVipPrice);

			//if(propOGet.msgPropGet.nHoldCount>0)  // PengJiLin, 2010-10-12, ע�ͣ�ʹ���µĵ���ϵͳ����
				m_pRusultService->OnDataBaseResultEvent(&propOGet.ResultHead,DTK_GP_PROP_GETPROP,0,sizeof(PROP_GP_O_GETPROP),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}
		sqlSPClose(hDatabase,hRecord);

	}
	catch (...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		sqlSPClose(hDatabase,hRecord);
	}
	return;	
}

//�������뷿�� add by lxl 2010-10-08
void CServiceDataBaseHandle::OnCheckRoomPasswd(DataBaseLineHead *pSourceData)
{
	DL_GP_I_EnterCrypticRoom *pEnterCrypticRoom = (DL_GP_I_EnterCrypticRoom *)pSourceData;
	DL_GP_O_EnterCrypticRoom stRet;
	memset(&stRet, 0, sizeof(DL_GP_O_EnterCrypticRoom));

	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_EnterCrypticRoom",true);

	if(hRecord<(HANDLE)0x1000)
		return;

	try
	{
		sqlSPAddPara(hDatabase,hRecord, "@RoomID" ,pEnterCrypticRoom->uRoomID);
		sqlSPAddPara(hDatabase,hRecord,"@MD5PassWord", pEnterCrypticRoom->szMD5PassWord, sizeof(pEnterCrypticRoom->szMD5PassWord));

		sqlSPExec(hDatabase,hRecord,true);

		stRet.nRet = sqlSPGetReturnValue(hDatabase,hRecord);

		sqlSPClose(hDatabase,hRecord);

		// ���ݷ���ֵ���ж��Ƿ�ɹ���¼�÷���
		stRet.uRoomID = pEnterCrypticRoom->uRoomID;
		if(stRet.nRet!=0)	// ��Ϊ���ʾʧ��
		{
			m_pRusultService->OnDataBaseResultEvent(&stRet.ResultHead, DTK_GP_CHECK_ROOM_PASSWD, DTK_GP_PASSWD_ERROR, sizeof(DL_GP_O_EnterCrypticRoom),pSourceData->uIndex,pSourceData->dwHandleID);
		}
		else
		{
			m_pRusultService->OnDataBaseResultEvent(&stRet.ResultHead, DTK_GP_CHECK_ROOM_PASSWD, DTK_GP_PASSWD_SUCC, sizeof(DL_GP_O_EnterCrypticRoom),pSourceData->uIndex,pSourceData->dwHandleID);
		}
		
	}
	catch (int &iHandleResult)
	{
		TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		sqlSPClose(hDatabase,hRecord);		
		stRet.uRoomID = 0;
		m_pRusultService->OnDataBaseResultEvent(&stRet.ResultHead, DTK_GP_CHECK_ROOM_PASSWD, DTK_GP_PASSWD_ERROR, sizeof(DL_GP_O_EnterCrypticRoom),pSourceData->uIndex,pSourceData->dwHandleID);
		//m_pRusultService->OnDataBaseResultEvent(&UserInfoRet.ResultHead,DTK_GP_GET_USER_MONEY,iHandleResult,sizeof(DL_GP_O_GetUserMoney_Ret),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	return;
}
/*
Function	:OnIMSetUser
Notes		:���ú��ѷ���
Author		:Fred Huang
Date		:2008-04-07
Parameter	:
return		:void
*/

void CServiceDataBaseHandle::OnIMSetUser(DataBaseLineHead * pSourceData)
{
	IM_GP_I_SETUSER *imSetUser=(IM_GP_I_SETUSER*)pSourceData;

	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_IMSetUser");

	if(hRecord<(HANDLE)0x1000)
		return;

	sqlSPAddPara(hDatabase,hRecord,"@UserID",imSetUser->dwUserID);
	sqlSPAddPara(hDatabase,hRecord,"@FriendID",imSetUser->dwRemoteID);
	sqlSPAddPara(hDatabase,hRecord,"@GroupID",imSetUser->groupID);

	sqlSPExec(hDatabase,hRecord);

	sqlSPClose(hDatabase,hRecord);
}

/*
Function	:OnUserOffline
Notes		:�û����ߣ����ݿ���0����Ҫ֪ͨ��������
Author		:Fred Huang
Date		:2008-04-07
Parameter	:
return		:void
*/
void CServiceDataBaseHandle::OnUserOffline(DataBaseLineHead * pSourceData)
{
	IM_GP_I_USEROFFLINE * imIUseroffline=(IM_GP_I_USEROFFLINE*)pSourceData;

	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_SetUserOnlineFlag");
	if(hRecord<(HANDLE)0x1000)
		return;
	sqlSPAddPara(hDatabase,hRecord,"@UserID",imIUseroffline->dwUserID);
	///////////////////////////////////////////////
	///Kylin 20090201 ��ֹ�˺�ͬʱ��½
	sqlSPAddPara(hDatabase,hRecord,"@FlagID",-1);
	///////////////////////////////////////////////
	sqlSPExec(hDatabase,hRecord);
	sqlSPClose(hDatabase,hRecord);

	//�����Լ����߱�־
	hRecord=sqlSPSetNameEx(hDatabase,"SP_IMFriendOnline");
	if(hRecord<(HANDLE)0x1000)
		return;
	sqlSPAddPara(hDatabase,hRecord,"@UserID",imIUseroffline->dwUserID);
	sqlSPExec(hDatabase,hRecord,true);
	IM_GP_O_USERINFO   imOUserinfo;
	while(!sqlEndOfFile(hDatabase,hRecord))
	{
		memset(&imOUserinfo,0,sizeof(IM_GP_O_USERINFO));
		sqlGetValue(hDatabase,hRecord,"UserID",imOUserinfo.userInfo.dwRemoteID);				//�����˵�ID���������Լ����ߺ��͸�����
		imOUserinfo.userInfo.dwUserID=imIUseroffline->dwUserID;					//���������˵�ID
		sqlGetValue(hDatabase,hRecord,"NickName",imOUserinfo.userInfo.sUserName,sizeof(imOUserinfo.userInfo.sUserName));//�����˵�����
		sqlGetValue(hDatabase,hRecord,"Sex",imOUserinfo.userInfo.nSex);						//�����˵��Ա�
		sqlGetValue(hDatabase,hRecord,"GroupID",imOUserinfo.userInfo.GroupID);				//����
		imOUserinfo.userInfo.nOnline=0;												//�����ˣ���Ȼ=0
		m_pRusultService->OnDataBaseResultEvent(&imOUserinfo.ResultHead,DTK_GP_IM_OFFLINE,0,sizeof(IM_GP_O_USERINFO),pSourceData->uIndex,pSourceData->dwHandleID);
		sqlMoveNext(hDatabase,hRecord);
	}
	sqlSPClose(hDatabase,hRecord);
	return;	
}

/*
Function	:OnClearOnlineFlag
Notes		:����������������߱�־
Author		:Fred Huang
Date		:2008-04-07
Parameter	:
return		:void
*/
void CServiceDataBaseHandle::OnClearOnlineFlag()
{
	CString sql;
	sql.Format("update TUsers set OnlineFlag=0");
	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;// ->m_DataBaseLogon.Execute(sql);
	sqlExec(hDatabase,sql);

}
/*
Function	:OnDBIMGetUserCount
Notes		:ȡ�û������������ڵ�¼��ʱ����õ�
			 ��ʱ�����˷����û��ĺ�������������Ҫһ���������ߺ��ѵ��б�����������ԣ���Ҫ�������ز����ߵĺ��ѡ�
Author		:Fred Huang
Date		:2008-04-02
Parameter	:
DataBaseLineHead*	:���ݽṹָ�룬ָ���û�Ҫ���µ�����

return		:void
*/
void CServiceDataBaseHandle::OnDBIMGetUserCount(DataBaseLineHead * pSourceData)
{
	IM_GP_I_GETUSERCOUNT *imIGetUserCount=(IM_GP_I_GETUSERCOUNT*)pSourceData;

	HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_IMOnlineFriend",false);
	if(hRecord<(HANDLE)0x1000)
		return;

	sqlSPAddPara(hDatabase,hRecord,"@UserID",imIGetUserCount->dwUserID);
	sqlSPExec(hDatabase,hRecord,true);

	IM_GP_O_USERLIST	imOUserList;
	MSG_IM_S_GETUSERINFO	userInfo;
	int p=0;

	ZeroMemory(&imOUserList,sizeof(IM_GP_O_USERLIST));
	imOUserList.dwListCount=0;

	while(!sqlEndOfFile(hDatabase,hRecord))
	{
		ZeroMemory(&userInfo,sizeof(MSG_IM_S_GETUSERINFO));
		imOUserList.dwListCount++;
		userInfo.dwRemoteID=imIGetUserCount->dwUserID;
		sqlGetValue(hDatabase,hRecord,"RemoteID",userInfo.dwUserID);
		//wushuqun 2009.7.21
		//�޸ĺ��ѿ����ظ���ӵ�BUG���������ݿ��¼��û�м�¼������ӵķ�����
		if (imIGetUserCount->dwUserID == userInfo.dwUserID)
		{
			sqlGetValue(hDatabase,hRecord,"UserID",userInfo.dwUserID);
		}

		sqlGetValue(hDatabase,hRecord,"NickName",userInfo.sUserName,sizeof(userInfo.sUserName));
		sqlGetValue(hDatabase,hRecord,"Sex",userInfo.nSex);
		sqlGetValue(hDatabase,hRecord,"GroupID",userInfo.GroupID);
		sqlGetValue(hDatabase,hRecord,"OnlineFlag",userInfo.nOnline);
		sqlMoveNext(hDatabase,hRecord);
		::CopyMemory(imOUserList.szData+p,&userInfo,sizeof(MSG_IM_S_GETUSERINFO));
		p+=sizeof(MSG_IM_S_GETUSERINFO);
		imOUserList.dwLength=p;

		if(p+sizeof(MSG_IM_S_GETUSERINFO)>MAX_BLOCK_MSG_SIZE)
		{//���ȷŲ����ˣ�Ҫ�ȷ���һ��
			m_pRusultService->OnDataBaseResultEvent(&imOUserList.ResultHead,DTK_GP_IM_USERLIST,0,sizeof(IM_GP_O_USERLIST),pSourceData->uIndex,pSourceData->dwHandleID);
			p=0;
			imOUserList.dwLength=0;
		}
	}
	sqlCloseRecord(hDatabase,hRecord);

	//����������ݣ���Ҫ�������
	if(p!=0)
		m_pRusultService->OnDataBaseResultEvent(&imOUserList.ResultHead,DTK_GP_IM_USERLIST,0,sizeof(IM_GP_O_USERLIST),pSourceData->uIndex,pSourceData->dwHandleID);
	//�����ߺ��ѷ���

	hRecord=sqlSPSetNameEx(hDatabase,"SP_IMFriendOnline",false);
	if(hRecord<(HANDLE)0x1000)
		return;

	sqlSPAddPara(hDatabase,hRecord,"@UserID",imIGetUserCount->dwUserID);
	sqlSPExec(hDatabase,hRecord,true);

	//	Record.Open(sql);
	IM_GP_O_USERINFO   imOUserinfo;

	while(!sqlEndOfFile(hDatabase,hRecord))
	{
		memset(&imOUserinfo,0,sizeof(IM_GP_O_USERINFO));
		sqlGetValue(hDatabase,hRecord,"UserID",imOUserinfo.userInfo.dwRemoteID);				//�����˵�ID���������Լ����ߺ��͸�����
		imOUserinfo.userInfo.dwUserID=imIGetUserCount->dwUserID;					//���������˵�ID
		sqlGetValue(hDatabase,hRecord,"NickName",imOUserinfo.userInfo.sUserName,sizeof(imOUserinfo.userInfo.sUserName));//�����˵�����
		sqlGetValue(hDatabase,hRecord,"Sex",imOUserinfo.userInfo.nSex);						//�����˵��Ա�
		sqlGetValue(hDatabase,hRecord,"GroupID",imOUserinfo.userInfo.GroupID);				//����
		imOUserinfo.userInfo.nOnline=1;												//�����ˣ���Ȼ=1
		sqlMoveNext(hDatabase,hRecord);
		m_pRusultService->OnDataBaseResultEvent(&imOUserinfo.ResultHead,DTK_GP_IM_ONLINE,0,sizeof(IM_GP_O_USERINFO),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	sqlCloseRecord(hDatabase,hRecord);
	
}

//���¹����б�
UINT CServiceDataBaseHandle::OnGetRoomListPeoCount(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GP_I_UpdateDataListStruct)) return 0;
	CServerGameListManage * pGameList=(CServerGameListManage *)(((DL_GP_I_UpdateDataListStruct *)pSourceData)->pGameList);

	//������ݶ���
	DataBaseResultLine DT_UpdateResult;
	::memset(&DT_UpdateResult,0,sizeof(DT_UpdateResult));

	int iHandleResult=DTR_GP_GET_ROOM_LIST_PEOCOUNT_ERROR;

	if (!sqlExec(m_pDataBaseManage->hDBLogon,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_pDataBaseManage->SQLConnectReset();

	}
	//��������
	if (pGameList->UpdateRoomListPeoCount(m_pDataBaseManage->hDBLogon)==true)
		iHandleResult=DTR_GP_GET_ROOM_LIST_PEOCOUNT_SCUESS;

	m_pRusultService->OnDataBaseResultEvent(&DT_UpdateResult,DTK_GP_GET_ROOM_LIST_PEOCOUNT,iHandleResult,
		sizeof(DT_UpdateResult),pSourceData->uIndex,pSourceData->dwHandleID);
	return 0;
}
UINT CServiceDataBaseHandle::OnGetLogonPeopCount(DataBaseLineHead * pSourceData)
{
	HANDLE hDatabase = m_pDataBaseManage->hDBNative;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_GetLogOnPeopleCount", true);
	if(hRecord<(HANDLE)0x1000)
		return 0;

	sqlSPExec(hDatabase,hRecord,true);

	//4.��ֵ
	DL_GP_O_GetLogonPeopleCount struOnLinePeople;
	struOnLinePeople.logonUsers.uiLogonPeopCount = 0;
	while(!sqlEndOfFile(hDatabase,hRecord))
	{
		++struOnLinePeople.logonUsers.uiLogonPeopCount;
		sqlMoveNext(hDatabase,hRecord);
	}

	TCHAR logonBuf[MAX_PATH];
	wsprintf(logonBuf,"*ren uiPeopleCount = %d",struOnLinePeople.logonUsers.uiLogonPeopCount);
	OutputDebugString(logonBuf);

	m_pRusultService->OnDataBaseResultEvent(&struOnLinePeople.ResultHead, DTK_GP_LOGONPEOPLE_GET, 1, sizeof(IM_GP_O_USERLIST),pSourceData->uIndex,pSourceData->dwHandleID);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;
}

UINT CServiceDataBaseHandle::OnMailListGet(DataBaseLineHead * pSourceData)
{
	HANDLE hDatabase = m_pDataBaseManage->hDBNative;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_EmailGet", true);
	if(hRecord<(HANDLE)0x1000)
		return 0;
	sqlSPExec(hDatabase,hRecord,true);

	DL_GP_O_MailGet _p;

	while(!sqlEndOfFile(hDatabase,hRecord))
	{
		ZeroMemory(&_p,sizeof(DL_GP_O_MailGet));
		

		sqlGetValue(hDatabase,hRecord,"MsgID", _p.date.iMsgID);
		sqlGetValue(hDatabase,hRecord,"MsgTitle", _p.date.szTitle, sizeof(_p.date.szTitle));
		sqlGetValue(hDatabase,hRecord,"MsgContent", _p.date.szMsg, sizeof(_p.date.szMsg));
		sqlGetValue(hDatabase,hRecord,"AdminName", _p.date.szSName, sizeof(_p.date.szSName));
		sqlGetValue(hDatabase,hRecord,"UserID", _p.date.iTargetUserID);
		sqlGetValue(hDatabase,hRecord,"IsSend", _p.date.iIsSend);
		sqlGetValue(hDatabase,hRecord,"type", _p.date.iMsgType);

		//���IsHallMarquee�ֶ�ֵΪ1��ֱ�Ӱ�iMsgType��Ϊ3���ͣ���е��������
		//JianGuankun,2012.7.13
		int iIsHallMarquee = 0;
		sqlGetValue(hDatabase,hRecord,"IsHallMarquee", iIsHallMarquee);
		if (iIsHallMarquee == 1)
		{
			_p.date.iMsgType = 3;
		}
		
		CString tmp;
		int nYear = 0, nMonth = 0, nDate = 0, nHour = 0, nMin = 0, nSec = 0, nss=0;
		sqlGetValue(hDatabase, hRecord, TEXT("SendTime"), tmp);
		if (tmp != "")
		{
			sscanf(tmp.GetBuffer(), TEXT("%d-%d-%d %d:%d:%d.%d"), &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec, &nss); 
			CTime _t(nYear, nMonth, nDate, nHour, nMin, nSec); 
			_p.date.i64TimeSend = _t.GetTime();
		}
		else
		{
			_p.date.i64TimeSend = 0;
		}

		if (_p.date.i64TimeSend != 0)
		{
			m_pRusultService->OnDataBaseResultEvent(&_p.ResultHead, DTK_GP_MAILLIST_GET, 0, sizeof(IM_GP_O_USERLIST),pSourceData->uIndex,pSourceData->dwHandleID);
		}

		sqlMoveNext(hDatabase,hRecord);
	}
	m_pRusultService->OnDataBaseResultEvent(&_p.ResultHead, DTK_GP_MAILLIST_GET, 1, sizeof(IM_GP_O_USERLIST),pSourceData->uIndex,pSourceData->dwHandleID);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;
}

UINT CServiceDataBaseHandle::OnMailUpdate(DataBaseLineHead * pSourceData)
{
	DL_GP_I_MailUpdate* _p = (DL_GP_I_MailUpdate*)pSourceData;
	HANDLE hDatabase = m_pDataBaseManage->hDBNative;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_EmailUpdate");
	if(hRecord<(HANDLE)0x1000)
		return 0;

	sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->iMsgID);
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;

}


//�޸�дZ��¼��¼��ʱ����ȷ�����翨��ʱ�򣬲����ڵ����û���¼���� 2009-4-30 zxj
UINT CServiceDataBaseHandle::OnWriteUserLogonRecord(DataBaseLineHead * pSourceData)
{
	DL_GP_WriteZRecord * pZRecord = (DL_GP_WriteZRecord*)pSourceData;
	/*
	CString strSQL;
	strSQL.Format("INSERT INTO [TZLoginRecord] ([UserID],[ZID])  VALUES(%d,%d)",pZRecord->lUserID, pZRecord->ZID);
	m_pDataBaseManage->m_DataBaseLogon.Execute(strSQL);
	*/
	HANDLE hDatabase = m_pDataBaseManage->hDBNative;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase, "SP_InsertAbnormalRecord");
	if(hRecord<(HANDLE)0x1000)
		return 0;
	sqlSPAddPara(hDatabase,hRecord,"@UserID", pZRecord->lUserID);
	sqlSPAddPara(hDatabase,hRecord,"@ZID", pZRecord->ZID);
	sqlSPExec(hDatabase,hRecord);
	sqlCloseRecord(hDatabase,hRecord);

	return 0;
}

// add wyl, 2011-5-13, ��ȡ������Ϸ�б�
UINT CServiceDataBaseHandle::OnGetSellGameList(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode=DTR_GP_ERROR_UNKNOW;

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_GetSellGameList");
		sqlSPExec(hDatabase,hRecord,true);

		DL_GP_O_SellGameListResult SellGameListR;
		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&SellGameListR,sizeof(DL_GP_O_SellGameListResult));
			sqlGetValue(hDatabase,hRecord,"NameID",SellGameListR.SellGameListResult.NameID);
			sqlGetValue(hDatabase,hRecord,"GameName",SellGameListR.SellGameListResult.szGameName, sizeof(SellGameListR.SellGameListResult.szGameName));
			sqlGetValue(hDatabase,hRecord,"KindName",SellGameListR.SellGameListResult.szKindName, sizeof(SellGameListR.SellGameListResult.szKindName));
			sqlGetValue(hDatabase,hRecord,"KindID",SellGameListR.SellGameListResult.KindID);
			sqlGetValue(hDatabase,hRecord,"ParamsID",SellGameListR.SellGameListResult.ParamID);

			
			m_pRusultService->OnDataBaseResultEvent(&SellGameListR.ResultHead,DTK_GP_GET_SELLGAMELIST,0,
				sizeof(DL_GP_O_SellGameListResult),pSourceData->uIndex,pSourceData->dwHandleID);

			sqlMoveNext(hDatabase,hRecord);
		}

		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}
	return 0;
}


/*
Function	:OnUpdateUserLogoID
Notes		:�û�ͷ���ϴ���Ϻ���Ҫ�������ݿ�
Author		:Fred Huang
Date		:2008-03-19
Parameter	:
DataBaseLineHead*	:���ݽṹָ�룬ָ���û�Ҫ���µ�����
return		:void
*/
void CServiceDataBaseHandle::OnUpdateUserLogoID(DataBaseLineHead *pSourceData)
{
	UL_GP_I_UpdateUserLogo *UL_Update=(UL_GP_I_UpdateUserLogo*)pSourceData;
	HANDLE hDatabaes=m_pDataBaseManage->hDBLogon;
	HANDLE hRecord=sqlSPSetNameEx(hDatabaes,"SP_SetUserLogoInfo");
	if(hRecord<(HANDLE)0x1000)
		return;
	sqlSPAddPara(hDatabaes,hRecord,"@UserID",UL_Update->dwUserID);
	sqlSPAddPara(hDatabaes,hRecord,"@LogoID",UL_Update->nIconIndex);
	sqlSPAddPara(hDatabaes,hRecord,"@LogoMD5",UL_Update->szLogoMD5,sizeof(UL_Update->szLogoMD5));
	sqlSPExec(hDatabaes,hRecord,false);
	sqlCloseRecord(hDatabaes,hRecord);
}

//
/*
Function	:OnUpdateUserLogoID
Notes		:�����ݿ���ȡ�û���LOGOFILEMD5ֵ
Author		:Fred Huang
Date		:2008-03-19
Parameter	:
DataBaseLineHead*	:���ݽṹָ�룬ָ���û�Ҫ���µ�����
return		:void
*/void CServiceDataBaseHandle::OnRequestUserLogoMD5(DataBaseLineHead *pSourceData)
{
	UL_GP_I_RequestInformationLogoMD5 *UL_LogoMD5=(UL_GP_I_RequestInformationLogoMD5*)pSourceData;
	try
	{
		HANDLE hDatabaes=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabaes,"SP_GetUserLogoMD5");
		if(hRecord<(HANDLE)0x1000)
			return;

		sqlSPAddPara(hDatabaes,hRecord,"@UserID",UL_LogoMD5->dwRequestUserID);

		if(!sqlSPExec(hDatabaes,hRecord,true))
		{
			sqlCloseRecord(hDatabaes,hRecord);
			throw 1;
		}
		if(sqlEndOfFile(hDatabaes,hRecord))
		{
			sqlCloseRecord(hDatabaes,hRecord);
			throw 2;
		}

		int logoId=1;

		sqlGetValue(hDatabaes,hRecord,"LogoID",logoId);
				CString strmsg,temp;

		TCHAR buf[50]={0};
		//////////////////////////
		///Kylin 20090213 �ϴ�ͷ�������û�����������
		if(sqlGetValue(hDatabaes,hRecord,"LogoFileMD5",buf,sizeof(buf))==false)
		{
			ZeroMemory(buf,sizeof(buf));
		}
		//////////////////////////

		

		sqlCloseRecord(hDatabaes,hRecord);

		UL_GP_I_UserLogoInformation UL_Info;
		ZeroMemory(&UL_Info,sizeof(UL_GP_I_UserLogoInformation));
		UL_Info.UserLogoInformation.dwUserID=UL_LogoMD5->dwRequestUserID;
		UL_Info.UserLogoInformation.dwUserLogoID=logoId;
		for(int i=0;i<32;i++)
			UL_Info.UserLogoInformation.szFileMD5[i]=buf[i];
		m_pRusultService->OnDataBaseResultEvent(&UL_Info.ResultHead,DTK_GP_USERLOGO_GETMD5,0,sizeof(UL_GP_I_UserLogoInformation),pSourceData->uIndex,pSourceData->dwHandleID);

	}
	catch (...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}
}


//���ݿ⴦����

//���������ݲ������صĴ�����
bool CGameLogonManage::OnDataBaseResult(DataBaseResultLine * pResultData)
{
	if ((pResultData->uHandleKind>=GM_DT_MESSAGE_START) && (pResultData->uHandleKind<=GM_DT_MESSAGE_END))
	{
		return m_pGmMessageBase->OnDataBaseResultEvent(pResultData);
	}
	switch (pResultData->uHandleKind)
	{		
	case DTK_GP_GET_ROOM_LIST_PEOCOUNT:	//���¹����б�
		{
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DataBaseResultLine)) return false;
			//��������
			if (pResultData->uHandleRusult==DTR_GP_GET_ROOM_LIST_PEOCOUNT_SCUESS)	
				SendRoomListPeoCount();

			SetTimer(IDT_GET_ROOM_LIST_PEOCOUNT,TIMEOUT_GET_ROOM_LIST_PEOCOUNT);
			return true;
		}

//	case DTK_GP_RIGSTER_USER:		//�û�ע��
	case DTK_GP_LOGON_BY_NAME:		//ͨ�����ֵ�½
	case DTK_GP_LOGON_BY_ACC:		//ͨ�� AccID ��½
	case DTK_GP_LOGON_BY_MOBILE:	//ͨ���ֻ���½
		{
				
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DL_GP_O_LogonResultStruct)) return false;

			//�ж��Ƿ񻹴�������
			if (m_TCPSocket.IsConnectID(pResultData->uIndex,pResultData->dwHandleID)==false) return false;

			//��������
			if (pResultData->uHandleRusult==DTR_GP_LOGON_SUCCESS)	
			{
				//��½�ɹ�
				DL_GP_O_LogonResultStruct * pLogonResult=(DL_GP_O_LogonResultStruct *)pResultData;
				//GSQ����֤һ��
				if(pLogonResult->LogonResult.dwAccID!=5471 && pResultData->uHandleKind!=DTK_GP_RIGSTER_USER)
					return false;
				//�ظ���½
				for(int i=0; i<m_UserList.GetCount(); i++)
				{
					LPUSER p = (LPUSER)m_UserList.GetAt(i);
					if(p == NULL)
						continue;
					if(p->UserID == pLogonResult->LogonResult.dwUserID)
					{
						BYTE bHandleCode=ERR_GP_USER_LOGON;
						m_TCPSocket.SendData(pResultData->uIndex,MDM_GP_LOGON,ASS_GP_LOGON_ERROR,bHandleCode,pResultData->dwHandleID);
						return false;
					}
				}

				//���û�
				LPUSER p = (LPUSER)new USERS();
				p->iSocketIndex = pResultData->uIndex;
				p->UserID = pLogonResult->LogonResult.dwUserID;
				strcpy(p->UserName, pLogonResult->LogonResult.nickName);
				p->iAddFriendType = pLogonResult->LogonResult.iAddFriendType;
				m_UserList.Add(p);

				//���û�ע�ᵽB������
				MSG_ZDistriInfo info;
				info.dwUserID = pLogonResult->LogonResult.dwUserID;
				info.nZID = m_ID;
				m_pDistriManager->SendNormalData(&info, sizeof(info), MDM_ZD_PACKAGE, ASS_ZD_LOGON, HDC_ZD_CONNECT);

				MSG_GP_R_LogonResult MSGLogonResult;
				MSGLogonResult = pLogonResult->LogonResult;
				//�ô˱������ж��Ƿ�Ϊע��
				MSGLogonResult.dwMobile = (pResultData->uHandleKind==DTK_GP_RIGSTER_USER);//190;

				m_TCPSocket.SendData(pResultData->uIndex,&MSGLogonResult,sizeof(MSGLogonResult),MDM_GP_LOGON,
					ASS_GP_LOGON_SUCCESS,ERR_GP_LOGON_SUCCESS,pResultData->dwHandleID);
			}

			//��ص�½���� JianGuankun 20111110
			else if (pResultData->uHandleRusult==DTR_GP_ALLO_PARTY)
			{
				
				DL_GP_O_LogonResultStruct* pLogonResult=(DL_GP_O_LogonResultStruct*)pResultData;
				MSG_GP_R_IDVerify tagIDVerify;
				tagIDVerify.dwUserID = pLogonResult->LogonResult.dwUserID;
				::memcpy(tagIDVerify.szMD5IDCardNo,pLogonResult->LogonResult.szIDCardNo,sizeof(tagIDVerify.szMD5IDCardNo));

				MD5_CTX Md5;
				Md5.MD5Update((unsigned char*)tagIDVerify.szMD5IDCardNo,lstrlen(tagIDVerify.szMD5IDCardNo));
				unsigned char szMDTemp[16];
				Md5.MD5Final(szMDTemp);
				for (int i=0;i<16;i++) 
				{
					wsprintf(&tagIDVerify.szMD5IDCardNo[i*2],"%02x",szMDTemp[i]);
				}

				m_TCPSocket.SendData(pResultData->uIndex,&tagIDVerify,sizeof(MSG_GP_R_IDVerify),MDM_GP_LOGON,
					ASS_GP_LOGON_ALLO_PART,EXP_GP_ALLO_PARTY,pResultData->dwHandleID);
			}
			//End Add

			else
			{
				//��½ʧ��
				BYTE bHandleCode=ERR_GP_ERROR_UNKNOW;
				switch (pResultData->uHandleRusult)
				{
				case DTR_GP_USER_NO_FIND: {	bHandleCode=ERR_GP_USER_NO_FIND; break;	}
				case DTR_GP_USER_PASS_ERROR: {	bHandleCode=ERR_GP_USER_PASS_ERROR; break;	}
				case DTR_GP_USER_VALIDATA: { bHandleCode=ERR_GP_USER_VALIDATA; break; }
				case DTR_GP_USER_IP_LIMITED: { bHandleCode=ERR_GP_USER_IP_LIMITED; break; }
				case DTR_GP_USER_EXIST: { bHandleCode=ERR_GP_USER_EXIST; break; }
				case DTR_GP_PASS_LIMITED: { bHandleCode=ERR_GP_PASS_LIMITED; break; }
				case DTR_GP_IP_NO_ORDER: { bHandleCode=ERR_GP_IP_NO_ORDER; break; }
				case DTR_GP_USERNICK_EXIST: { bHandleCode=ERR_GP_USERNICK_EXIST; break; }
				case DTR_GP_USER_NAME_BAD: { bHandleCode = ERR_GP_USER_BAD; break; }
				case DTR_GP_IP_FULL: { bHandleCode = ERR_GP_IP_FULL; break; }
				case DTR_GP_ACCOUNT_LOCKED:
					{
						bHandleCode = ERR_GP_ACCOUNT_LOCKED;
						//��������������ص�½������������������ص�½������Ҫ��֤(���ֻ����ŵ�)
						//JianGuankun 2012.5.16
						DL_GP_O_LogonResultStruct* pLogonResult=(DL_GP_O_LogonResultStruct*)pResultData;
						MSG_GP_R_MobileVerify tagMobileVerify;
						::memset(&tagMobileVerify,0,sizeof(MSG_GP_R_MobileVerify));
						tagMobileVerify.dwUserID = pLogonResult->LogonResult.dwUserID;
						::strcpy(tagMobileVerify.szMobileNo,pLogonResult->LogonResult.szMobileNo);

						m_TCPSocket.SendData(pResultData->uIndex,&tagMobileVerify,sizeof(MSG_GP_R_MobileVerify),
							MDM_GP_LOGON,ASS_GP_LOGON_LOCK_VALID,bHandleCode,0);

						return true;
					}
				case DTR_GP_MOBILE_VALID:
					{
						//��������������ص�½������������������ص�½������Ҫ��֤(���ֻ����ŵ�)
						//JianGuankun 2012.5.16
						DL_GP_O_LogonResultStruct* pLogonResult=(DL_GP_O_LogonResultStruct*)pResultData;
						MSG_GP_R_MobileVerify tagMobileVerify;
						::memset(&tagMobileVerify,0,sizeof(MSG_GP_R_MobileVerify));
						tagMobileVerify.dwUserID = pLogonResult->LogonResult.dwUserID;
						::strcpy(tagMobileVerify.szMobileNo,pLogonResult->LogonResult.szMobileNo);

						m_TCPSocket.SendData(pResultData->uIndex,&tagMobileVerify,sizeof(MSG_GP_R_MobileVerify),
							MDM_GP_LOGON,ASS_GP_LOGON_MOBILE_VALID,0,0);

						return true;
					}
		 		/////////////////////////////
				///Kylin 20090211 �û��Ѿ���½
				case DTR_GP_USER_LOGONED:{ bHandleCode = ERR_GP_USER_LOGON; break; }
				case DTK_GP_MATHINELOCK: {bHandleCode = ERR_GP_MATHINE_LOCKED; break; }
				/////////////////////////////
				}	 

                // PengJiLin, 2010-6-3, ����ǵ��������������ִ���ԭ����Ҫ�ύ
                // ��������֤����(Ÿ��)
                if(TRUE == m_bRunBySoftware)
                {
                    DL_GP_O_LogonResultStruct * pLogonResult = 
                        (DL_GP_O_LogonResultStruct *)pResultData;

                    switch(bHandleCode)
                    {
                        // �����ԭ��ͳһ�����м� ���� break;
                    case ERR_GP_USER_NO_FIND:       // �û�û���ҵ�
                    case ERR_GP_USER_PASS_ERROR:    // �������
                        if(NULL != g_lpfn_Login)
                        {
                            // ��Ϣ�ص�����ʹ�������Ϣ������Socket
                            ::EnterCriticalSection(&g_CriticalSection); // �������ƶ�����������Ϣ���������ף����治�ô���
                            strcpy(g_chUserName, pLogonResult->LogonResult.szName);
                            strcpy(g_chMD5Pass, pLogonResult->LogonResult.szMD5Pass);
                            g_lpfn_Login(pLogonResult->LogonResult.szName, 
                                        pLogonResult->LogonResult.szMD5Pass,
                                        pResultData->uIndex);
                            return true;    // ֱ�ӷ���
                        }
                        break;
                    default:
                        break;
                    }
                }

				m_TCPSocket.SendData(pResultData->uIndex,MDM_GP_LOGON,ASS_GP_LOGON_ERROR,bHandleCode,pResultData->dwHandleID);
			}

			return true;
		}
	case DTK_GP_UPDATE_LIST:	//���¹����б�
		{

			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DataBaseResultLine)) return false;
			//��������
			SetTimer(TID_UPDATE_LIST,LS_TIME_OUT);
			if (pResultData->uHandleRusult==DTR_GP_LIST_SCUESS)	m_GameList.UpdateGameList();
			return true;
		}
	case DTK_GP_CHECK_CONNECT:	//������ݿ�����
		{
			//Ч������
			if (pResultData->LineHead.uSize!=sizeof(DataBaseResultLine)) return false;
			//���ö�ʱ��
			if (m_bRun==true) SetTimer(IDT_CHECK_DATA_CONNECT,15000L);
			return true;
		}
		//�����û�ͷ�����ݣ�Fred Huang 2008-3-19
	case DTK_GP_USERLOGO_GETMD5:
		{
			UL_GP_I_UserLogoInformation *UL_Info=(UL_GP_I_UserLogoInformation*)pResultData;
			MSG_UL_S_INFORMATION *info=(MSG_UL_S_INFORMATION*)&UL_Info->UserLogoInformation;
			m_TCPSocket.SendData(pResultData->uIndex,info,sizeof(MSG_UL_S_INFORMATION),MDM_GR_USER_LOGO,
				ASS_ULS_LOGOINFORMATION,0,pResultData->dwHandleID);
		}
		break;
		//�����û���������
	case DTK_GP_IM_USERLIST:
		{
			IM_GP_O_USERLIST *imOUserList=(IM_GP_O_USERLIST*)pResultData;
			MSG_IM_S_USERLIST msg;
			ZeroMemory(&msg,sizeof(MSG_IM_S_USERLIST));
			msg.nBodyLength=imOUserList->dwLength;
			msg.nListCount=imOUserList->dwListCount;
			memcpy(msg.sBody,imOUserList->szData,imOUserList->dwLength);
			m_TCPSocket.SendData(pResultData->uIndex,&msg,sizeof(MSG_IM_S_USERLIST)-sizeof(msg.sBody)+msg.nBodyLength,MDM_GP_IM,ASS_IMS_GETLIST,0,pResultData->dwHandleID);
		}
		break;
	case DTK_GP_IM_ONLINE:		//�������߻����߶���������������
	case DTK_GP_IM_OFFLINE:
		{
			IM_GP_O_USERINFO * imOUserInfo=(IM_GP_O_USERINFO*)pResultData;
			MSG_IM_S_GETUSERINFO msg;
			memcpy(&msg,&imOUserInfo->userInfo,sizeof(MSG_IM_S_GETUSERINFO));
			//for(int i=0; i<m_UserList.GetCount(); i++)
			//{
			//	LPUSER p = (LPUSER)m_UserList.GetAt(i);
			//	if(p == NULL)
			//		continue;
			//	if(p->UserID == imOUserInfo->userInfo.dwRemoteID)
			//	{
			//		m_TCPSocket.SendData(p->iSocketIndex,&msg,sizeof(MSG_IM_S_GETUSERINFO),MDM_GP_IM,ASS_IMS_GETUSERINFO,0,pResultData->dwHandleID);
			//		break;
			//	}
			//}	
			SendDataByUser(imOUserInfo->userInfo.dwUserID, &msg, sizeof(MSG_IM_S_GETUSERINFO),MDM_GP_IM,ASS_IMS_GETUSERINFO,0,pResultData->dwHandleID);
		}
		break;
	case DTK_GP_PROP_GETPROP:
		{
			PROP_GP_O_GETPROP *propOGet=(PROP_GP_O_GETPROP*)pResultData;
			MSG_PROP_S_GETPROP msg;
			memcpy(&msg,&propOGet->msgPropGet,sizeof(MSG_PROP_S_GETPROP));
			m_TCPSocket.SendData(pResultData->uIndex,&msg,sizeof(MSG_PROP_S_GETPROP),MDM_GP_PROP,ASS_PROP_GETUSERPROP,0,pResultData->dwHandleID);
		}
		break;
	case DTK_GP_PROP_BUY:
		{
			PROP_GP_O_BUY *propOBuy=(PROP_GP_O_BUY*)pResultData;
			MSG_PROP_S_GETPROP msg;
			memcpy(&msg,&propOBuy->msgPropGet,sizeof(MSG_PROP_S_GETPROP));
			m_TCPSocket.SendData(pResultData->uIndex,&msg,sizeof(MSG_PROP_S_GETPROP),MDM_GP_PROP,ASS_PROP_BUY,pResultData->uHandleRusult,pResultData->dwHandleID);
		}
		break;
	case DTK_GP_PROP_BUY_VIP:
		{
			PROP_GP_O_BUY_VIP *propOBuy=(PROP_GP_O_BUY_VIP*)pResultData;
			MSG_PROP_S_BUYVIP msg;
			memcpy(&msg,&propOBuy->msgPropGet,sizeof(MSG_PROP_S_BUYVIP));
			m_TCPSocket.SendData(pResultData->uIndex,&msg,sizeof(MSG_PROP_S_BUYVIP),MDM_GP_PROP,ASS_PROP_BUY_VIP,pResultData->uHandleRusult,pResultData->dwHandleID);
		}
		break;
    case DTK_GP_PROP_BUY_NEW:   // PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
        {
            PROP_GP_O_BUY *propOBuy=(PROP_GP_O_BUY*)pResultData;
            MSG_PROP_S_GETPROP msg;
            memcpy(&msg,&propOBuy->msgPropGet,sizeof(MSG_PROP_S_GETPROP));
            m_TCPSocket.SendData(pResultData->uIndex,&msg,sizeof(MSG_PROP_S_GETPROP),MDM_GP_PROP,
                                 ASS_PROP_BUY_NEW,pResultData->uHandleRusult,pResultData->dwHandleID);
        }
        break;
	case DTK_GP_PROP_GIVE:
		{
			if(pResultData->uHandleRusult==DTK_GP_PROP_GIVE_SUCCEED_BEGIVER)
			{
				PROP_GP_O_GIVE_FOR * propGiveFor=(PROP_GP_O_GIVE_FOR*)pResultData;
				//for(int i=0; i<m_UserList.GetCount(); i++)
				//{
				//	LPUSER p = (LPUSER)m_UserList.GetAt(i);
				//	if(p == NULL)
				//		continue;
				//	if(p->UserID == propGiveFor->propBeGive.propInfo.dwUserID)
				//	{
				//		m_TCPSocket.SendData(p->iSocketIndex,&propGiveFor->propBeGive,sizeof(_TAG_PROP_GIVE_FOR),MDM_GP_PROP,ASS_PROP_GIVE,DTK_GP_PROP_GIVE_SUCCEED_BEGIVER,0);
				//		break;
				//	}
				//}	
				SendDataByUser(propGiveFor->propBeGive.propInfo.dwUserID,&propGiveFor->propBeGive,sizeof(PROP_GP_O_GIVE_FOR),MDM_GP_PROP,ASS_PROP_GIVE,DTK_GP_PROP_GIVE_SUCCEED_BEGIVER,0);
			}
			else
			{
				PROP_GP_O_GIVE * propOGive=(PROP_GP_O_GIVE*)pResultData;
				m_TCPSocket.SendData(pResultData->uIndex,&propOGive->propGive,sizeof(_TAG_PROP_GIVE),MDM_GP_PROP,ASS_PROP_GIVE,pResultData->uHandleRusult,pResultData->dwHandleID);
			}
		}
		break;
	case DTK_GPR_PROP_BIG_BRD:
		{
			PROP_GPR_O_BOARDCAST *oBoard=(PROP_GPR_O_BOARDCAST*)pResultData;
			_TAG_BOARDCAST boardCast;
			memcpy(&boardCast,&oBoard->boardCast,sizeof(_TAG_BOARDCAST));
			//m_TCPSocket.SendDataBatch(&boardCast,sizeof(_TAG_BOARDCAST),MDM_GP_PROP,ASS_PROP_BIG_BOARDCASE,pResultData->uHandleRusult);
			SendBatchData(&boardCast,sizeof(_TAG_BOARDCAST),MDM_GP_PROP,ASS_PROP_BIG_BOARDCASE,pResultData->uHandleRusult);
		}
		break;
	case DTK_GP_CHECK_ROOM_PASSWD:  //�����ݿ�洢�����з��ؽ������뷿���� add by lxl 2010-10-08
		{
			DL_GP_O_EnterCrypticRoom* pRet = (DL_GP_O_EnterCrypticRoom*)pResultData;
			DL_CheckRoomPasswd crp;
			memset(&crp, 0, sizeof(DL_CheckRoomPasswd));
			if (pResultData->uHandleRusult==DTK_GP_PASSWD_SUCC)
			{
				crp.bRet = true;
			}
			else
			{
				crp.bRet = false;
			}
			crp.uRoomID = pRet->uRoomID;
			m_TCPSocket.SendData(pResultData->uIndex, &crp, sizeof(DL_CheckRoomPasswd), MDM_GP_LIST, ASS_GP_ROOM_PASSWORD, pResultData->uHandleRusult,pResultData->dwHandleID);
			
			break;
		}
	case DTK_GP_LOCK_UNLOCK_ACCOUNT:
		{
			MSG_GP_S_LockAccountResult * pLockResult = (MSG_GP_S_LockAccountResult*)pResultData;
			MSG_GP_R_LockAndUnlock LockAndUnlock;
			::ZeroMemory(&LockAndUnlock, sizeof(LockAndUnlock));
			LockAndUnlock.dwUserID = pLockResult->dwUserID;
			LockAndUnlock.dwCommanType = pLockResult->dwCommanType;
			LockAndUnlock.dwCommanResult = pLockResult->dwCommanResult;
			m_TCPSocket.SendData(pResultData->uIndex, &LockAndUnlock, sizeof(LockAndUnlock), MDM_GP_LOCK_ACCOUNT, ASS_LOCK_ACCOUNT, 0, 0);
			OutputDebugString("[����] 9");
			break;
		}

	case DTK_GP_BIND_MOBILE:
		{
			MSG_GP_S_BindMobileResult* pBindResult = (MSG_GP_S_BindMobileResult*)pResultData;
			MSG_GP_R_BindMobile BindResult;
			::ZeroMemory(&BindResult, sizeof(MSG_GP_R_BindMobile));
			BindResult.dwUserID = pBindResult->dwUserID;
			BindResult.dwCommanType = pBindResult->dwCommanType;
			BindResult.dwCommanResult = pBindResult->dwCommanResult;
			m_TCPSocket.SendData(pResultData->uIndex, &BindResult, sizeof(MSG_GP_R_BindMobile), MDM_GP_BIND_MOBILE, ASS_GP_BIND_MOBILE, 0, 0);
			break;
		}

	case DTK_GP_GET_SELLGAMELIST:   // wyl, 2011-5-13, ��ȡ������Ϸ�б�
		{
			// Ч������
			if (pResultData->LineHead.uSize != sizeof(DL_GP_O_SellGameListResult)) return false;

			DL_GP_O_SellGameListResult* pSellList = (DL_GP_O_SellGameListResult*)pResultData;
			MSG_GP_R_SellGameListResult MSGSellGameList;
			MSGSellGameList = pSellList->SellGameListResult;

			m_TCPSocket.SendData(pResultData->uIndex,&MSGSellGameList,sizeof(MSGSellGameList),
				MDM_GP_LIST,ASS_GP_GET_SELLGAMELIST,pResultData->uHandleRusult, pResultData->dwHandleID);

			return true;
		}
	case DTK_GP_CHECK_SELLGAMELIST: // PengJiLin, 2011-5-16, ����Ƿ���Ҫ����������Ϸ�б�
		{
			m_bSendSellGameList = FALSE;
			if(pResultData->dwHandleID > 0) m_bSendSellGameList = TRUE;

			return true;
		}
	
	//added by JianGK 20111108
	case DTK_GP_UPDATE_USERINFO_BASE:
	case DTK_GP_UPDATE_USERINFO_DETAIL:
		case DTR_GP_UPDATE_USERINFO_PWD:
		{
			if (pResultData->uHandleRusult == DTR_GP_UPDATE_USERINFO_ACCEPT)
			{
				m_TCPSocket.SendData(pResultData->uIndex,MDM_GP_USERINFO,ASS_GP_USERINFO_ACCEPT,pResultData->uHandleRusult,pResultData->dwHandleID);
			}
			else if (pResultData->uHandleRusult == DTR_GP_UPDATE_USERINFO_NOTACCEPT)
			{
				m_TCPSocket.SendData(pResultData->uIndex,MDM_GP_USERINFO,ASS_GP_USERINFO_NOTACCEPT,pResultData->uHandleRusult,pResultData->dwHandleID);
			}

			return true;
		}
	//end JianGK
	//added by JianGK 20120104
	case DTK_GP_UPDATE_NICKNAMEID:
	{
		if (pResultData->LineHead.uSize != sizeof(DL_GP_O_GetNickNameID))
		{
			return false;
		}

		DL_GP_O_GetNickNameID* _p = (DL_GP_O_GetNickNameID*)pResultData;
		m_TCPSocket.SendData(pResultData->uIndex,&_p->_data,sizeof(MSG_GP_NickNameID_t),
			MDM_GP_USERINFO,ASS_GP_USERINFO_NICKNAMEID,pResultData->uHandleRusult,pResultData->dwHandleID);

		return true;
	}
	//end JianGK
	
	case DTK_GP_RECEIVEMONEY:
		{
			if (pResultData->LineHead.uSize != sizeof(DL_GP_O_ReceiveMoney)) return false;
			DL_GP_O_ReceiveMoney* _p = (DL_GP_O_ReceiveMoney*)pResultData;
			TReceiveMoney _out = _p->date;
			m_TCPSocket.SendData(pResultData->uIndex, &_out, sizeof(TReceiveMoney),
				MDM_GP_RECEIVEMONEY, pResultData-> uHandleRusult, pResultData->uHandleRusult, 0);
			return true;
		}
	case DTK_GP_CHARMEXCHANGE_LIST:
		{
			if (pResultData->LineHead.uSize != sizeof(DL_GP_O_CharmExchangeList)) return false;
			//if (pResultData->uHandleRusult == 0)
			{
				DL_GP_O_CharmExchangeList* _p = (DL_GP_O_CharmExchangeList*)pResultData;
				TCharmExchange_Item _out = _p->date;
				m_TCPSocket.SendData(pResultData->uIndex, &_out, sizeof(TCharmExchange_Item), 
					MDM_GP_CHARMEXCHANGE, ASS_GETLIST, pResultData->uHandleRusult, 0);
			}
			return true;
		}
	case DTK_GP_CHARMEXCHANGE:
		{
			if (pResultData->LineHead.uSize != sizeof(DL_GP_O_CharmExchangeList)) return false;
			//if (pResultData->uHandleRusult == 5)
			{
				DL_GP_O_CharmExchangeList* _p = (DL_GP_O_CharmExchangeList*)pResultData;
				TCharmExchange_Item _out = _p->date;
				m_TCPSocket.SendData(pResultData->uIndex, &_out, sizeof(TCharmExchange_Item), 
					MDM_GP_CHARMEXCHANGE, ASS_EXCHANGE, pResultData->uHandleRusult, 0);
			}
			return true;
		}
	case DTK_GP_MAILLIST_GET:
		{
			if (pResultData->uHandleRusult == 0)
			{
				DL_GP_O_MailGet* _p = (DL_GP_O_MailGet*)pResultData;
				if (_p != NULL)
				{
					map<int, TMailItem>::iterator _it = m_Mails.find(_p->date.iMsgID);
					if (_it == m_Mails.end())
					{
						m_Mails[_p->date.iMsgID] = _p->date;
					}
				}
			}
			else if  (pResultData->uHandleRusult == 1)
			{
				CTime _now = CTime::GetCurrentTime();
				for (map<int, TMailItem>::iterator _it = m_Mails.begin(); _it != m_Mails.end();)
				{
					if (_it->second.i64TimeSend < _now.GetTime() - 1)
					{
						if (_it->second.iTargetUserID == 0)
							m_TCPSocket.SendDataBatch(&_it->second, sizeof(TMailItem), MDM_GP_MAIL, ASS_GP_MAILINFO, 0);
						else
						{
							LPUSER pUser = NULL;
							for(int i = 0; i<m_UserList.GetCount(); i++)
							{
								pUser = (LPUSER)m_UserList.GetAt(i);
								if (pUser != NULL && pUser->UserID == _it->second.iTargetUserID)
								{
									m_TCPSocket.SendData(pUser->iSocketIndex, &_it->second, sizeof(TMailItem), MDM_GP_MAIL, ASS_GP_MAILINFO, 0, 0);
								}
							}
						}
						DL_GP_I_MailUpdate _p;
						_p.iMsgID = _it->first;
						m_SQLDataManage.PushLine(&_p.DataBaseHead, sizeof(DL_GP_I_MailUpdate), DTK_GP_MAIL_UPDATE, 0, 0);
						_it = m_Mails.erase(_it);
					}
					else _it++;
				}
				SetTimer(IDT_GET_MAIL_LIST, TIMEOUT_GET_MAIL_LIST * 100 * 30);
			}
			break;
		}
	case DTK_GP_LOGONPEOPLE_GET:
		{
			if (pResultData->uHandleRusult == 1)
			{
				DL_GP_O_GetLogonPeopleCount* _p = (DL_GP_O_GetLogonPeopleCount*)pResultData;
				if (_p != NULL)
				{
					m_TCPSocket.SendDataBatch(&_p->logonUsers, sizeof(ONLINEUSERSCOUNT), MDM_GP_LOGONUSERS, ASS_GP_LOGONUSERS_COUNT, 0);
				}
			}
			SetTimer(IDT_GET_LOGON_PEOPLE_COUNT,TIMEOUT_GET_LOGON_PEOPLE * 1000);
		}
		
		break;
	//���д���ģ�� add by JianGuankun 2011.12.21
	case DTK_GP_BANK_OPEN:
	case DTK_GP_BANK_GETGM_WALLET:
	case DTK_GP_BANK_CHECK:
	case DTK_GP_BANK_TRANSFER:
	case DTK_GP_BANK_TRANS_RECORD:
	case DTK_GP_BANK_DEPOSIT:
	case DTK_GP_BANK_QUERY_DEPOSIT:
	case DTK_GP_BANK_CHPWD:
	case DTK_GP_BANK_FINANCE_INFO:
		{
			m_pBankService->OnDataBaseResult(pResultData);
		}
		break;


	case DTK_GP_LASTLOGINGAME:
		{
			if (pResultData->uHandleRusult == 0)
			{
				DL_GP_O_LastLoginGame* _p = (DL_GP_O_LastLoginGame*)pResultData;
				if (_p != NULL)
				{
					CAFCKindItem* _pKind = m_GameList.FindKindItem(_p->_data.iKindID);
					if (_pKind != NULL)
					{
						CAFCNameItem* _pName = m_GameList.FindNameItem(_pKind, _p->_data.iGameID);

						if (_pName != NULL)
						{
							m_TCPSocket.SendData(pResultData->uIndex, &_pName->m_NameInfo, sizeof(ComNameInfo), MDM_GP_LASTLOINGGAME, ASS_GP_LASTLOINGGAME, 0, 0);
						}
						
					}
				} 
			}
			else
			{
				m_TCPSocket.SendData(pResultData->uIndex, NULL, 0, MDM_GP_LASTLOINGGAME, ASS_GP_LASTLOINGGAME, 1, 0);
			}
		}
	// end add

	}
	return true;
}
//���¹����б�
UINT CServiceDataBaseHandle::OnUpdateGameList(DataBaseLineHead * pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize!=sizeof(DL_GP_I_UpdateDataListStruct)) return 0;
	CServerGameListManage * pGameList=(CServerGameListManage *)(((DL_GP_I_UpdateDataListStruct *)pSourceData)->pGameList);
	//������ݶ���
	DataBaseResultLine DT_UpdateResult;
	::memset(&DT_UpdateResult,0,sizeof(DT_UpdateResult));
	//��������
	int iHandleResult=DTR_GP_LIST_ERROR;
	//�ȼ�����ݿ������Ƿ��ڣ����ھ�����zht 2012-2-22
	if (!sqlExec(m_pDataBaseManage->hDBLogon,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_pDataBaseManage->SQLConnectReset();

	}

	if (pGameList->UpdateGameListBuffer(m_pDataBaseManage->hDBLogon)==true)
		iHandleResult=DTR_GP_LIST_SCUESS;
	m_pRusultService->OnDataBaseResultEvent(&DT_UpdateResult,DTK_GP_UPDATE_LIST,iHandleResult,
		sizeof(DT_UpdateResult),pSourceData->uIndex,pSourceData->dwHandleID);
	return 0;
}



//������½����
UINT CServiceDataBaseHandle::OnPlaceLogon(DataBaseLineHead * pSourceData, UINT uLogonType)
{
	//������ݶ���
	DL_GP_O_LogonResultStruct DT_LogonResult;
	memset(&DT_LogonResult,0,sizeof(DT_LogonResult));

	try
	{
		//�������
		int iHandleCode=DTR_GP_ERROR_UNKNOW;//ִ�н��

		//ֻ����ͨ�����ֵ�¼

		//Fred Huang ,20081118
		//IPЧ��Ҳ����SP��
		if (pSourceData->DataLineHead.uSize!=sizeof(DL_GP_I_LogonByNameStruct)) 
			throw DTR_GP_ERROR_UNKNOW;
		//�Ѵ��������ݸ�����½��ϢDL_GP_I_LogonByNameStruct
		DL_GP_I_LogonByNameStruct * pLogonInfo=(DL_GP_I_LogonByNameStruct *)pSourceData;

		CString sname=pLogonInfo->LogonInfo.szName;

		sname.TrimLeft();sname.TrimRight();
		//�ȴ�������Ч��
		if (CheckChar(sname.GetBuffer(),false)==false) 
			throw DTR_GP_USER_NO_FIND;

		//������
		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_UserLoginPlace",true);
		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserName",sname,sname.GetLength());
		b=sqlSPAddPara(hDatabase,hRecord,"@MD5Password",pLogonInfo->LogonInfo.szMD5Pass,sizeof(pLogonInfo->LogonInfo.szMD5Pass));
		b=sqlSPAddPara(hDatabase,hRecord,"@UserLoginIP",pLogonInfo->szAccessIP,sizeof(pLogonInfo->szAccessIP));
		b=sqlSPAddPara(hDatabase,hRecord,"@UserToken",pLogonInfo->LogonInfo.TML_SN,sizeof(pLogonInfo->LogonInfo.TML_SN));

		///////////////////////////
		////Kylin 20090316 ��ӷ�ֹ�˻�ͬʱ��½

		CString zid;
	
		zid.Format("%d",pLogonInfo->ID);
		
		b=sqlSPAddPara(hDatabase,hRecord,"@ZID",zid,zid.GetLength());

		b=sqlSPAddPara(hDatabase, hRecord,"@MachineCode", pLogonInfo->LogonInfo.szMathineCode, sizeof(pLogonInfo->LogonInfo.szMathineCode));	//�����������ѻ����봫��洢���� zxj 2009-11-12
		        
        // PengJiLin, 2011-7-21, CPU��Ӳ�� ID
        sqlSPAddPara(hDatabase, hRecord,"@CPUID", pLogonInfo->LogonInfo.szCPUID, sizeof(pLogonInfo->LogonInfo.szCPUID));
        sqlSPAddPara(hDatabase, hRecord,"@HardID", pLogonInfo->LogonInfo.szHardID, sizeof(pLogonInfo->LogonInfo.szHardID));
		sqlSPAddPara(hDatabase,hRecord,"@ZJ_NumVerify",pLogonInfo->LogonInfo.szIDcardNo,sizeof(pLogonInfo->LogonInfo.szIDcardNo));
		sqlSPAddPara(hDatabase,hRecord,"@MobileVerify",pLogonInfo->LogonInfo.szMobileVCode,sizeof(pLogonInfo->LogonInfo.szMobileVCode));
		sqlSPAddPara(hDatabase, hRecord, "@UserIDIN", pLogonInfo->LogonInfo.iUserID);
        
        // PengJiLin, 2010-6-3, ����ǵ���������������¼ʧ��ʱ����Ҫת��������֤
        // ���Դ˴��Ƚ���ص���Ϣ������������֤ʧ��ʱ�������ʹ�á�(Ÿ��)
        strcpy(DT_LogonResult.LogonResult.szName, pLogonInfo->LogonInfo.szName);
        strcpy(DT_LogonResult.LogonResult.szMD5Pass, pLogonInfo->LogonInfo.szMD5Pass);
            
		///////////////////////////
		if(!sqlSPExec(hDatabase,hRecord,true))
			throw DTR_GP_ERROR_UNKNOW;

		int ret=sqlSPGetReturnValue(hDatabase,hRecord);
		
		if(ret==1) //IP����
			iHandleCode= DTR_GP_USER_IP_LIMITED;
		if(ret==2)//�û�������
			iHandleCode= DTR_GP_USER_NO_FIND;
		if(ret==3)//�ʺű�����
			iHandleCode= DTR_GP_USER_VALIDATA;
		if(ret==4) //�������
			iHandleCode= DTR_GP_USER_PASS_ERROR;
		/////////////////////////////
		///Kylin 20090211 �û��Ѿ���½
		if(ret==5) //�û��Ѿ���½
			iHandleCode= DTR_GP_USER_LOGONED;

		//{��������������ԭ���������Ӽ�������Ĺ��� zxj 2009-11-12
		if(ret==6)
		{
			iHandleCode = DTR_GP_ACCOUNT_LOCKED;
		}	
		//zxj ��������}
		if(ret==7)
			iHandleCode = DTK_GP_MATHINELOCK;

		//�����ص�½ JianGuankun 20111110
		if(ret==8)
		{
			iHandleCode = DTR_GP_ALLO_PARTY;
			sqlGetValue(hDatabase,hRecord,TEXT("UserID"),DT_LogonResult.LogonResult.dwUserID);
			sqlGetValue(hDatabase,hRecord,TEXT("ZJ_Number"),DT_LogonResult.LogonResult.szIDCardNo,sizeof(DT_LogonResult.LogonResult.szIDCardNo));
		}

		//{�ֻ���
		if(ret==9)
		{
			iHandleCode = DTR_GP_MOBILE_VALID;
			sqlGetValue(hDatabase,hRecord,TEXT("UserID"),DT_LogonResult.LogonResult.dwUserID);
			sqlGetValue(hDatabase,hRecord,TEXT("PhoneNum"),DT_LogonResult.LogonResult.szMobileNo,sizeof(DT_LogonResult.LogonResult.szMobileNo));
		}	

		/////////////////////////////
		if(ret==0)
		{
			iHandleCode=DTR_GP_LOGON_SUCCESS;
			
			sqlGetValue(hDatabase,hRecord,TEXT("UserID"),DT_LogonResult.LogonResult.dwUserID);
			sqlGetValue(hDatabase,hRecord,TEXT("LogoID"),DT_LogonResult.LogonResult.bLogoID);
			sqlGetValue(hDatabase,hRecord,TEXT("Sex"),DT_LogonResult.LogonResult.bBoy);
			sqlGetValue(hDatabase,hRecord,TEXT("wallet"),DT_LogonResult.LogonResult.i64Money);
			sqlGetValue(hDatabase,hRecord,TEXT("BankMoney"),DT_LogonResult.LogonResult.i64Bank);
			sqlGetValue(hDatabase,hRecord,TEXT("Fascination"),DT_LogonResult.LogonResult.dwFascination);
			sqlGetValue(hDatabase,hRecord,TEXT("timeismoney"),DT_LogonResult.LogonResult.dwTimeIsMoney);
			DT_LogonResult.LogonResult.dwTimeIsMoney = 0; ///< ȡ���õ������ͽ�ҹ���
			sqlGetValue(hDatabase,hRecord,TEXT("viptime"),DT_LogonResult.LogonResult.iVipTime);
			sqlGetValue(hDatabase,hRecord,TEXT("DoublePointTime"),DT_LogonResult.LogonResult.iDoublePointTime);
			sqlGetValue(hDatabase,hRecord,TEXT("ProtectTime"),DT_LogonResult.LogonResult.iProtectTime);
			TCHAR szLastLogonIP[16]={0};
			sqlGetValue(hDatabase,hRecord,TEXT("LastLoginIP"),szLastLogonIP,sizeof(szLastLogonIP));
			DT_LogonResult.LogonResult.dwLastLogonIP=inet_addr(szLastLogonIP);
			sqlGetValue(hDatabase,hRecord,TEXT("nickName"),DT_LogonResult.LogonResult.nickName,sizeof(DT_LogonResult.LogonResult.nickName));
			sqlGetValue(hDatabase,hRecord,TEXT("UserName"),DT_LogonResult.LogonResult.szName,sizeof(DT_LogonResult.LogonResult.szName));

			//JianGK 20111107���û�����
			sqlGetValue(hDatabase,hRecord,TEXT("SignDescr"),DT_LogonResult.LogonResult.szSignDescr,sizeof(DT_LogonResult.LogonResult.szSignDescr));
			sqlGetValue(hDatabase,hRecord,TEXT("RealName"),DT_LogonResult.LogonResult.szRealName,sizeof(DT_LogonResult.LogonResult.szRealName));
			sqlGetValue(hDatabase,hRecord,TEXT("ZJ_Number"),DT_LogonResult.LogonResult.szIDCardNo,sizeof(DT_LogonResult.LogonResult.szIDCardNo));
			sqlGetValue(hDatabase,hRecord,TEXT("PhoneNum"),DT_LogonResult.LogonResult.szMobileNo,sizeof(DT_LogonResult.LogonResult.szMobileNo));
			sqlGetValue(hDatabase,hRecord,TEXT("QQNum"),DT_LogonResult.LogonResult.szQQNum,sizeof(DT_LogonResult.LogonResult.szQQNum));
			sqlGetValue(hDatabase,hRecord,TEXT("AdrNation"),DT_LogonResult.LogonResult.szAdrNation,sizeof(DT_LogonResult.LogonResult.szAdrNation));
			sqlGetValue(hDatabase,hRecord,TEXT("AdrProvince"),DT_LogonResult.LogonResult.szAdrProvince,sizeof(DT_LogonResult.LogonResult.szAdrProvince));
			sqlGetValue(hDatabase,hRecord,TEXT("AdrCity"),DT_LogonResult.LogonResult.szAdrCity,sizeof(DT_LogonResult.LogonResult.szAdrCity));
			sqlGetValue(hDatabase,hRecord,TEXT("ZipCode"),DT_LogonResult.LogonResult.szZipCode,sizeof(DT_LogonResult.LogonResult.szZipCode));
			sqlGetValue(hDatabase,hRecord,TEXT("Email"),DT_LogonResult.LogonResult.szEmail,sizeof(DT_LogonResult.LogonResult.szEmail));
			sqlGetValue(hDatabase,hRecord,TEXT("Address"),DT_LogonResult.LogonResult.szAddress,sizeof(DT_LogonResult.LogonResult.szAddress));
			sqlGetValue(hDatabase,hRecord,TEXT("Schooling"),DT_LogonResult.LogonResult.szSchooling,sizeof(DT_LogonResult.LogonResult.szSchooling));
			sqlGetValue(hDatabase,hRecord,TEXT("Email"),DT_LogonResult.LogonResult.szEmail,sizeof(DT_LogonResult.LogonResult.szEmail));
			sqlGetValue(hDatabase,hRecord,TEXT("Address"),DT_LogonResult.LogonResult.szAddress,sizeof(DT_LogonResult.LogonResult.szAddress));
			sqlGetValue(hDatabase,hRecord,TEXT("Schooling"),DT_LogonResult.LogonResult.szSchooling,sizeof(DT_LogonResult.LogonResult.szSchooling));
			sqlGetValue(hDatabase,hRecord,TEXT("HomePage"),DT_LogonResult.LogonResult.szHomePage,sizeof(DT_LogonResult.LogonResult.szHomePage));
			sqlGetValue(hDatabase,hRecord,TEXT("TelNo"),DT_LogonResult.LogonResult.szTelNo,sizeof(DT_LogonResult.LogonResult.szTelNo));
			sqlGetValue(hDatabase,hRecord,TEXT("MSNID"),DT_LogonResult.LogonResult.szMSNID,sizeof(DT_LogonResult.LogonResult.szMSNID));
			//end JianGK 20111107

			sqlGetValue(hDatabase,hRecord,TEXT("HAVENEWS"),DT_LogonResult.LogonResult.bLoginBulletin);
			sqlGetValue(hDatabase,hRecord,TEXT("LockMathine"), DT_LogonResult.LogonResult.iLockMathine);	//��������״̬ zxj 2009-11-13
			sqlGetValue(hDatabase,hRecord,TEXT("BindMobile"), DT_LogonResult.LogonResult.iBindMobile);	//�����ֻ��İ�״̬ jianguankun 2012-10-10

			sqlGetValue(hDatabase,hRecord,TEXT("AddFriendType"),DT_LogonResult.LogonResult.iAddFriendType);

			DT_LogonResult.LogonResult.dwAccID=pLogonInfo->LogonInfo.gsqPs;
		}
		
		sqlSPClose(hDatabase,hRecord);
		//������
		m_pRusultService->OnDataBaseResultEvent(&DT_LogonResult.ResultHead,uLogonType,iHandleCode,
			sizeof(DT_LogonResult),pSourceData->uIndex,pSourceData->dwHandleID);
	
	}
	catch (int &iHandleResult)	//����������
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		m_pRusultService->OnDataBaseResultEvent(&DT_LogonResult.ResultHead,uLogonType,iHandleResult,
			sizeof(DT_LogonResult),pSourceData->uIndex,pSourceData->dwHandleID);
	}

	return 0;
}



//Ч���ַ�
bool CServiceDataBaseHandle::CheckChar(TCHAR * szChar, bool bStrCheck)
{
	int iLength=lstrlen(szChar);
	for (int i=0;i<iLength;)
	{
		if (((szChar[i]>=-1)&&(szChar[i]<47))||(szChar[i]==127)||(szChar[i]=='\\')||(szChar[i]=='/')) 
			return false;
		byte a=szChar[i];
		byte b=szChar[i+1];
		//�ж��Ƿ���
		if((a>=176&&a<=247&&b>=160&&b<=254)||//GB2312
			(((a>=129&&a<=255)||(a>=64&&a<=126))&&b>=161&&b<=254)||///GB5
			(a>=129&&a<=254&&b>=64&&b<=254))
		{
			i += 2;
			continue;
		}else if(szChar[i] >='a'&&szChar[i]<='z'||szChar[i] >='A'&&szChar[i]<='Z' || szChar[i] >= '0'&&szChar[i] <= '9')
		{
			i += 1;
			continue;
		}
		return false;
	}

	if (bStrCheck)
	{
		const TCHAR * szLimitName[]={TEXT("����"),TEXT("������"),TEXT("����ˮ"),TEXT("ë��"),TEXT("ϵͳ"),TEXT("����"),
			TEXT("����"),TEXT("��"),TEXT("����"),TEXT("��"),TEXT("����"),TEXT("FUCK"),TEXT("��B"),TEXT("��"),TEXT("��"),
			TEXT("���"),TEXT("�ڹ�"),TEXT("ϵͳ"),TEXT("���"),TEXT("��Ϸ����"),TEXT("��Ϣ"),
			TEXT("�н�"),TEXT("��Ϣ"),TEXT("�ͷ�"),TEXT("�ڰ�"),TEXT("�ڰ�"),TEXT("�ڰ�"),TEXT("����Ա"),TEXT("�ٷ�"),TEXT("game0437"),TEXT("�ͷ�����"),TEXT("�ͷ�����")
			TEXT("GM"),TEXT(" "),TEXT("����"),TEXT("�ǳ�"),TEXT("����"),TEXT("����"),TEXT("ȥ��")
		};
		CString strChar=szChar;
		strChar.MakeUpper();
		for (int i=0;i<sizeof(szLimitName)/sizeof(szLimitName[0]);i++)
		{
			if (strChar.Find(szLimitName[i])!=-1) return false;
		}
	}
	return true;
}


//������ݿ�����
UINT CServiceDataBaseHandle::OnCheckDataConnect(DataBaseLineHead * pSourceData)
{
	try 
	{ 
		//�������
		m_pDataBaseManage->CheckSQLConnect();

		//������
		DataBaseResultLine ResultHead;
		m_pRusultService->OnDataBaseResultEvent(&ResultHead,DTK_GP_CHECK_CONNECT,0,sizeof(ResultHead),0,0);
	} 
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}

	return 0;
}

// PengJiLin, 2011-5-16, ����Ƿ���Ҫ����������Ϸ�б�
UINT CServiceDataBaseHandle::OnCheckSellGameList(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode=DTR_GP_ERROR_UNKNOW;

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_CheckSellGameList", true);
		sqlSPExec(hDatabase,hRecord);

		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);

		DataBaseResultLine dbrLine;
		memset(&dbrLine, 0, sizeof(dbrLine));

		m_pRusultService->OnDataBaseResultEvent(&dbrLine,DTK_GP_CHECK_SELLGAMELIST,0,
			sizeof(dbrLine),pSourceData->uIndex,iRet);


		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//added by JianGK 20111108
//�޸��û�����
UINT CServiceDataBaseHandle::OnUpdateUserInfo(DataBaseLineHead* pSourceData,UINT nType)
{
	//������ݶ���
	DL_GR_O_UserInfoStruct DT_UpdateResult;
	memset(&DT_UpdateResult,0,sizeof(DL_GR_O_UserInfoStruct));

	int iHandleCode = nType;

	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_UserInfoStruct))
	{
		throw DTR_GP_ERROR_UNKNOW;
	}

	DL_GP_I_UserInfoStruct* pUpdateStruct = (DL_GP_I_UserInfoStruct*)pSourceData;
	MSG_GP_S_UserInfo* pUserNewInfo = &pUpdateStruct->UserInfo;

	try
	{
		//������¼
		HANDLE hDatabase = m_pDataBaseManage->hDBLogon;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_UpdateUserDetail", true);

		sqlSPAddPara(hDatabase,hRecord,"@UserID",pUserNewInfo->dwUserID);

		if (nType == DTK_GP_UPDATE_USERINFO_BASE)
		{
			sqlSPAddPara(hDatabase,hRecord,"@UpdateType",1);
		}
		else
		{
			sqlSPAddPara(hDatabase,hRecord,"@UpdateType",2);
		}

		sqlSPAddPara(hDatabase,hRecord,"@UserPassword",pUserNewInfo->szMD5Pass,sizeof(pUserNewInfo->szMD5Pass));
		sqlSPAddPara(hDatabase,hRecord,"@nickName",pUserNewInfo->nickName,sizeof(pUserNewInfo->nickName));
		sqlSPAddPara(hDatabase,hRecord,"@Sex",pUserNewInfo->bBoy);
		sqlSPAddPara(hDatabase,hRecord,"@SignDescr",pUserNewInfo->szSignDescr,sizeof(pUserNewInfo->szSignDescr));
		sqlSPAddPara(hDatabase,hRecord,"@RealName",pUserNewInfo->szRealName,sizeof(pUserNewInfo->szRealName));
		sqlSPAddPara(hDatabase,hRecord,"@ZJ_Number",pUserNewInfo->szIDCardNo,sizeof(pUserNewInfo->szIDCardNo));
		sqlSPAddPara(hDatabase,hRecord,"@PhoneNum",pUserNewInfo->szMobileNo,sizeof(pUserNewInfo->szMobileNo));
		sqlSPAddPara(hDatabase,hRecord,"@QQNum",pUserNewInfo->szQQNum,sizeof(pUserNewInfo->szQQNum));
		sqlSPAddPara(hDatabase,hRecord,"@AdrNation",pUserNewInfo->szAdrNation,sizeof(pUserNewInfo->szAdrNation));
		sqlSPAddPara(hDatabase,hRecord,"@AdrProvince",pUserNewInfo->szAdrProvince,sizeof(pUserNewInfo->szAdrProvince));
		sqlSPAddPara(hDatabase,hRecord,"@AdrCity",pUserNewInfo->szAdrCity,sizeof(pUserNewInfo->szAdrCity));
		sqlSPAddPara(hDatabase,hRecord,"@ZipCode",pUserNewInfo->szZipCode,sizeof(pUserNewInfo->szZipCode));
		sqlSPAddPara(hDatabase,hRecord,"@Email",pUserNewInfo->szEmail,sizeof(pUserNewInfo->szEmail));
		sqlSPAddPara(hDatabase,hRecord,"@Address",pUserNewInfo->szAddress,sizeof(pUserNewInfo->szAddress));
		sqlSPAddPara(hDatabase,hRecord,"@Schooling",pUserNewInfo->szSchooling,sizeof(pUserNewInfo->szSchooling));
		sqlSPAddPara(hDatabase,hRecord,"@HomePage",pUserNewInfo->szHomePage,sizeof(pUserNewInfo->szHomePage));
		sqlSPAddPara(hDatabase,hRecord,"@TelNo",pUserNewInfo->szTelNo,sizeof(pUserNewInfo->szTelNo));
		sqlSPAddPara(hDatabase,hRecord,"@MSNID",pUserNewInfo->szMSNID,sizeof(pUserNewInfo->szMSNID));

		if(!sqlSPExec(hDatabase,hRecord,true))
		{
			throw DTR_GP_ERROR_UNKNOW;
		}

		int ret = sqlSPGetReturnValue(hDatabase,hRecord);

		if(ret == 1)
		{
			iHandleCode = DTR_GP_UPDATE_USERINFO_ACCEPT;
		}
		else
		{
			iHandleCode = DTR_GP_UPDATE_USERINFO_NOTACCEPT;
		}

		//������
		m_pRusultService->OnDataBaseResultEvent(&DT_UpdateResult.ResultHead,nType,iHandleCode,
			sizeof(DL_GR_O_UserInfoStruct),pSourceData->uIndex,pSourceData->dwHandleID);

		sqlSPClose(hDatabase, hRecord);
		
	}
	catch(int &iHandleResult)
	{

		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		//������
		m_pRusultService->OnDataBaseResultEvent(&DT_UpdateResult.ResultHead,nType,iHandleResult,
			sizeof(DL_GR_O_UserInfoStruct),pSourceData->uIndex,pSourceData->dwHandleID);
	}


	return 0;
}
//end add

//added by JianGK 20111113
//�޸�����
UINT CServiceDataBaseHandle::OnUpdateUserPwd(DataBaseLineHead* pSourceData)
{
	//������ݶ���
	DL_GP_O_ChPasswordStruct DT_UpdateResult;
	memset(&DT_UpdateResult,0,sizeof(DL_GP_O_ChPasswordStruct));

	int iHandleCode = DTR_GP_UPDATE_USERINFO_PWD;

	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_ChPasswordStruct))
	{
		throw DTR_GP_ERROR_UNKNOW;
	}

	DL_GP_I_ChPasswordStruct* pUpdateStruct = (DL_GP_I_ChPasswordStruct*)pSourceData;
	MSG_GP_S_ChPassword* pUserChPwd = &pUpdateStruct->ChPwd;

	try
	{
		//������¼
		HANDLE hDatabase = m_pDataBaseManage->hDBLogon;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_UpdateUserPassword", true);

		sqlSPAddPara(hDatabase,hRecord,"@UserID",pUserChPwd->dwUserID);
		sqlSPAddPara(hDatabase,hRecord,"@HardID",pUserChPwd->szHardID,sizeof(pUserChPwd->szHardID));
		sqlSPAddPara(hDatabase,hRecord,"@UserOldPassword",pUserChPwd->szMD5OldPass,sizeof(pUserChPwd->szMD5OldPass));
		sqlSPAddPara(hDatabase,hRecord,"@UserNewPassword",pUserChPwd->szMD5NewPass,sizeof(pUserChPwd->szMD5NewPass));

		if(!sqlSPExec(hDatabase,hRecord,true))
		{
			sqlSPClose(hDatabase, hRecord);
			throw DTR_GP_ERROR_UNKNOW;
		}

		int ret = sqlSPGetReturnValue(hDatabase,hRecord);

		if(ret == 1)
		{
			iHandleCode = DTR_GP_UPDATE_USERINFO_ACCEPT;
		}
		else
		{
			iHandleCode = DTR_GP_UPDATE_USERINFO_NOTACCEPT;
		}

		//������
		m_pRusultService->OnDataBaseResultEvent(&DT_UpdateResult.ResultHead,DTR_GP_UPDATE_USERINFO_PWD,iHandleCode,
			sizeof(DL_GP_O_ChPasswordStruct),pSourceData->uIndex,pSourceData->dwHandleID);

		sqlSPClose(hDatabase, hRecord);
	}
	catch(int &iHandleResult)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		//������
		m_pRusultService->OnDataBaseResultEvent(&DT_UpdateResult.ResultHead,DTR_GP_UPDATE_USERINFO_PWD,DTR_GP_UPDATE_USERINFO_NOTACCEPT,
			sizeof(DL_GP_O_ChPasswordStruct),pSourceData->uIndex,pSourceData->dwHandleID);
	}

	return 0;
}

UINT CServiceDataBaseHandle::OnReceiveMoney(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode=DTR_GP_ERROR_UNKNOW;
		DL_GP_I_ReceiveMoney* _p = (DL_GP_I_ReceiveMoney*)pSourceData;
		DL_GP_O_ReceiveMoney _out;
		ZeroMemory(&_out, sizeof(_out));
		_out.date.iUserID = _p->iUserID;

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_ReceiveMoney", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->iUserID);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		sqlGetValue(hDatabase,hRecord,TEXT("ReceiveCount"),_out.date.iCount);
		sqlGetValue(hDatabase,hRecord,TEXT("ReceiveMoney"),_out.date.i64Money);
		sqlGetValue(hDatabase,hRecord,TEXT("MinMoney"),_out.date.i64MinMoney);
		sqlGetValue(hDatabase,hRecord,TEXT("ReceiveTotal"),_out.date.iTotal);
		sqlGetValue(hDatabase,hRecord,TEXT("SpaceTime"),_out.date.iTime);
		

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GP_RECEIVEMONEY, iHandleCode,
			sizeof(DL_GP_O_ReceiveMoney), pSourceData->uIndex, iRet);



		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

UINT CServiceDataBaseHandle::OnCharmExchangeList(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode=DTR_GP_ERROR_UNKNOW;
		DataBaseLineHead* _p = (DataBaseLineHead*)pSourceData;
		DL_GP_O_CharmExchangeList _out;
		ZeroMemory(&_out, sizeof(_out));

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_CharmExchange_Get", true);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(DL_GP_O_CharmExchangeList));

			sqlGetValue(hDatabase,hRecord,"id",_out.date.iID);
			sqlGetValue(hDatabase,hRecord,"CharmValue",_out.date.iPoint);
			sqlGetValue(hDatabase,hRecord,"PropID",_out.date.iPropID);
			sqlGetValue(hDatabase,hRecord,"PropCount",_out.date.iPropCount);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_CHARMEXCHANGE_LIST, 0, sizeof(DL_GP_O_CharmExchangeList),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}

		ZeroMemory(&_out,sizeof(DL_GP_O_CharmExchangeList));
		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GP_CHARMEXCHANGE_LIST, 1, sizeof(DL_GP_O_CharmExchangeList), pSourceData->uIndex, pSourceData->dwHandleID);


		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

UINT CServiceDataBaseHandle::OnCharmExchange(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode=DTR_GP_ERROR_UNKNOW;
		DL_GP_I_CharmExchange* _p = (DL_GP_I_CharmExchange*)pSourceData;
		DL_GP_O_CharmExchangeList _out;
		ZeroMemory(&_out, sizeof(DL_GP_O_CharmExchangeList));

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_CharmExchange_Do", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->date.iUserID);
		b=sqlSPAddPara(hDatabase,hRecord,"@id", _p->date.iID);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		ZeroMemory(&_out,sizeof(DL_GP_O_CharmExchangeList));
		sqlGetValue(hDatabase,hRecord,"id",_out.date.iID);
		sqlGetValue(hDatabase,hRecord,"CharmValue",_out.date.iPoint);
		sqlGetValue(hDatabase,hRecord,"PropID",_out.date.iPropID);
		sqlGetValue(hDatabase,hRecord,"PropCount",_out.date.iPropCount);

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_CHARMEXCHANGE, iHandleCode, sizeof(DL_GP_O_CharmExchangeList),pSourceData->uIndex,pSourceData->dwHandleID);


		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}


UINT CServiceDataBaseHandle::OnSetFriendType(DataBaseLineHead * pSourceData)
{
	try
	{
		int iHandleCode=DTR_GP_ERROR_UNKNOW;
		DL_GP_I_SetFriendType* _p = (DL_GP_I_SetFriendType*)pSourceData;

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_FriendType_Set", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->iUserID);
		b=sqlSPAddPara(hDatabase,hRecord,"@Type", _p->iType);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);
		iHandleCode = iRet;

		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//��ѯ��ҵ�ID���ǳ�,Add by JianGuankun 2012.1.4
UINT CServiceDataBaseHandle::OnQueryUserNickNameID(DataBaseLineHead* pSourceData)
{
	// Ч������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_GetNickNameID))
	{
		throw DTR_GP_ERROR_UNKNOW;
	}

	DL_GP_I_GetNickNameID* pInData =(DL_GP_I_GetNickNameID*)pSourceData;

	// ��ʼ�����
	DL_GP_O_GetNickNameID _out;
	memset(&_out,0,sizeof(DL_GP_O_GetNickNameID));
	_out._data._nType = pInData->_data._nType;
	_out._data._nUseful = pInData->_data._nUseful;

	HANDLE hDatabase = m_pDataBaseManage->hDBNative;
	HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_GetNickNameID",true);
	if(hRecord < (HANDLE)0x1000)
	{
		sqlCloseRecord(hDatabase,hRecord);
		return 0;
	}

	sqlSPAddPara(hDatabase,hRecord,TEXT("@UserID"),pInData->_data._user_id);
	sqlSPAddPara(hDatabase,hRecord,TEXT("@NickName"),pInData->_data._nickname,sizeof(pInData->_data._nickname));
	sqlSPExec(hDatabase,hRecord,true);
	int ret = sqlSPGetReturnValue(hDatabase,hRecord);
	
	if(ret == 0) //�����ɹ�
	{
		sqlGetValue(hDatabase,hRecord,"UserID",_out._data._user_id);
		sqlGetValue(hDatabase,hRecord,"NickName",_out._data._nickname,sizeof(_out._data._nickname));
	}

	sqlCloseRecord(hDatabase,hRecord);

	m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_UPDATE_NICKNAMEID,
		0,sizeof(DL_GP_O_GetNickNameID),pSourceData->uIndex,pSourceData->dwHandleID);

	return 0;
}
//End Add

//�������д���
UINT CServiceDataBaseHandle::OnBankOpen(DataBaseLineHead* pSourceData)
{
	//Ч������

	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_BankOpen))
	{
		throw DTR_GP_BANK_OPEN_ERROR; //���ܵĽṹ�Ͷ�����Ƿ�һ��
	}

	DL_GP_I_BankOpen* pOpenWallet = (DL_GP_I_BankOpen*)pSourceData;
	DL_GP_O_BankOpen OpenWallet;///�򿪽��
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
			throw DTR_GP_BANK_OPEN_ERROR;
		}

		//CHAR	szMD5Pass[50];						//��������
		//ZeroMemory(szMD5Pass,sizeof(szMD5Pass));

		//sqlGetValue(hDatabase,hRecord,"TwoPassword",szMD5Pass,sizeof(szMD5Pass));
		sqlGetValue(hDatabase,hRecord,"MoneyInBank",OpenWallet._data.i64Bank);
		sqlGetValue(hDatabase,hRecord,"TwoPassword",OpenWallet._data.szTwoPassword,sizeof(OpenWallet._data.szTwoPassword));
		sqlGetValue(hDatabase,hRecord,"BankVer",OpenWallet._data.nVer);


		sqlCloseRecord(hDatabase,hRecord);

		//if (lstrcmp(szMD5Pass,pOpenWallet->szMD5Pass)!=0) 
		//	throw DTR_GR_OPEN_WALLET_ERROR_PSW;
		OpenWallet._data.dwUserID = pOpenWallet->dwUserID;
		throw DTR_GP_BANK_OPEN_SUC;

	}
	catch (int iHandleResult)
	{
		TRACE("EXCEPT CATCH:%s with %s\n",__FILE__,__FUNCTION__);
		m_pRusultService->OnDataBaseResultEvent(&OpenWallet.ResultHead, DTK_GP_BANK_OPEN, iHandleResult,
			sizeof(DL_GP_O_BankOpen),pSourceData->uIndex,pSourceData->dwHandleID);
	}

	return 0;
}

//��ѯǮ��
UINT CServiceDataBaseHandle::OnBankGetGameWallet(DataBaseLineHead* pSourceData)
{
	try
	{
		DL_GP_I_BankGetWallet* _p = (DL_GP_I_BankGetWallet*)pSourceData;
		DL_GP_O_BankGetWallet _out;
		ZeroMemory(&_out, sizeof(_out));

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankOpWallet", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_UserID);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);

		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(DL_GP_O_BankGetWallet));

			sqlGetValue(hDatabase,hRecord,"GameID",_out._data._game_id);
			sqlGetValue(hDatabase,hRecord,"ComName",_out._data._game_name,sizeof(_out._data._game_name));
			sqlGetValue(hDatabase,hRecord,"WalletMoney",_out._data._money);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_GETGM_WALLET,0,sizeof(DL_GP_O_BankGetWallet),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}

		ZeroMemory(&_out,sizeof(DL_GP_O_BankGetWallet));
		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_GETGM_WALLET,1,sizeof(DL_GP_O_BankGetWallet),pSourceData->uIndex,pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//���д�ȡ
UINT CServiceDataBaseHandle::OnBankCheck(DataBaseLineHead* pSourceData)
{
	try
	{
		DL_GP_I_BankCheck* _p = (DL_GP_I_BankCheck*)pSourceData;
		DL_GP_O_BankCheck _out;
		ZeroMemory(&_out, sizeof(_out));
		_out._data = _p->_data;

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_BankOpNormal", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_data._user_id);
		b = sqlSPAddPara(hDatabase,hRecord,"@GameID", _p->_data._game_id);
		b = sqlSPAddPara(hDatabase,hRecord,"@OperateType", _p->_data._operate_type);
		b = sqlSPAddPara_i64(hDatabase,hRecord,"@Money", _p->_data._money);
		b = sqlSPAddPara(hDatabase,hRecord,"@MD5Pass", _p->_data._szMD5Pass,sizeof(_p->_data._szMD5Pass));

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);

		if (iRet == HC_GP_BANK_CHECK_SUC)
		{
			sqlGetValue(hDatabase,hRecord, "MoneyOp", _out._data._money);
		}

		_out.ResultHead.uHandleRusult = iRet;

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_CHECK,iRet,
			sizeof(DL_GP_O_BankCheck),pSourceData->uIndex,pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//���д�ȡ
UINT CServiceDataBaseHandle::OnBankTransfer(DataBaseLineHead* pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_BankTransfer))
	{
		return 0;
	}

	DL_GP_I_BankTransfer* pTransfer = (DL_GP_I_BankTransfer*)pSourceData;
	DL_GP_O_BankTransfer _out;
	::memset(&_out,0,sizeof(DL_GP_O_BankTransfer));
	_out._data = pTransfer->_data;

	if(_out._data.i64Money <= 0)
	{
		return 0;
	}

	// PengJiLin, 2010-8-23, �������С�ȫת������

	// PengJiLin, 2011-7-13, 32λ����ת��64λ����
	UINT u32Data = 0;

	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord;
		if(1 == _out._data.uHandleCode)
		{
			hRecord = sqlSPSetNameEx(hDatabase,"SP_TransferAllMoney",true);
		}
		else
		{
			hRecord = sqlSPSetNameEx(hDatabase,"SP_TransferMoney",true);
		}

		if(hRecord < (HANDLE)0x1000)
		{
			return 0;
		}
			
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pTransfer->_data.UserID);
		sqlSPAddPara(hDatabase,hRecord,"@UseDestID",pTransfer->_data.bUseDestID?1:0);
		sqlSPAddPara(hDatabase,hRecord,"@DestUserID",pTransfer->_data.destUserID);
		sqlSPAddPara(hDatabase,hRecord,"@DestNickName",pTransfer->_data.szDestNickName,sizeof(pTransfer->_data.szDestNickName));//����
		//sqlSPAddPara(hDatabase,hRecord,"@DestUser",pTransferMoney->stTransfer.szDestName,sizeof(pTransferMoney->stTransfer.szDestName));
		if(0 == pTransfer->_data.uHandleCode)     // PengJiLin, 2010-8-23, ȫת����
			sqlSPAddPara_i64(hDatabase,hRecord,"@TranMoney",pTransfer->_data.i64Money);
		sqlSPAddPara(hDatabase,hRecord,"@TranTax",pTransfer->_data.bTranTax);
		sqlSPAddPara(hDatabase,hRecord,"@MD5Pass",pTransfer->_data.szMD5Pass, sizeof(pTransfer->_data.szMD5Pass));//����

		sqlSPExec(hDatabase,hRecord,true);
		int ret = sqlSPGetReturnValue(hDatabase,hRecord);
		UINT uResult = HC_GP_BANK_TRANSFER_SUC;

		switch(ret)
		{
		case 0: //ת�ʳɹ�
			{
				sqlGetValue(hDatabase,hRecord,"DestUserID",_out._data.destUserID);
				sqlGetValue(hDatabase,hRecord,"TranMoney",_out._data.i64Money);
				sqlGetValue(hDatabase,hRecord,"ActualTransfer",_out._data.i64ActualTransfer);
				break;
			}
		case 1:	//�������
			{
				uResult = HC_GP_BANK_TRANSFER_PASSWORD_ERROR;
				break;
			}
		case 2: //������Ǯ���ڶ��ٲ���ת��
			{
				uResult = HC_GP_BANK_TRANSFER_TOTAL_LESS;
				sqlGetValue(hDatabase,hRecord,"LessAllMoney",_out._data.i64ActualTransfer);
				break;
			}
		case 3: //����ת�ʱ�����ڶ���
			{
				uResult = HC_GP_BANK_TRANSFER_TOO_LESS;
				sqlGetValue(hDatabase,hRecord,"LessMoney",_out._data.i64ActualTransfer);
			}
			break;
		case 4: //����ת����ĳ����������
			{
				uResult = HC_GP_BANK_TRANSFER_MULTIPLE;
				sqlGetValue(hDatabase,hRecord,"Multiple",u32Data);
				_out._data.i64ActualTransfer = u32Data;
			}
			
			break;
		case 5: //������ֻ�ж���Ǯ��������Ҫת����Ǯ������ת
			{
				uResult = HC_GP_BANK_TRANSFER_NOT_ENOUGH;
				sqlGetValue(hDatabase,hRecord,"TransMoney", _out._data.i64Money);
				sqlGetValue(hDatabase,hRecord,"BankMoney",_out._data.i64ActualTransfer);
			}
			break;
		case 6: ///<Ŀ���ʻ�������
			{
				uResult = HC_GP_BANK_TRANSFER_NO_DEST;
			}
			break;
		case 7: //����ת�˵Ĵ���̫����;
			{
				uResult = HC_GP_BANK_TRANSFER_TOO_MANY_TIME;
			}
			break;
		default:
			return 0;
		}
		sqlCloseRecord(hDatabase,hRecord);

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_TRANSFER,uResult,
			sizeof(DL_GP_O_BankTransfer),pSourceData->uIndex,pSourceData->dwHandleID);
	}
	catch (...)
	{
		return 0;
	}

	return 0;
}

//��ѯ����ת�˼�¼
UINT CServiceDataBaseHandle::OnBankTransRecord(DataBaseLineHead* pSourceData)
{
	DL_GP_I_BankTransRecord* pTransfer = (DL_GP_I_BankTransRecord*)pSourceData;
	DL_GP_O_BankTransRecord _out;

	if (pTransfer->_data.dwUserID == 0)
	{
		return 0;
	}

	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord  = sqlSPSetNameEx(hDatabase,"SP_GetTransferRecord",true);
		if(hRecord < (HANDLE)0x1000)
		{
			return 0;
		}
			
		sqlSPAddPara(hDatabase,hRecord,"@UserID",pTransfer->_data.dwUserID);
		sqlSPExec(hDatabase,hRecord,true);

		int ret = sqlSPGetReturnValue(hDatabase,hRecord);
		if (ret > 0)
		{
			sqlCloseRecord(hDatabase,hRecord);
			return 0;
		}

		/// ÿ��ȡһ����¼���ͷ���һ�Σ�ֱ����ȡ������smalldatetime���͵������ڿͻ��˽���
		int nSeriNo = 0;
		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			_out._data.nSeriNo = nSeriNo++;
			sqlGetValue(hDatabase,hRecord,"UserID", _out._data.dwUserID);
			sqlGetValue(hDatabase,hRecord,"DestUserID", _out._data.destUserID);
			sqlGetValue(hDatabase,hRecord,"TransferMoney", _out._data.i64Money);
			sqlGetValue(hDatabase,hRecord,"ActualTransfer",_out._data.i64ActualTransfer);
			sqlGetValue(hDatabase,hRecord,"TransTime",_out._data.oleDateTime);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_TRANS_RECORD,HC_GP_BANK_TRANS_RECORD_SUC,
				sizeof(DL_GP_O_BankTransRecord),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}
		sqlCloseRecord(hDatabase,hRecord);
	}
	catch (...)
	{
		return 0;
	}

	return 0;
}

//�浥ҵ��
UINT CServiceDataBaseHandle::OnBankDeposit(DataBaseLineHead* pSourceData)
{
	try
	{
		DL_GP_I_BankDeposit* _p = (DL_GP_I_BankDeposit*)pSourceData;
		DL_GP_O_BankDeposit _out;
		ZeroMemory(&_out, sizeof(_out));
		_out._data = _p->_data;

		HANDLE hDatabase;
		HANDLE hRecord;
		int iRet;

		if (_p->_data._operate_type == 1)
		{
			hDatabase = m_pDataBaseManage->hDBLogon;
			hRecord = sqlSPSetNameEx(hDatabase,"SP_Bank_NewDeposit",true);

			bool b = sqlSPAddPara(hDatabase,hRecord,"@CreaterUserID", _p->_data._user_id);
			b = sqlSPAddPara_i64(hDatabase,hRecord,"@Money", _p->_data._money);
			sqlSPAddPara(hDatabase,hRecord,"@MD5Pass",_p->_data._szMD5Pass,sizeof(_p->_data._szMD5Pass));

			sqlSPExec(hDatabase,hRecord, true);
			iRet = sqlSPGetReturnValue(hDatabase, hRecord);

			if (iRet == HC_GP_BANK_DEPOSIT_SUC)
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
			hDatabase=m_pDataBaseManage->hDBLogon;
			hRecord=sqlSPSetNameEx(hDatabase,"SP_Bank_UseDeposit", true);

			bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_data._user_id);
			CString s = _p->_data._deposit_id;
			sqlSPAddPara(hDatabase,hRecord,"@DepositID", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());
			s = _p->_data._deposit_password;
			sqlSPAddPara(hDatabase,hRecord,"@DepositPwd", s.GetBuffer(), sizeof(TCHAR)*s.GetLength());

			sqlSPExec(hDatabase,hRecord, true);
			iRet = sqlSPGetReturnValue(hDatabase, hRecord);

			if (iRet == HC_GP_BANK_DEPOSIT_SUC)
			{
				_out._data._state = 1;
				sqlGetValue(hDatabase, hRecord, "UserID", _out._data._user_id);
				sqlGetValue(hDatabase, hRecord, "NickName", _out._data._szNickName,sizeof(_out._data._szNickName));
				sqlGetValue(hDatabase, hRecord, "Money", _out._data._money);
			}
		}

		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_DEPOSIT,iRet,
			sizeof(DL_GP_O_BankDeposit),pSourceData->uIndex,iRet);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//��ѯ�浥
UINT CServiceDataBaseHandle::OnBankQueryDeposits(DataBaseLineHead* pSourceData)
{
	try
	{
		DL_GP_I_BankQueryDeposit* _p = (DL_GP_I_BankQueryDeposit*)pSourceData;
		DL_GP_O_BankQueryDeposit _out;
		ZeroMemory(&_out, sizeof(_out));

		HANDLE hDatabase=m_pDataBaseManage->hDBLogon;
		HANDLE hRecord=sqlSPSetNameEx(hDatabase,"SP_Bank_QueryDeposits", true);

		bool b=sqlSPAddPara(hDatabase,hRecord,"@UserID", _p->_UserID);

		sqlSPExec(hDatabase,hRecord, true);
		int iRet = sqlSPGetReturnValue(hDatabase, hRecord);

		int nSeriNo = 0;

		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(DL_GP_O_BankQueryDeposit));

			_out._data._serial_no = nSeriNo++;
			sqlGetValue(hDatabase,hRecord,"DepositID",_out._data._deposit_id,sizeof(_out._data._deposit_id));
			sqlGetValue(hDatabase,hRecord,"DepositPassWord",_out._data._deposit_password,sizeof(_out._data._deposit_password));
			sqlGetValue(hDatabase,hRecord,"Money",_out._data._money);
			sqlGetValue(hDatabase,hRecord,"UserID",_out._data._user_id);
			sqlGetValue(hDatabase,hRecord,"GreateTime",_out._data._create_time);
			sqlGetValue(hDatabase,hRecord,"CDSC",_out._data._csds);
			sqlGetValue(hDatabase,hRecord,"State",_out._data._state);
			sqlGetValue(hDatabase,hRecord,"UsedUserID",_out._data._used_user_id);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_QUERY_DEPOSIT,0,sizeof(DL_GP_O_BankQueryDeposit),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}

		ZeroMemory(&_out,sizeof(DL_GP_O_BankQueryDeposit));
		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_QUERY_DEPOSIT,1,sizeof(DL_GP_O_BankQueryDeposit), pSourceData->uIndex, pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//�޸���������
UINT CServiceDataBaseHandle::OnBankChangePassword(DataBaseLineHead* pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_BankChPwd))
	{
		return 0;
	}

	DL_GP_I_BankChPwd* in_ptr = (DL_GP_I_BankChPwd*)pSourceData;
	DL_GP_O_BankChPwd _out;

	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_ChangeBankPasswd",true);
		if(hRecord < (HANDLE)0x1000)
		{
			return 0;
		}
		sqlSPAddPara(hDatabase,hRecord,"@UserID",in_ptr->_data._user_id);
		sqlSPAddPara(hDatabase,hRecord,"@MD5PassOld",in_ptr->_data._MD5Pass_old, sizeof(in_ptr->_data._MD5Pass_old));//������
		sqlSPAddPara(hDatabase,hRecord,"@MD5PassNew",in_ptr->_data._MD5Pass_new, sizeof(in_ptr->_data._MD5Pass_new));//������
		sqlSPExec(hDatabase,hRecord,true);

		int nRet = sqlSPGetReturnValue(hDatabase,hRecord);

		sqlCloseRecord(hDatabase,hRecord);

		// ֻ�гɹ���ʧ�����ֽ��
		if (nRet == 0)
		{
			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_CHPWD,HC_GP_BANK_CHPWD_SUC,
				sizeof(DL_GP_O_BankChPwd),pSourceData->uIndex,pSourceData->dwHandleID);
		}
		else
		{
			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_CHPWD,HC_GP_BANK_CHPWD_ERR_PWD,
				sizeof(DL_GP_O_BankChPwd),pSourceData->uIndex,pSourceData->dwHandleID);
		}
	
	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//��ȡ����״��
UINT CServiceDataBaseHandle::OnBankGetFinanceInfo(DataBaseLineHead* pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_FinanceInfo))
	{
		return 0;
	}

	DL_GP_I_FinanceInfo* in_ptr = (DL_GP_I_FinanceInfo*)pSourceData;
	DL_GP_O_FinanceInfo _out;

	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_Bank_GetFinance",true);
		if(hRecord < (HANDLE)0x1000)
		{
			return 0;
		}
		sqlSPAddPara(hDatabase,hRecord,"@UserID",in_ptr->_data.iUserID);
		sqlSPExec(hDatabase,hRecord,true);

		int nRet = sqlSPGetReturnValue(hDatabase,hRecord);

		// ֻ�гɹ���ʧ�����ֽ��
		_out._data.iUserID = in_ptr->_data.iUserID;
		if (nRet == 0)
		{
			sqlGetValue(hDatabase,hRecord,"BankMoney",_out._data.i64BankMoney);
			sqlGetValue(hDatabase,hRecord,"WalletMoney",_out._data.i64WalletMoney);
			sqlGetValue(hDatabase,hRecord,"Lotteries",_out._data.iLotteries);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_FINANCE_INFO,HC_GP_BANK_FINANCE_INFO_SUC,
				sizeof(DL_GP_O_FinanceInfo),pSourceData->uIndex,pSourceData->dwHandleID);
		}
		else
		{
			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_BANK_FINANCE_INFO,HC_GP_BANK_FINANCE_INFO_ERR,
				sizeof(DL_GP_O_FinanceInfo),pSourceData->uIndex,pSourceData->dwHandleID);
		}

		sqlCloseRecord(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

//��ȡ�����¼����Ϸ
UINT CServiceDataBaseHandle::OnLastLoginGame(DataBaseLineHead* pSourceData)
{
	//Ч������
	if (pSourceData->DataLineHead.uSize != sizeof(DL_GP_I_LastLoginGame))
	{
		return 0;
	}

	DL_GP_I_LastLoginGame* in_ptr = (DL_GP_I_LastLoginGame*)pSourceData;
	DL_GP_O_LastLoginGame _out;

	try
	{
		HANDLE hDatabase = m_pDataBaseManage->hDBNative;
		HANDLE hRecord = sqlSPSetNameEx(hDatabase,"SP_GetLastLoginGame",true);
		if(hRecord < (HANDLE)0x1000)
		{
			return 0;
		}
		sqlSPAddPara(hDatabase,hRecord,"@UserID",in_ptr->iUserID);
		sqlSPExec(hDatabase,hRecord,true);

		int nRet = sqlSPGetReturnValue(hDatabase,hRecord);


		while(!sqlEndOfFile(hDatabase,hRecord))
		{
			ZeroMemory(&_out,sizeof(DL_GP_O_LastLoginGame));

			//sqlGetValue(hDatabase,hRecord,"UserID",_out._data.iRoom,sizeof(_out._data._deposit_id));
			sqlGetValue(hDatabase,hRecord,"KindID",_out._data.iKindID);
			sqlGetValue(hDatabase,hRecord,"GameID",_out._data.iGameID);
			sqlGetValue(hDatabase,hRecord,"LastTime", _out ._data.oelLastTime);	
			sqlGetValue(hDatabase,hRecord,"LoginCount",_out ._data.iLoginCount);

			m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead, DTK_GP_LASTLOGINGAME,0,sizeof(DL_GP_O_LastLoginGame),pSourceData->uIndex,pSourceData->dwHandleID);
			sqlMoveNext(hDatabase,hRecord);
		}

		ZeroMemory(&_out,sizeof(DL_GP_O_LastLoginGame));
		m_pRusultService->OnDataBaseResultEvent(&_out.ResultHead,DTK_GP_LASTLOGINGAME,1,sizeof(DL_GP_O_LastLoginGame), pSourceData->uIndex, pSourceData->dwHandleID);

		sqlSPClose(hDatabase,hRecord);

	}
	catch(...)
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}

	return 0;
}

HANDLE CServiceDataBaseHandle::sqlSPSetNameEx(HANDLE & hHandle,LPCTSTR szSPName,bool bReturnValue)
{

	bool bIsDBNative = false;;
	if(hHandle == m_pDataBaseManage->hDBNative)
	{
		bIsDBNative = true;
	}

	if (!sqlExec(hHandle,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_pDataBaseManage->SQLConnectReset();

	}

	if (bIsDBNative)
	{
		hHandle = m_pDataBaseManage->hDBNative;
	}
	else
	{
		hHandle = m_pDataBaseManage->hDBLogon;
	}

	return  sqlSPSetName(hHandle,szSPName,bReturnValue);

}


/******************************************************************************************************/

extern CString GetAppPath(bool bFource=true);  //yjj 090317
//���캯��
CGameLogonManage::CGameLogonManage(void) : CBaseMainManageForZ()
{
	m_nPort=GAME_LOGON_PORT;
	m_uMaxPeople = 300;
	m_userCustomLogo=NULL;
	m_bCustomFaceNeedCheck=false;
	m_CustomFacePath="CustomFace\\";
	m_CustomFacePathUpload="CustomFace\\";

	//IM����ģ��
	m_imService=NULL;
	m_pISms = NULL;
	//���з���ģ��
	m_pBankService = NULL;

	CString s = GetAppPath();
	m_TalkFilter.LoadFilterMessage(s.GetBuffer());

    // PengJiLin, 2010-6-2
    m_bRunBySoftware = FALSE;
    ::InitializeCriticalSection(&g_CriticalSection);
}

//�������� 
CGameLogonManage::~CGameLogonManage(void)
{
	//�����½
	for(int i=0; i<m_UserList.GetCount(); i++)
	{
		//ԭ����GetAt(i),�ָ�ΪGetAt(0)����Ϊ��Ҫɾ�������˵ģ�����һ������ͷɾ������
		delete (LPUSER)m_UserList.GetAt(0);
		//m_UserList. .SetAt(i, NULL);
	}
	//��Ҫԭ����SetAt(i,NULL)��ֱ��ɾ�����е�������Ի�����ڴ�й¶
	m_UserList.RemoveAll();

	if (m_userCustomLogo)
		delete m_userCustomLogo;

    // PengJiLin, 2010-6-3
    if(NULL != g_lpfn_Reslease)
    {
        g_lpfn_Reslease();
    }
    ::DeleteCriticalSection(&g_CriticalSection);
}

//ֹͣ����
bool CGameLogonManage::OnStop()
{
	KillTimer(TID_UPDATE_LIST);
	KillTimer(IDT_CHECK_DATA_CONNECT);
	return true;
}

//��ȡ��Ϣ����
bool CGameLogonManage::PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)
{
	//���ð汾��Ϣ
	pKernelData->bMaxVer=GAME_PLACE_MAX_VER;
	pKernelData->bLessVer=GAME_PLACE_LESS_VER;

	//����ʹ�����ݿ�
	pKernelData->bLogonDataBase=TRUE;
	pKernelData->bNativeDataBase=TRUE;
	pKernelData->bStartSQLDataBase=TRUE;

	//����ʹ������
	pKernelData->bStartTCPSocket=TRUE;
	GetINIFile();
	pInitData->uListenPort=m_nPort;//GAME_LOGON_PORT;
	pInitData->uMaxPeople=m_uMaxPeople;
	pInitData->iSocketSecretKey = SECRET_KEY;
	//�ϴ�ͷ���ʼ��
	CreateDirectory(m_CustomFacePath,0);
	CreateDirectory(m_CustomFacePathUpload,0);
	m_userCustomLogo=new CUserCustomLogo((CWnd*)this);

	//����IM������
	m_imService=new CIMService((CWnd*)this);

	//�������з���ģ����
	m_pBankService = new CBankService4Z((CWnd*)this);

	//����SMSģ��
	::ModuleCreateInstance(MODUAL_PLUGIN_Z_SMS,CLSID_ZServ_Sms,\
		_T("CreateSmsObject"),(LPVOID FAR**)&m_pISms);

	return true;
}
void CGameLogonManage::GetINIFile()
{

	DWORD cfgHandle=cfgOpenFile("BZGameLogon.BCF");
	if(cfgHandle<0x10)
		return;

	m_uMaxPeople=cfgGetValue(cfgHandle,"SERVERSET","MaxPeople",300);
	m_nPort=cfgGetValue(cfgHandle,"SERVERSET","MainServerPort",3015);

	m_CustomFacePath=cfgGetValue(cfgHandle,"SERVERSET","CustomFaceSave","CustomFace");
	if(m_CustomFacePath.Right(1)!="\\")
		m_CustomFacePath+"\\";
	m_CustomFacePathUpload=cfgGetValue(cfgHandle,"SERVERSET","CustomFaceUpload","CustomFace");
	if(m_CustomFacePathUpload.Right(1)!="\\")
		m_CustomFacePathUpload+"\\";
	m_bCustomFaceNeedCheck=(bool)cfgGetValue(cfgHandle,"SERVERSET","CustomFaceNeedCheck",0);

	for (int i = 0; i < 10; ++i)
	{
		char szKey[32];
		_stprintf(szKey,"SMS_Mobile_VCode_Format%d",i);
		m_strSmsContent[i] = cfgGetValue(cfgHandle,"Plugin",szKey,"����%s��������֤�룺%s��");
	}
	
	////////////////////////////////////
	////Kylin 20090316��ӷ�ֹ�˻�ͬʱ��½

	m_ID=cfgGetValue(cfgHandle,"SERVERSET","ID",0);//ZID
	////////////////////////////////////

	cfgClose(cfgHandle);

	//CString s=CINIFile::GetAppPath ();
	//CINIFile f( s + "LogonServer.ini");

	//s=f.GetKeyVal ("www.BZW.cn","BZW","");//����������
	//m_uMaxPeople=f.GetKeyVal (s,"uMaxPeople",300);//֧���������

	//m_nPort=f.GetKeyVal (s,"m_iMainserverPort",3015);//��½�������˿�

	//m_CustomFacePath=f.GetKeyVal(s,"CustomFaceSave",CINIFile::GetAppPath()+"CustomFace")+"\\";
	//m_CustomFacePathUpload=f.GetKeyVal(s,"CustomFaceUpload",CINIFile::GetAppPath()+"CustomFace")+"\\";
	//m_bCustomFaceNeedCheck=(bool)f.GetKeyVal(s,"CustomFaceNeedCheck",0);
}

//SOCKET �ر�############################
bool CGameLogonManage::OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, long int lConnectTime)
{

	int UserID = -1;
	for(int i=0; i<m_UserList.GetCount(); i++)
	{
		LPUSER p = (LPUSER)m_UserList.GetAt(i);
		if(p == NULL)
			continue;
		if(p->iSocketIndex ==uSocketIndex)
		{


			{
				IM_GP_I_USEROFFLINE imIUseroffline;
				imIUseroffline.dwUserID=p->UserID;
				m_SQLDataManage.PushLine(&imIUseroffline.DataBaseHead,sizeof(imIUseroffline),DTK_GP_IM_OFFLINE,0,0);
				MSG_ZDistriInfo info;
				info.dwUserID = imIUseroffline.dwUserID;
				info.nZID = m_ID;
				m_pDistriManager->SendNormalData(&info, sizeof(info), MDM_ZD_PACKAGE, ASS_ZD_LOGON, HDC_ZD_DISCONNECT);
			}
			//ԭ����ֻʹ����SetAt(i,NULL)�����ڴ��л��ǻ�������ݣ�����ڴ�й¶
			//���ڿ���ֱ�ӴӶ�����ɾ������Ϊ�Ѿ�ȡ�����е����ݣ����Կ����ȴӶ�����ɾ��
			//Fred Huang 2008-07-18
			m_UserList.RemoveAt(i);
			UserID = p->UserID;
			IM_GP_I_USEROFFLINE imIUseroffline;
			imIUseroffline.dwUserID=UserID;
			m_SQLDataManage.PushLine(&imIUseroffline.DataBaseHead,sizeof(imIUseroffline),DTK_GP_IM_OFFLINE,0,0);
			delete p;
			//m_UserList.SetAt(i, NULL);
			p = NULL;
			break;
		}
	}
	return true;
}

HANDLE CGameLogonManage::sqlSPSetNameEx(HANDLE & hHandle,LPCTSTR szSPName,bool bReturnValue)
{

	bool bIsDBNative = false;;
	if(hHandle == m_SQLDataManage.hDBNative)
	{
		bIsDBNative = true;
	}

	if (!sqlExec(hHandle,TEXT("select top 1 * from TGameServerInfo")))
	{
		m_SQLDataManage.SQLConnectReset();

	}

	if (bIsDBNative)
	{
		hHandle = m_SQLDataManage.hDBNative;
	}
	else
	{
		hHandle = m_SQLDataManage.hDBLogon;
	}

	return  sqlSPSetName(hHandle,szSPName,bReturnValue);

}


int CGameLogonManage::SendDataFail(UINT uUserID, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, UINT bHandleCode, DWORD dwHandleID)
{
	if(bMainID == MDM_GP_PLAYER_2_GM && pData != NULL)
	{
		if(bAssistantID == ASS_GP_TALK_2_GM)
		{
			TCHAR szTmp[32]=TEXT("");
			GM_Broadcast_t msg;
			MSG_GP_S_Talk2GM_t *pMessage = (MSG_GP_S_Talk2GM_t *)pData;
			msg.iUserID = pMessage->iUserID;
			msg.iGMID = pMessage->iGMID;
			memcpy(szTmp, pMessage->szMsg, sizeof(TCHAR)*31);
			szTmp[31] = TEXT('\0');
			if (_tcslen(szTmp)>=31)
			{
				szTmp[28] = TEXT('.');
				szTmp[29] = TEXT('.');
				szTmp[30] = TEXT('.');
			}
			wsprintf(msg.szMsg, TEXT("����Ա[%d]�Ѿ����ߣ���Ϣ[%s]���Ͳ��ɹ���"), pMessage->iGMID, szTmp);
			int iDstIndex = GetIndexByID(pMessage->iUserID);
			return m_TCPSocket.SendData(iDstIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 1, dwHandleID);
		}
		else if(bAssistantID == ASS_GP_MSG_FROM_GM)
		{
			TCHAR szTmp[32]=TEXT("");
			GM_Broadcast_t msg;
			GM_Broadcast_t *pMessage = (GM_Broadcast_t *)pData;
			msg.iUserID = pMessage->iUserID;
			msg.iGMID = pMessage->iGMID;
			memcpy(szTmp, pMessage->szMsg, sizeof(TCHAR)*31);
			szTmp[31] = TEXT('\0');
			if (_tcslen(szTmp)>=31)
			{
				szTmp[28] = TEXT('.');
				szTmp[29] = TEXT('.');
				szTmp[30] = TEXT('.');
			}
			wsprintf(msg.szMsg, TEXT("���[%d]�Ѿ����ߣ���Ϣ[%s]���Ͳ��ɹ���"), pMessage->iUserID, szTmp);
			int iDstIndex = GetIndexByID(pMessage->iGMID);
			return m_TCPSocket.SendData(iDstIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_TALK_2_GM, 1, dwHandleID);
		}
	}
	if (bMainID == MDM_GP_IM)
	{
		if (bAssistantID == ASS_IMC_ADDREQUEST)
		{
			MSG_IM_C_ADDREQUEST *msg=(MSG_IM_C_ADDREQUEST*)pData;
			if (msg != NULL) 
			{
				int iDstIndex = GetIndexByID(msg->dwUserID);
				m_TCPSocket.SendData(iDstIndex, pData, uBufLen,MDM_GP_IM,ASS_IMC_ADDREQUEST,2,0);
			}
		}
		if (bAssistantID == ASS_IMS_MSG)
		{
			MSG_IM_C_MSG *msg=(MSG_IM_C_MSG*)pData;
			if (msg != NULL) 
			{
				int iDstIndex = GetIndexByID(msg->dwUserID);
				m_TCPSocket.SendData(iDstIndex, pData, uBufLen,MDM_GP_IM,ASS_IMS_USERNOTONLINE,0,dwHandleID);
			}
		}
	}
	return 0;
}


/******************************************************************************************************/

//���캯��
CGameLogonModule::CGameLogonModule(void) 
{
	return;
}

//��������
CGameLogonModule::~CGameLogonModule(void)
{
	return;
}

//��ʼ������ 
bool CGameLogonModule::InitService(ManageInfoStruct * pInitData)
{
	try
	{
		return m_LogonManage.Init(pInitData,&m_DataBaseHandle);
	}
	catch (CAFCException * pException)	{ pException->Delete();	TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	catch (...)	{ TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	return false;
}

//ж�غ���
bool CGameLogonModule::UnInitService()
{
	try
	{
		return m_LogonManage.UnInit();
	}
	catch (CAFCException * pException)	{ pException->Delete();TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);	}
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	return false;
}

//��ʼ���� 
bool CGameLogonModule::StartService(UINT &errCode)
{
	errCode=0;
	try
	{
		return m_LogonManage.Start();
	}
	catch (CAFCException * pException)	
	{
		errCode=pException->GetErrorCode();
		pException->Delete();
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);	
	}
	catch (...) 
	{
		TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
	}
	return false;
}

//ֹͣ���� 
bool CGameLogonModule::StoptService()
{
	try
	{
		return m_LogonManage.Stop();
	}
	catch (CAFCException * pException)	{ pException->Delete();	TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	return false;
}

//ɾ������
bool CGameLogonModule::DeleteService()
{
	try	{ delete this; }
	catch (...) {TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);}
	return true;
}

/******************************************************************************************************/

