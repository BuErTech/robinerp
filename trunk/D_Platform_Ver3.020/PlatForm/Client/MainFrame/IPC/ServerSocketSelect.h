#pragma once

#include "xsocketbase.h"
#include <Windows.h>
#include <stdio.h>

#include "sink.h"


////�ͻ����������ӿ�
////interface IServerSocketService
////{
////	�ӿں��� 
////public:
////	////////////////////////////////////////////////////////////////////////
////	virtual bool OnSocketAccept(SOCKET iSocket) = 0;
////	�����ȡ��Ϣ
////	virtual bool OnSocketReadEvent(SOCKET iSocket, void * pNetData, UINT uDataSize)=0;
////	����ر���Ϣ
////	virtual bool OnSocketCloseEvent(SOCKET iSocket)=0;
////};

struct Net_MessageHead
{
	DWORD						uMessageSize;						///���ݰ���С
	DWORD						dMainID;							///����������
	DWORD						dAssistantID;						///������������ ID
	DWORD						dHandleCode;						///���ݰ��������
	DWORD						dReserve;							///�����ֶ�
};


class CServerSocketSelect: public IServerSocketService
{
public:
	CServerSocketSelect(void);
	~CServerSocketSelect(void);
public:
	bool OnSocketAccept(SOCKET iSocket);
	//�����ȡ��Ϣ
	bool OnSocketReadEvent(SOCKET iSocket, void * pNetData, UINT uDataSize);
	//����ر���Ϣ
	bool OnSocketCloseEvent(SOCKET iSocket);

public:
	bool OnSocketRead(SOCKET iSocket, Net_MessageHead* pNetHead, UINT8* pNetData);

	void SetChannel(IChannelMessageSink *pIChannel) {m_pIChannelMessageSink = pIChannel;}
public:
	IChannelMessageSink				* m_pIChannelMessageSink;				//�ص��ӿ�
};
