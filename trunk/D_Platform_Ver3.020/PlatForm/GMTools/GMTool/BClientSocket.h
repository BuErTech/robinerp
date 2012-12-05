#pragma once

//////////////////////////////////////////////////////////////////////////
///B�������ͻ����࣬����洢�����գ�������B��������ص�����
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "../../../SDK/include/socket/TCPClientSocket.h"
#include "CenterServerhead.h"
#include "CommandMessage.h"

#include <fstream>
#include <string>
using namespace std;


class CBClientSocket :
	public IClientSocketService
{
public:
	CBClientSocket(void);
public:
	~CBClientSocket(void);


	///ʵ��IClientSocketService�������ӿ�
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
public:
	///socket���ӻص�����
	bool OnConnect(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///����Z����������
	bool OnRequreGamePara(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);

private:
	CTCPClientSocket	m_ClientSocket;
	CenterServerMsg		m_par;				///Z����������

	string				m_IP;				///B������IP

};
