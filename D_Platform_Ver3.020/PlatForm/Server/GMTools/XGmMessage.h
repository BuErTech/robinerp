#pragma once
#include "InterfaceForGM.h"


// Get individual number
#define FIRST_IPADDRESS(x)  ((x>>24) & 0xff)
#define SECOND_IPADDRESS(x) ((x>>16) & 0xff)
#define THIRD_IPADDRESS(x)  ((x>>8) & 0xff)
#define FOURTH_IPADDRESS(x) (x & 0xff)


/// 
/// GM��Ϣ����ģ���ඨ��
class XGmMessageModule :
	public CGMMessageBase
{
	/// ����ʵ�ֽӿڣ����ڷ������ݿ⣬���·�����
	IGMFunction *m_pFunction;

private:
	/// �޲ι��캯�����������캯������ֵ��������������
	XGmMessageModule();
	XGmMessageModule(const XGmMessageModule &);
	XGmMessageModule & operator=(const XGmMessageModule &);

public:
	XGmMessageModule(IGMFunction *pFunction);
	virtual ~XGmMessageModule(void);

public:
	/// �ӿڶ���
	/// ��Ϣ������
	virtual bool OnSocketMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///���ݿ�������
	virtual bool OnDataBaseResultEvent(DataBaseResultLine * pResultData);
private:

	//////////////////////////////////////////////////////////////////////////
	/// Socket�������

	/// ������ҷ���
	bool OnGetAwardType(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	/// GMȡ��ָ����ҵı�������¼
	bool OnGetPunishedUser(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	/// ����ҽ��в���
	bool OnOperateUserByID(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	/// ������Ϣ����ĳ�ض���ҷ���Ϣ
	bool OnGMSendMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///// ȡ�������
	//bool OnGetLogonUsers(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	/// ȡ�õ���ID�����ƶ�Ӧ��ϵ
	bool OnGetPropInfo(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	/// gm��¼
	bool OnGMLogonOn(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);


	//////////////////////////////////////////////////////////////////////////
	/// ���ݿ�������غ���

	/// GM��ȡָ����ҵ���ʷ��������¼
	bool OnGMGetPunishedUserResult(DataBaseResultLine *pResultData);
	/// GM��¼���
	bool OnGMLogonOnResult(DataBaseResultLine *pResultData);
	/// GM��ȡ�����������
	bool OnGMGetAwardTypeListResult(DataBaseResultLine *pResultData);
	/// GM������ҽ��
	bool OnGMPutOperationUserResult(DataBaseResultLine *pResultData);
	/// GM��ȡ����ID�����ƶ�Ӧ��ϵ���
	bool OnGMGetPropInfoResult(DataBaseResultLine *pResultData);

	/// IP��ַת��Ϊ�ַ���
	TCHAR * ULongToIP(ULONG dwIP, TCHAR * szIP);
	TCHAR * GetTimeString(TCHAR *szTime, int iMinutes);
};

