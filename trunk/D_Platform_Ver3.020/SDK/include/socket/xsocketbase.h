#pragma once

#include <Windows.h>

#ifdef SOCKET_EXPORTS
#define SOCKET_API __declspec(dllexport)
#else
#define SOCKET_API __declspec(dllimport)
#endif //SOCKET_EXPORTS




typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;


const int MAX_MESSAGE_SIZE = 100000;					///< ������Ϣ��󳤶�
/// �����¼�������
const UINT16 SOCKET_CONNECT			 = 0;
const UINT16 SOCKET_RECEIVE			 = 1;
const UINT16 SOCKET_CLOSE				 = 2;
const UINT16 SOCKET_CONNECT_FAILED	 = 3;
const UINT16 SOCKET_RECEIVE_LESSDATA = 4;
/// ����Ϊ�ⲿ��ȡ�����ṩ�Ĳ����ṹ
struct RECV_DATA_T 
{
	UINT16 wSocketID;		///< �ĸ�Socket�����ݣ�ID��
	UINT16 wType;			///< ��Ϣ�����ͣ�ȡֵΪ���֣�SOCKET_CONNECT, SOCKET_RECEIVE, SOCKET_CLOSE
	UINT8 *pData;			///< ���ݻ�����ָ�룬�����ⲿȷ����ָ�����Ч�Ժ��㹻�Ŀռ�
	UINT32 uDataLen;		///< ���������ݳ��ȣ���Ϊ��������ʱ��ָʾ���������ȣ���Ϊ��������ʱ��ָʾ����ʵ�ʳ���
};

interface ISocket;
interface IServerSocket;
interface IServerSocketService;



//�ͻ����������ӿ�
interface IClientSocketService__
{
	//�ӿں��� 
public:
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(SOCKET iSocket, void * pNetData, UINT uDataSize)=0;
	//����������Ϣ
	virtual bool OnSocketConnectEvent(SOCKET iSocket, UINT uErrorCode)=0;
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent(SOCKET iSocket)=0;
};

//�ͻ����������ӿ�
interface IServerSocketService
{
	//�ӿں��� 
public:
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnSocketAccept(SOCKET iSocket) = 0;
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(SOCKET iSocket, void * pNetData, UINT uDataSize)=0;
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent(SOCKET iSocket)=0;
};


/// XSocket�ṩ�Ĺ���ʼ�������Ļ���
/// 
class XSocketBase
{
public:
	XSocketBase(void){}
	virtual ~XSocketBase(void){}
public:
	/// ��ʼ��Socket���봫��һ���¼����
	virtual INT32 InitialNew(IClientSocketService* pSocketService)=0;
	/// ��ʼ��Socket���봫��һ���¼����
	virtual INT32 Initial(HANDLE hEvent)=0;
	/// �յ��¼�����󣬵��øú�����ȡ��������Ϣ
	virtual INT32 RecvData(RECV_DATA_T &rd)=0;
};


class XServerSocketBase
{
public:
	XServerSocketBase(void){}
	virtual ~XServerSocketBase(void){}
public:
	/// ��ʼ��Socket���봫��һ���¼����
	virtual INT32 Initial(IServerSocketService* pSocketService)=0;
};

extern SOCKET_API bool CreateSocket(XSocketBase **pSocketBase, ISocket **pISocket);
extern SOCKET_API bool CreateServerSocket(XServerSocketBase** pSocketBase, IServerSocket **pIServerSocket);
