#pragma once

#ifndef AFC_INTERFACE_FILE
#define AFC_INTERFACE_FILE

/********************************************************************************************/

///����ӿڶ���

/********************************************************************************************/

class CTCPSocket;					///<SOCKET ��
class CTCPSocketForZ;				//SOCKET ��ΪZ���������� Add by ZXD 20090804
class CTCPClientSocket;				///<SOCKET ��
class CAFCDataBaseManage;			///<���ݿ�ģ����
struct NetMessageHead;				///<�������ݰ�ͷ
struct ManageInfoStruct;			///<��ʼ����Ϣ�ṹ
struct KernelInfoStruct;			///<�ں���Ϣ�ṹ
struct DataLineHead;				///<����ͷ�ṹ
struct DataBaseLineHead;			///<���ݿ����ͷ�ṹ
struct DataBaseResultLine;			///<���ݿ���ͷ�ṹ

///�������������ӿ� 
interface IServerSocketService
{
	///�ӿں���
public:
	///����رմ���
	virtual bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, long int lConnectTime)=0;
	///������Ϣ����
	virtual bool OnSocketReadEvent(CTCPSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID)=0;
};
//�������������ӿ� 
interface IServerSocketServiceForZ
{
	//�ӿں���
public:
	//����رմ���
	virtual bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, long int lConnectTime)=0;
	//������Ϣ����
	virtual bool OnSocketReadEvent(CTCPSocketForZ * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID)=0;
};

///�ͻ����������ӿ�
interface IClientSocketService
{
	///�ӿں��� 
public:
	///�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)=0;
	///����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)=0;
	///����ر���Ϣ
	virtual bool OnSocketCloseEvent()=0;
};

///���ݿ�������ӿ�
interface IDataBaseResultService
{
	///�ӿں���
public:
	///���ݿ�������
	virtual bool OnDataBaseResultEvent(DataBaseResultLine * pResultData, UINT uHandleKind, UINT uHandleResult, UINT uResultSize, UINT uIndex, DWORD dwHandleID)=0;
};

///���ݿ⴦�����ӿ�
interface IDataBaseHandleService
{
	///�ӿں���
public:
	///���ò���
	virtual bool SetParameter(IDataBaseResultService * pRusultService, CAFCDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)=0;
	///���ݿ⴦��ӿ�
	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData)=0;
};

///�������ӿ�
interface IModuleManageService
{
	///�ӿں���
public:
	///��ʼ������ 
	virtual bool InitService(ManageInfoStruct * pInitData)=0;
	///ȡ����ʼ������ 
	virtual bool UnInitService()=0;
	///��ʼ������ 
	virtual bool StartService(UINT &errCode)=0;
	///��ʼ������ 
	virtual bool StoptService()=0;
	///ɾ������
	virtual bool DeleteService()=0;
};

/********************************************************************************************/

#endif