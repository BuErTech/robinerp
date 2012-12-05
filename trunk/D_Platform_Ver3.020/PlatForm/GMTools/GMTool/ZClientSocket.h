#pragma once

//////////////////////////////////////////////////////////////////////////
///Z�������ͻ����࣬����洢�����գ�������Z��������ص�����
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "../../../SDK/include/socket/TCPClientSocket.h"
#include "CenterServerhead.h"
#include "CommandMessage.h"
#include "comstruct.h"
#include "gameplacemessage.h"
#include "iptypes.h"
#include "iphlpapi.h"
#include "sha1.h"
#include "MD5.h"
#include "GM_MessageDefine.h"

#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class CZClientSocket :
	public IClientSocketService
{
public:
	CZClientSocket(void);
	~CZClientSocket(void);

public:
	//ʵ��IClientSocketService�������ӿ�
public:
	///�����ȡ��Ϣ
	bool OnSocketReadEvent(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///����������Ϣ
	bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket* pClientSocket);
	///����ر���Ϣ
	bool OnSocketCloseEvent();


public:
	///���ӵ�������
	bool ConnectToServer();

	///���������ص���Ϣ��Ӧ
private:
	///���Ӻ�ص�����
	bool OnConnect(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///��¼��ص�����
	bool OnLongo(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///��ȡ��Ϸ�б�Ļص�����
	bool OnGetGameList(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///�յ�GM��Ϣ�Ļص�����
	bool OnGMMessage(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///�յ��û�˽�ĵ���Ϣ
	bool OnTalkMsg(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);

	///�����������ָ��
public:
	///��¼
	bool Longo();
	///��ȡ��Ϸ�б�
	bool GetGameList();
	///��ȡ��Ϸ����
	bool GetGameRoom(UINT uKindID, UINT uNameID);
	///���Ͳ���
	bool SendOperate(GM_OperateByID_t *Operate);
	///���������û�
	bool SendFindUser(GM_GetOperationByPlayer_t *play);
	///����˽�ĺ͹���
	bool SendTalk(GM_Broadcast_t *talk);

private:
	///��ȡ������
	CString innerGetMac();
	CString coreGetCode();

private:
	CTCPClientSocket	m_ClientSocket;

public:
	CenterServerMsg				m_ServerInfo;					///Z��������Ϣ
	CString						m_UerseName, m_PassWord;		///�û���������
	GM_MSG_O_LogonResult_t		m_LogonInfo;					///��¼��Ϣ
	MSG_GP_S_LogonByNameStruct	m_LogonByName;					///��¼��Ϣ
	GM_MSG_S_LogonByName_t      m_GMLogin;						///��¼���ݰ�

	GM_Broadcast_t				m_GMTalkMsg;					///����˽�����ݰ�
	GM_OperatedPlayerInfo_t		m_GMFindUser;					///�����û����ݰ�

	
	map<int, GM_AwardType_t*> m_AwardType;						///����������
	map<int, GM_GetPropInfo_t*> m_PropList;						///���߱�
	
	bool m_GetAwardTypeing;										///�Ƿ��ȡ����������
	bool m_GetAwardListing;										///�Ƿ��ȡ�����б���
	
	vector<ComKindInfo*> m_KindList;							///��Ϸ���ͱ�
	vector<ComNameInfo*> m_NameList;							///��Ϸ���ֱ�
	vector<ComRoomInfo*> m_RoomList;							///��Ϸ�����
};
