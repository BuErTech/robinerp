#pragma once


//////////////////////////////////////////////////////////////////////////
///��̨W�������ͻ����࣬����洢�����գ�������W��������ص�����
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

class CBackWSocket :
	public IClientSocketService
{
public:
	CBackWSocket(void);
public:
	~CBackWSocket(void);

public:
	///�����ȡ��Ϣ�ص�����
	bool OnSocketReadEvent(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket);
	///����������Ϣ�ص�����
	bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket* pClientSocket);
	///����ر���Ϣ�ص�����
	bool OnSocketCloseEvent();

public:
	///�������ӻص�����
	bool OnConnect(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize, CTCPClientSocket* pClientSocket); 
public:
	///���ӵ�������
	bool ConnectToServer();
	///�Ͽ�����
	bool closesocket();
	///��¼
	bool Longo();
	///���Ͷ��û��Ĳ���
	bool SendOpera(GM_OperateByID_t* opera);
private:
	CTCPClientSocket			m_ClientSocket;

public:
	HWND						m_hwnd;			///������Ϣ�Ĵ���
	GM_MSG_O_LogonResult_t*		m_pLogonInfo;   ///��¼����
	ComRoomInfo*				m_pRoomInfo;	///������Ϣ
};
