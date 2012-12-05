#pragma once

//////////////////////////////////////////////////////////////////////////
///W�������ͻ����࣬����洢�����գ�������W��������ص�����
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "../../../SDK/include/socket/TCPClientSocket.h"
#include "comstruct.h"
#include "gameplacemessage.h"
#include "basemessage.h"
#include "GameUserManage.h"
#include "gameroommessage.h"
#include "comstruct.h"
#include "CommandMessage.h"
#include "GM_MessageDefine.h"

static CString LOGIN_ERR_STRING[16] = {"δ֪����", "��½�ɹ�", "�û�������", "�û��������", "�û��ʺŽ���", "��½ IP ��ֹ",
"����ָ����ַ", "��Ա��Ϸ����", "������������", "�ʺ�����ʹ��", "�����Ѿ���", "�����û��б�", "ȫ���û��б�",
"��ͣ��½����", "��Ϸ����", "ʱ�䵽��"};


class CWClientSocket :
	public IClientSocketService
{
public:
	CWClientSocket();
	CWClientSocket(GM_MSG_O_LogonResult_t* plogonInfo, ComRoomInfo* pRoomInfo);
public:
	~CWClientSocket(void);


public:
	//ʵ��IClientSocketService�������ӿ�
public:
	//�����ȡ��Ϣ
	bool OnSocketReadEvent(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	//����������Ϣ
	bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket* pClientSocket);
	//����ر���Ϣ
	bool OnSocketCloseEvent();

public:
	///���Ӻ�ص�����
	bool OnConnect(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket); 
	///��ȡ��Ϸ�б�Ļص�����
	bool OnGetUserList(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket); 
	///�û������ص�����
	bool OnUserAction(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket); 
	///������Ϣ�ص�����
	bool OnRoomMsg(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///����
	bool OnLockMsg(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);

public:
	///���ӵ�������
	bool ConnectToServer();
	///�ر�����
	bool CloseSocket();
	///��¼
	bool Longo();
	///���Ͳ���
	bool SendOpera(GM_OperateByID_t* opera);
	///����
	bool LockDesk(int UserID, int iDeskIndex, int state);
	///��������
	bool LockDesk(int UserID, int lower, int upper, int state);
	///��ӡ��־
	bool SendPrintLogCmd(GM_MSG_PrintLog_t* pCmd);

private:
	///��ȡ�û����
	UINT GetUserSortIndex(UserInfoStruct * pUserItem);
	///
	UINT GetUserConnection(long int dwUserID);

private:
	CTCPClientSocket			m_ClientSocket;

public:
	HWND						m_hwnd;				///������Ϣ����
	GM_MSG_O_LogonResult_t*		m_pLogonInfo;		///��¼��Ϣ
	ComRoomInfo*				m_pRoomInfo;		///������Ϣ	

	CGameUserManage				m_UserManage;		///�û������
	CDeskStation				m_DeskStation;		///����״̬	
};
