//////////////////////////////////////////////////////////////////////////
// ����GM���ܵĽӿڶ���ͷ�ļ�
// GM���ܵľ���ʵ���ɾ���ʵ�������
// ���������ܶ�
// ���ڣ�20100722
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __INTERFACE_FOR_GM_H
#define __INTERFACE_FOR_GM_H

#include "GM_MessageDefine.h"

///
/// GM��Ϣ����ӿڣ�GM��Ϣ�����ɴ˽ӿ���ɣ�
/// Ϊ�����ͷţ�����Ϊ�����������interface
///
class CGMMessageBase
{
public:
	/// ���캯��
	CGMMessageBase(){};
	/// ��������
	virtual ~CGMMessageBase(){};

public:
	/// �ӿڶ���
	/// ��Ϣ������
	virtual bool OnSocketMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID) = 0;
	///���ݿ�������
	virtual bool OnDataBaseResultEvent(DataBaseResultLine * pResultData)=0;
private:
	CGMMessageBase(const CGMMessageBase&);
	CGMMessageBase &operator=(const CGMMessageBase&);
};

///
/// GM���ܵ��ýӿڣ�CGameLogonManage��CGameMainManage����ʵ�ֱ��ӿ��еĺ���
/// GM��Ϣ�����ദ������Ϣ����Ҫ��ɹ��ܵľ���ʵ�֣�
/// ����ʵ����ԭ��CGameLogonManage��CGameMainManage��ɣ�
/// ����������Ҫ�ӱ��ӿ����������ӿ�ָ�����ṩ��GM��Ϣ������
///
interface IGMFunction
{
public:
	/// ��ͻ��˷�������
	virtual int SendData(UINT uIndex, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID) = 0;
	/// ��ͻ��˷�������
	virtual int SendData(UINT uIndex, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID) = 0;
	/// ��������ҷ�������
	virtual int SendDataByUser(UINT uUserID, BYTE bMainID, BYTE bAssistantID, UINT bHandleCode, DWORD dwHandleID) = 0;
	/// ��������ҷ�������
	virtual int SendDataByUser(UINT uUserID, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, UINT bHandleCode, DWORD dwHandleID) = 0;
	/// ��ͻ��˷��͹㲥����
	virtual int SendBatchData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode) = 0;
	/// �����û�ID��ȡSocket���
	virtual int GetIndexByID(int iUserID)=0;
	/// �����ݿ�ģ�鷢�ͷ�������
	virtual bool PushDataLine(DataBaseLineHead * pData, UINT uSize, UINT uHandleKind, UINT uIndex,DWORD dwHandleID) = 0;
	/// �ж�ĳ�����Ƿ񻹴���
	virtual bool IsConnectID(UINT uIndex, DWORD dwHandleID) = 0;
	/// �ж�ĳ����Ƿ��Ѿ���¼
	virtual bool IsUserAlreadyLogon(DWORD dwUserID) = 0;
	/// �����ҵ�¼��¼
	virtual bool AddLogonUser(DWORD dwUserID, UINT uIndex, bool bIsGM = false) = 0;
};


/// ����GM��Ϣ�������
extern CGMMessageBase *CreateGMModule(IGMFunction *);
/// ����GM��Ϣ�������
extern void DestroyGMModule(CGMMessageBase *);

#endif//__INTERFACE_FOR_GM_H