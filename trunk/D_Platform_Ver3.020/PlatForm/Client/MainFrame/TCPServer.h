#pragma once

#include "xsocketbase.h"

#include <WinSock2.h>
#include <Windows.h>
#include <string>
using namespace std;


///�������ݰ��ṹͷ
struct Net__MessageHead
{
	DWORD						uMessageSize;						///���ݰ���С
	DWORD						bMainID;							///����������
	DWORD						bAssistantID;						///������������ ID
	DWORD						bHandleCode;						///���ݰ��������
	DWORD						bReserve;							///�����ֶ�
};


//���涨��
#define SED_SIZE				60000									//��������С
#define RCV_SIZE				30000									//��������С

//״̬���� 
#define NO_CONNECT				0								//û������
#define CONNECTING				1								//��������
#define CONNECTED				2								//�ɹ�����

#define CONNECTPROXY1			3
#define CONNECTPROXY2			4
#define CONNECTPROXY3			5
#define CONNECTPROXY4			6

//#define __PROXY__ ///�������ʾʹ�ô������������


///���ӳɹ���Ϣ 
struct MSG_S_ConnectSuccess__
{
	BYTE						bMaxVer;							///���°汾����
	BYTE						bLessVer;							///��Ͱ汾����
	BYTE						bReserve[2];						///�����ֶ�
};

#define NET_HEAD_SIZE			sizeof(NetMessageHead)				///���ݰ�ͷ
#define MAX_SEND_SIZE			2044								///�����Ϣ��


////�ͻ����������ӿ�
//interface IServerSocketService
//{
//	//�ӿں��� 
//public:
//	//////////////////////////////////////////////////////////////////////////
//	virtual bool OnSocketAccept(SOCKET iSocket) = 0;
//	//�����ȡ��Ϣ
//	virtual bool OnSocketReadEvent(SOCKET iSocket, void * pNetData, UINT uDataSize)=0;
//	//����ر���Ϣ
//	virtual bool OnSocketCloseEvent(SOCKET iSocket)=0;
//};
//


class CTCPServerSocket : public CWnd
{
DECLARE_MESSAGE_MAP()

public:
	CTCPServerSocket(IServerSocketService* pSocket);
	~CTCPServerSocket(void);



	//��������
private:
	int									m_iReadBufLen;					//����������
	BYTE								m_bConnectState;				//����״̬
	SOCKET								m_hSocket;						//SOCKET ���
	IServerSocketService				*m_pIService;					//����ӿ�
	SOCKET								m_hClientSocket;
	BYTE								m_szBuffer[RCV_SIZE];			//���ݻ�����

	int									m_iPort;

	//HWND								m_hWnd;						//��Ϣ����

	//��������
private:

	//�Ƿ��Ѿ�����
	BYTE GetSocketState() { return m_bConnectState; }
	//��������
	bool Listen();
	//�󶨵�ַ
	bool Bind(UINT uPort);


	//���ʹ���
	string TranslateError(UINT uErrorCode);

	//SOCKET ��Ϣ�������
	LRESULT	OnSocketNotifyMesage(WPARAM wParam, LPARAM lParam);



	//�ڲ����� 
private:
	//��ʼ�� SOCKET ����
	bool InitSocketWnd();
	//�ر� SOCKET ����
	bool CloseSocketWnd();

	void CloseSocket(INT32 nSocketID);

public:
	/// �ر�ָ��Socket��ŵ�socket
	void CloseSocket();
	int SendData(UINT8 * pBuffer, int nLen);
	int GetSocketPort() {return m_iPort;}
	HWND GetHWDN() {return m_hWnd;} 
	void CloseClientSocket();

	//���ͺ���
	int SendData(int nSocketID, void * pData, int uSize, int bMainID, int bAssistantID, int bHandleCode);
	//������ͺ���
	int SendData(int nSocketID, int bMainID, int bAssistantID, int bHandleCode);
	///���ͺ���
	int SendData(int nSocketID, UINT8* pBuffer, int nLen);

};


