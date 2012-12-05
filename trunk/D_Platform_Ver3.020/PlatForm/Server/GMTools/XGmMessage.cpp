#include "XGmMessage.h"
#include <stdio.h>
#include <tchar.h>
#include <atltime.h>

/// ����GM��Ϣ�������
CGMMessageBase *CreateGMModule(IGMFunction *pFunction)
{
	XGmMessageModule *p = new XGmMessageModule(pFunction);
	if (NULL != p)
	{
		return p;
	}
	return NULL;
}
/// ����GM��Ϣ�������
void DestroyGMModule(CGMMessageBase *pBase)
{
	if (NULL != pBase)
	{
		delete pBase;
		pBase = NULL;
	}
}

/// ��ӡ������Ϣ
static void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\GM_Module.txt");
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

/// ���캯��
XGmMessageModule::XGmMessageModule(IGMFunction *pFunction) : m_pFunction(pFunction)
{
}
/// ��������
XGmMessageModule::~XGmMessageModule(void)
{
}
/// ��ֵ�� IP �ַ�
TCHAR * XGmMessageModule::ULongToIP(ULONG dwIP, TCHAR * szIP)
{
	wsprintf(szIP,TEXT("%d.%d.%d.%d"),FOURTH_IPADDRESS(dwIP),THIRD_IPADDRESS(dwIP),SECOND_IPADDRESS(dwIP),FIRST_IPADDRESS(dwIP));
	return szIP;
}
/// ����ת��Ϊ�Ѻ��ַ���
TCHAR * XGmMessageModule::GetTimeString(TCHAR *szTime, int iMinutes)
{
	int iDays = iMinutes / 60 / 24;
	int iHours = iMinutes / 60 - iDays * 24;
	int iMin = iMinutes % 60;
	
	TCHAR szDay[20], szHour[20],szMin[20];

	//modify by lxl 2010-12-6 
	if(iDays != 0)
	{
		wsprintf(szDay, TEXT("%d��"),iDays);
		_tcscat(szTime,szDay);
	}
	
	if(iHours != 0)
	{
		wsprintf(szHour, TEXT("%dСʱ"),iHours);
		_tcscat(szTime,szHour);
	}
	if(iMin != 0)
	{
		wsprintf(szMin, TEXT("%d����"),iMin);
		_tcscat(szTime,szMin);
	}
	return szTime;
}

/// ����ID�������б�
bool XGmMessageModule::OnGetPropInfo(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	/// �޲���
	DataBaseLineHead dbHead;
	m_pFunction->PushDataLine(&dbHead, sizeof(dbHead), DTK_GET_PROP_INFO_GM, uIndex, dwHandleID);
	return true;
}
/// ��������
bool XGmMessageModule::OnGetAwardType(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	/// �޲���
	DataBaseLineHead dbHead;
	m_pFunction->PushDataLine(&dbHead, sizeof(dbHead), DTK_GET_AWARD_TYPE_GM, uIndex, dwHandleID);
	return true;
}
/// GMȡ��ָ����ҵı�������¼
bool XGmMessageModule::OnGetPunishedUser(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	//Ч������
	if (uSize!=sizeof(GM_GetOperationByPlayer_t)) return false;///��֤�ṹ�Ƿ�һ��
	if (NULL == pData)
	{
		return false;
	}

	//����
	GM_GetOperationByPlayer_t * pPlayInfo=(GM_GetOperationByPlayer_t *)pData;
	pPlayInfo->playerInfo.szName[sizeof(pPlayInfo->playerInfo.szName)/sizeof(pPlayInfo->playerInfo.szName[0])-1]=0;

	//�������
	GM_DT_I_GetOperatedUser_t dl_GetHistory;
	dl_GetHistory.requestPlayer = *pPlayInfo;

	m_pFunction->PushDataLine(&dl_GetHistory.DataBaseHead,sizeof(dl_GetHistory), DTK_GET_OPEATED_USER_GM,uIndex,dwHandleID);//�ύ���ݿ�
	return true;
}
/// GM������ң������ǽ��������ǳͷ�
bool XGmMessageModule::OnOperateUserByID(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	//Ч������
	if (uSize!=sizeof(GM_OperateByID_t)) return false;///��֤�ṹ�Ƿ�һ��
	if (NULL == pData)
	{
		return false;
	}

	//����
	GM_OperateByID_t * pOperation=(GM_OperateByID_t *)pData;


	if (!m_pFunction->IsUserAlreadyLogon(pOperation->iUserID))
	{
		m_pFunction->SendDataByUser(pOperation->iUserID, pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode, 0);
		return true;
	}

	//�������
	GM_DT_I_OperateUser_t dl_OperateUser;
	dl_OperateUser.operation = *pOperation;

	m_pFunction->PushDataLine(&dl_OperateUser.DataBaseHead,sizeof(dl_OperateUser), DTK_PUT_OPEATION_USER_GM,uIndex,dwHandleID);//�ύ���ݿ�
	int iTargetIndex = m_pFunction->GetIndexByID(pOperation->iUserID);

	TCHAR szTime[128];
	ZeroMemory(szTime,128);
	GetTimeString(szTime, pOperation->iWorkMinutes);
	GM_Broadcast_t msg;

	//modify by lxl 2010-12-6 ��ʾ��Ϣ��ʾ����ǳ�
	char lpBuffer[256];
	GetCurrentDirectory(255,lpBuffer);
	CString ss=lpBuffer;
	ss += "\\Function.ini";
	
	BOOL bShowGmMsg = GetPrivateProfileInt("ShowGmMsg","ShowGmMsg",1, ss);

	//CBcfFile f(CBcfFile::GetAppPath() + "Function.bcf");
	//BOOL bShowGmMsg = f.GetKeyVal("ShowGmMsg","ShowGmMsg",1);
	switch(pOperation->iOperation)
	{
	case GM_ACT_AWORD://		1	///< �������������浽�����У���ʵʱд���û�Ǯ���������Ϣ���͵�Z������
		{
			wsprintf(msg.szMsg, TEXT("��ϲ����[%s]�����Ϸ����Ա[%d]����������Ѵ������У����������µ�¼����ա�"), pOperation->szReason, pOperation->iGMID);
			m_pFunction->SendData(iTargetIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GR_MSG_BE_AWARD, 0, 0);
			wsprintf(msg.szMsg, TEXT("��ϲ���[%s]��[%s]����Ϸ����Ա[%d]������ҵ������ɡ�"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID);
			break;
		}
	case GM_ACT_NO_TALK://		2	///< ���ԣ���������������ǰ��ĳ�����У�GM�����߿ͻ�����ͬʱ���������ڵ�W��������������Ϣ
		{
			wsprintf(msg.szMsg, TEXT("���[%s]��[%s]����Ϸ����Ա[%d]����%s��"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID, szTime);
			if (pOperation->iWorkMinutes == 0)
			{
				wsprintf(msg.szMsg, TEXT("���[%s]��[%s]����Ϸ����Ա[%d]������ԡ�"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID);
			}
			break;
		}
	case GM_ACT_KICK_OUT://		3	///< �߳����䣬��������������ǰ��ĳ�����У�GM�����߿ͻ�����ͬʱ���������ڵ�W��������������Ϣ
		{
			wsprintf(msg.szMsg, TEXT("���[%s]��[%s]����Ϸ����Ա[%d]�߳�����%s��"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID, szTime);
			if (pOperation->iWorkMinutes == 0)
			{
				wsprintf(msg.szMsg, TEXT("���[%s]��[%s]����Ϸ����Ա[%d]�ָ����뷿��Ȩ�ޡ�"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID);
			}
			else
			{
				m_pFunction->SendData(iTargetIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GR_MSG_BE_KICKED, 0, 0);
			}
			if (!bShowGmMsg)
				return true;
			break;
		}
	case GM_ACT_BANDON_ID://	4	///< ��ֹ��¼������ţ���������������ǰ��ĳ�����У�GM�����߿ͻ���ͬʱ���������ڵ�W��������������Ϣ
		{
			wsprintf(msg.szMsg, TEXT("���[%s]��[%s]����Ϸ����Ա[%d]���%s��"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID, szTime);
			if (pOperation->iWorkMinutes == 0)
			{
				wsprintf(msg.szMsg, TEXT("���[%s]��[%s]����Ϸ����Ա[%d]ȡ����š�"), pOperation->szNickName, pOperation->szReason, pOperation->iGMID);
			}
			else
			{
				m_pFunction->SendData(iTargetIndex, &msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GR_MSG_BE_BANDID, 0, 0);
			}
			if (!bShowGmMsg)
				return true;
			break;
		}
	}
	// ���XXX��XXXX����Ϸ����Ա�߳�����%d���ӡ�
	// ���XXX��XXXX����Ϸ����Ա����%d���ӡ�
	// ���XXX��XXXX����Ϸ����Ա������ҵ������ɡ�
	// ���XXX��XXXX����Ϸ����Ա���%d���ӡ�

	
	m_pFunction->SendBatchData(&msg, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 0);

	return true;
}
/// GM�㲥����ĳ�ض���ҷ���Ϣ
bool XGmMessageModule::OnGMSendMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	//Ч������
	if (uSize!=sizeof(GM_Broadcast_t)) return false;///��֤�ṹ�Ƿ�һ��

	GM_Broadcast_t *pMessage = (GM_Broadcast_t *)pData;
	/// �㲥��Ϣ
	if (pMessage->iUserID == 0)
	{
		m_pFunction->SendBatchData(pMessage, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 0);
	}
	else
	{
		pMessage->szMsg[sizeof(pMessage->szMsg)/sizeof(TCHAR)-1] = TEXT('\0');
		int iDstIndex = m_pFunction->GetIndexByID(pMessage->iUserID);
		//DebugPrintf("���[%d]Socket���Ϊ[%d]", pMessage->iUserID, iDstIndex);
		if (iDstIndex < 0)
		{
			GM_Broadcast_t msgTmp;
			msgTmp.iUserID = pMessage->iUserID;
			msgTmp.iGMID = pMessage->iGMID;
			TCHAR szTmp[32];
			memcpy(szTmp, pMessage->szMsg, sizeof(TCHAR)*32);
			szTmp[31] = TEXT('\0');
			if (_tcslen(pMessage->szMsg)>31)
			{
				szTmp[28] = TEXT('.');
				szTmp[29] = TEXT('.');
				szTmp[30] = TEXT('.');
			}
			wsprintf(msgTmp.szMsg, TEXT("���[%d]�Ѿ����ߣ���Ϣ[%s]���Ͳ��ɹ���"), pMessage->iUserID, szTmp);
			m_pFunction->SendData(uIndex, &msgTmp, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 1, dwHandleID);
			return true;
		}
		m_pFunction->SendData(iDstIndex, pMessage, sizeof(GM_Broadcast_t), MDM_GP_PLAYER_2_GM, ASS_GP_MSG_FROM_GM, 0, 0);
	}
	return true;
}

/// GM��¼
bool XGmMessageModule::OnGMLogonOn(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	//Ч������
	if (uSize!=sizeof(GM_MSG_S_LogonByName_t)) return false;///��֤�ṹ�Ƿ�һ��

	//�����½
	GM_MSG_S_LogonByName_t * pLogonInfo=(GM_MSG_S_LogonByName_t *)pData;
	pLogonInfo->szName[sizeof(pLogonInfo->szName)/sizeof(pLogonInfo->szName[0])-1]=0;
	pLogonInfo->szMD5Pass[sizeof(pLogonInfo->szMD5Pass)/sizeof(pLogonInfo->szMD5Pass[0])-1]=0;
	//�������
	GM_DT_I_LogonByName_t DL_Logon;
	DL_Logon.logonInfo=*pLogonInfo;//////��ֵ���ѵ�½�û����붼����ȥ

	ULongToIP(uAccessIP,DL_Logon.szAccessIP);//��IPŪ����

	m_pFunction->PushDataLine(&DL_Logon.DataBaseHead,sizeof(DL_Logon),DTK_LOGON_BY_NAME_GM,uIndex,dwHandleID);//�ύ���ݿ�
	return true;
}

/// ��Ϣ������
bool XGmMessageModule::OnSocketMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)
{
	if (pNetHead->bMainID!=MDM_GAMEMASTER_MESSAGE)
	{
		return false;
	}
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_MESSAGE_AWARD_TYPE:			/// 1���·���������
		OnGetAwardType(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		break;
	case ASS_GM_MESSAGE_OPERATE_HISTORY:	/// 2���·�ĳ��ҵĴ�����ʷ��¼
		OnGetPunishedUser(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		break;
	case ASS_GM_MESSAGE_OPERATE_ACT:		/// 3���ϴ�����ҵĲ���
		OnOperateUserByID(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		break;
	case ASS_GM_MESSAGE_BROADCAST:			/// 4����ĳ�ض���ҷ��ԣ����㲥��Ϣ������
		OnGMSendMessage(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		break;
	case ASS_GM_MESSAGE_PROP_INFO:		/// 6������Z��������ȡ�õ���ID�����ƶ�Ӧ��ϵ
		OnGetPropInfo(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		break;
	case ASS_GM_MESSAGE_GM_LOGON:			/// 7��GM��¼Э�飬����ͨ��ҵ�¼���ò�ͬЭ��
		OnGMLogonOn(pNetHead,pData,uSize,uAccessIP,uIndex,dwHandleID);
		break;
	default:
		break;
	}
	
	return true;
}
///���ݿ�������
bool XGmMessageModule::OnDataBaseResultEvent(DataBaseResultLine * pResultData)
{
	if ((pResultData->uHandleKind<GM_DT_MESSAGE_START) || (pResultData->uHandleKind>GM_DT_MESSAGE_END))
	{
		return false;
	}
	switch (pResultData->uHandleKind)
	{
	case DTK_GET_AWARD_TYPE_GM:		///< 1����ȡ��������
		OnGMGetAwardTypeListResult(pResultData);
		break;
	case DTK_GET_OPEATED_USER_GM:	///< 2����ȡָ����ҵ���ʷ��������¼����ǰ״̬
		OnGMGetPunishedUserResult(pResultData);
		break;
	case DTK_PUT_OPEATION_USER_GM:	///< 3��д�����ҵĲ�������¼����
		OnGMPutOperationUserResult(pResultData);
		break;
	case DTK_GET_PROP_INFO_GM:		///< 4����ȡ����ID�����ƶ�Ӧ��ϵ
		OnGMGetPropInfoResult(pResultData);
	case DTK_LOGON_BY_NAME_GM:		///< 5��GM�ʺŵ�¼��Ϣ
		OnGMLogonOnResult(pResultData);
		break;
	default:
		break;
	}
	
	return true;
}
/// GM������ҽ��
bool XGmMessageModule::OnGMPutOperationUserResult(DataBaseResultLine *pResultData)
{
	/// Ч������
	if (pResultData->LineHead.uSize != sizeof(DataBaseResultLine)) return false;
	BYTE bHandleCode= (pResultData->uHandleRusult == DTR_GM_OPERATE_SUCCESS)? ERR_GM_OPERATOR_SUCCESS : ERR_GM_OPERATOR_FAILED;
	m_pFunction->SendData(pResultData->uIndex,MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_OPERATE_ACT, bHandleCode,pResultData->dwHandleID);
	return true;
}
/// GM��ȡ����ID�����ƶ�Ӧ��ϵ���
bool XGmMessageModule::OnGMGetPropInfoResult(DataBaseResultLine *pResultData)
{
	/// Ч������
	if (pResultData->LineHead.uSize != sizeof(GM_DT_O_GetPropInfoResult_t)) return false;
	/// ��������
	GM_DT_O_GetPropInfoResult_t *pPropInfoResult = (GM_DT_O_GetPropInfoResult_t*)pResultData;

	/// ���ַ��ؽ��
	/// 1��������������Ч���ݣ�������δ����
	if (pResultData->uHandleRusult==DTR_GM_GET_RECORD_OK)
	{
		BYTE bHandleCode = HC_GM_GET_LIST_OK;
		m_pFunction->SendData(pPropInfoResult->ResultHead.uIndex, 
			&pPropInfoResult->propInfo,
			sizeof(pPropInfoResult->propInfo),
			MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_PROP_INFO,
			bHandleCode, pResultData->dwHandleID);

	}
	/// 2���������ݽ�������������Ϊ��Ч���ݣ�ֻ����ָʾ
	else if (pResultData->uHandleRusult==DTR_GM_GET_RECORD_END)
	{
		BYTE bHandleCode = HC_GM_GET_LIST_END;
		m_pFunction->SendData(pPropInfoResult->ResultHead.uIndex, 
			&pPropInfoResult->propInfo,
			sizeof(pPropInfoResult->propInfo),
			MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_PROP_INFO,
			bHandleCode, pResultData->dwHandleID);

	}	return true;
}
/// GM��ȡָ����ҵ���ʷ��������¼
bool XGmMessageModule::OnGMGetPunishedUserResult(DataBaseResultLine *pResultData)
{
	/// Ч������
	if (pResultData->LineHead.uSize != sizeof(GM_DT_O_OperatedUser_t)) return false;
	/// ��������
	GM_DT_O_OperatedUser_t *pOperatedPlayerResult = (GM_DT_O_OperatedUser_t*)pResultData;

	/// ������������Ч����
	BYTE bHandleCode = pResultData->uHandleRusult == DTR_GM_GET_RECORD_OK ? HC_GM_GET_LIST_OK : HC_GM_GET_LIST_END;
	m_pFunction->SendData(pOperatedPlayerResult->ResultHead.uIndex, 
		&pOperatedPlayerResult->operatedPlayer,
		sizeof(pOperatedPlayerResult->operatedPlayer),
		MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_OPERATE_HISTORY,
		bHandleCode, pResultData->dwHandleID);
	return true;
}

/// ȡ���������б���
bool XGmMessageModule::OnGMGetAwardTypeListResult(DataBaseResultLine *pResultData)
{
	/// Ч������
	if (pResultData->LineHead.uSize != sizeof(GM_DT_O_AwardType_t)) return false;
	/// ��������
	GM_DT_O_AwardType_t *pAwardTypeResult = (GM_DT_O_AwardType_t*)pResultData;
	/// ���ַ��ؽ��
	/// 1��������������Ч���ݣ�������δ����
	if (pResultData->uHandleRusult==DTR_GM_GET_RECORD_OK)
	{
		BYTE bHandleCode = HC_GM_GET_LIST_OK;
		m_pFunction->SendData(pAwardTypeResult->ResultHead.uIndex, 
			&pAwardTypeResult->awardType,
			sizeof(pAwardTypeResult->awardType),
			MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_AWARD_TYPE,
			bHandleCode, pResultData->dwHandleID);
	}
	/// 2���������ݽ�������������Ϊ��Ч���ݣ�ֻ����ָʾ
	else if (pResultData->uHandleRusult==DTR_GM_GET_RECORD_END)
	{
		BYTE bHandleCode = HC_GM_GET_LIST_END;
		m_pFunction->SendData(pAwardTypeResult->ResultHead.uIndex,
			MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_AWARD_TYPE,
			bHandleCode, pResultData->dwHandleID);
	}
	return true;
}
/// GM��¼���ؽ��
bool XGmMessageModule::OnGMLogonOnResult(DataBaseResultLine *pResultData)
{
	//Ч������
	if (pResultData->LineHead.uSize!=sizeof(GM_DT_O_LogonResult_t)) return false;

	//�ж��Ƿ񻹴�������
	if (!m_pFunction->IsConnectID(pResultData->uIndex,pResultData->dwHandleID)) return false;

	//��������
	if (pResultData->uHandleRusult==DTR_GM_LOGON_SUCCESS)	
	{
		//��½�ɹ�
		GM_DT_O_LogonResult_t * pLogonResult=(GM_DT_O_LogonResult_t *)pResultData;
		////GSQ����֤һ��
		//if(pLogonResult->logonResult.dwAccID!=5471 && pResultData->uHandleKind!=DTK_GP_RIGSTER_USER)
		//	return false;
		//�ж��Ƿ��Ѿ��ظ���½
		if (m_pFunction->IsUserAlreadyLogon(pLogonResult->logonResult.dwUserID))
		{
			BYTE bHandleCode=ERR_GP_USER_LOGON;
			m_pFunction->SendData(pResultData->uIndex,MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_GM_LOGON, bHandleCode,pResultData->dwHandleID);
			return false;
		}
		//������û�
		m_pFunction->AddLogonUser(pLogonResult->logonResult.dwUserID, pResultData->uIndex, true);

		GM_MSG_O_LogonResult_t MSGLogonResult;
		MSGLogonResult = pLogonResult->logonResult;

		m_pFunction->SendData(pResultData->uIndex,&MSGLogonResult,sizeof(MSGLogonResult),MDM_GAMEMASTER_MESSAGE,
			ASS_GM_MESSAGE_GM_LOGON, ERR_GP_LOGON_SUCCESS,pResultData->dwHandleID);
		return true;
	}
	else
	{
		//��½ʧ��
		BYTE bHandleCode=ERR_GP_ERROR_UNKNOW;
		switch (pResultData->uHandleRusult)
		{
		case DTR_GM_USER_NOT_FOUND:  {bHandleCode=ERR_GP_USER_NO_FIND; break;}
		case DTR_GM_PASSWORD_ERROR:  {bHandleCode=ERR_GP_USER_PASS_ERROR; break;}
		case DTR_GM_HAS_NOT_RIGHT:   {bHandleCode=ERR_GP_USER_VALIDATA; break;}
		case DTR_GM_MACHINECODE_ERR: {bHandleCode=ERR_GP_ACCOUNT_LOCKED; break;}
		case DTR_GM_ALREADY_LOGON:   {bHandleCode=ERR_GP_USER_LOGON; break;}
		default: break;
		}	 
		m_pFunction->SendData(pResultData->uIndex,MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_GM_LOGON,bHandleCode,pResultData->dwHandleID);
		return false;
	}
}
