#include "StdAfx.h"
#include "BackWSocket.h"



CBackWSocket::CBackWSocket(void):
m_ClientSocket(this)
{
}

CBackWSocket::~CBackWSocket(void)
{
}

///����������Ϣ�ص�����
bool CBackWSocket::OnConnect(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//���������Ϣ
	{

		pClientSocket->SendData(MDM_CONNECT, ASS_NET_TEST, 0);//������Ϣ����
		return true;
	}
	else if ((pNetHead->bAssistantID == 2) || (pNetHead->bAssistantID == 3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		return Longo();
	}
}

///�����ȡ��Ϣ�ص�����
bool CBackWSocket::OnSocketReadEvent(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket)
{
	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:	//������Ϣ 
		{
			return OnConnect(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case MDM_GR_LOGON:
		{
			switch (pNetHead->bAssistantID)
			{
			case ASS_GR_LOGON_SUCCESS:
				{
					PostMessage(m_hwnd, MSG_CMD_BACKWLOGIN, 0, 0);
					break;
				}
			case ASS_GR_LOGON_ERROR:
				{
					pClientSocket->CloseSocket(true);
					AfxMessageBox(ERR_RP_STRING[pNetHead->bHandleCode]);
					break;
				}
			}
			break;
		}
	}
	return true;
}
//����������Ϣ�ص�����
bool CBackWSocket::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket* pClientSocket)
{
	if (uErrorCode!=0)	//���ӳ��ִ���
	{
		AfxMessageBox("����W������ʧ��!");
		//PostQuitMessage(0);
		return true;
	}
	return true;//���ӳɹ�
}
//����ر���Ϣ�ص�����
bool CBackWSocket::OnSocketCloseEvent()
{
	return true;
}


///���ӵ�������
bool CBackWSocket::ConnectToServer()
{
	return m_ClientSocket.Connect(m_pRoomInfo->szServiceIP, m_pRoomInfo->uServicePort);
}

///�Ͽ�����
bool CBackWSocket::closesocket()
{
	return m_ClientSocket.CloseSocket(true);
}

///��¼
bool CBackWSocket::Longo()
{
	MSG_GR_S_RoomLogon RoomLogon;
	RoomLogon.uGameVer = 8;//m_pRoomInfo->dwGameLSVer;//m_GameInfo.dwGameLSVer;
	RoomLogon.uRoomVer = GAME_PLACE_MAX_VER;
	RoomLogon.uNameID = m_pRoomInfo->uNameID;
	RoomLogon.dwUserID =  m_pLogonInfo->dwUserID;
	lstrcpy(RoomLogon.szMD5Pass,m_pLogonInfo->szMD5Pass);

	return m_ClientSocket.SendData(&RoomLogon,sizeof(RoomLogon), MDM_GR_LOGON, ASS_GR_LOGON_BY_ID, 0);
}

///���Ͷ��û��Ĳ���
bool CBackWSocket::SendOpera(GM_OperateByID_t* opera)
{
	opera->iGMID = m_pLogonInfo->dwUserID;
	m_ClientSocket.SendData(opera, sizeof(GM_OperateByID_t), MDM_GAMEMASTER_MESSAGE, ASS_GM_MESSAGE_OPERATE_ACT, 0);
	return m_ClientSocket.CloseSocket();
}