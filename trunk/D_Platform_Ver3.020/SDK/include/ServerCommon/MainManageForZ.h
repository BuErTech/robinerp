#pragma once

#ifndef MAINMANAGEFORZ_H_FILE
#define MAINMANAGEFORZ_H_FILE

#include "AFCInterface.h"
#include "../Common/ComStruct.h"
#include "DataLine.h"
#include "AFCSocket.h"
#include "AFCDataBase.h"
#include "TalkMessageFilter.h"

//�������ݹ�����
class AFX_EXT_CLASS CBaseMainManageForZ : public IServerSocketServiceForZ , public IDataBaseResultService
{
	//�ؼ�����
protected:
	bool									m_bRun;						//���б�־
	bool									m_bInit;					//��ʼ����־
	HWND									m_hWindow;					//���ھ��
	HANDLE									m_hWindowThread;			//�����߳�
	HANDLE									m_hHandleThread;			//�����߳�
	HANDLE									m_hCompletePort;			//������ɶ˿�
	CDataLine								m_DataLine;					//���ݶ���

public:
	ManageInfoStruct						m_InitData;					//��ʼ������
	KernelInfoStruct						m_KernelData;				//�ں�����
	ServerDllInfoStruct						m_DllInfo;					//DLL ��Ϣ
	CTCPSocketManageForZ					m_TCPSocket;				//����ģ��
	CAFCDataBaseManage						m_SQLDataManage;			//���ݿ�ģ��


	//��ӹ������
	CTalkMessageFilter                     m_TalkFilter;     //���Թ���ϵͳ   yjj 090317


	//��������
public:
	//���캯��
	CBaseMainManageForZ(void);
	//��������
	virtual ~CBaseMainManageForZ(void);

	//����ʹ�ù��ܺ��� ���������̵߳��ã�
public:	
	//�趨��ʱ��
	bool SetTimer(UINT uTimerID, UINT uElapse);
	//�����ʱ��
	bool KillTimer(UINT uTimerID);

	//����ӿں��� �����̵߳��ã�
public:
	//��ʼ������
	virtual bool Init(ManageInfoStruct * pInitData, IDataBaseHandleService * pDataHandleService);
	//ȡ����ʼ������
	virtual bool UnInit();
	//��������
	virtual bool Start();
	//ֹͣ����
	virtual bool Stop();

	//����ӿں��� �����̵߳��ã�
private:
	//��ȡ��Ϣ���� ���������أ�
	virtual bool PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)=0;
	//���ݹ���ģ���ʼ��
	virtual bool OnInit(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData) { return true; };
	//���ݹ���ģ��ж��
	virtual bool OnUnInit() { return true; };
	//���ݹ���ģ������
	virtual bool OnStart() { return true; };
	//���ݹ���ģ��ر�
	virtual bool OnStop() { return true; };

	//������չ�ӿں��� ���������̵߳��ã�
private:
	//SOCKET ���ݶ�ȡ ���������أ�
	virtual bool OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID)=0;
	//SOCKET �ر� ���������أ�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, long int lConnectTime)=0;
	//���ݿ⴦���� ���������أ�
	virtual bool OnDataBaseResult(DataBaseResultLine * pResultData)=0;
	//��ʱ����Ϣ ���������أ�
	virtual bool OnTimerMessage(UINT uTimerID)=0;

	//����ӿں��� �����������̵߳��ã�
public:
	//����رմ��� ������Ҫ���أ�
	virtual bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, long int lConnectTime);
	//������Ϣ���� ������Ҫ���أ�ΪZ������׼��
	virtual bool OnSocketReadEvent(CTCPSocketForZ * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID);
	//���ݿ������� ������Ҫ���أ�
	virtual bool OnDataBaseResultEvent(DataBaseResultLine * pResultData, UINT uHandleKind, UINT uHandleResult, UINT uResultSize, UINT uIndex, DWORD dwHandleID);

	//�ڲ����������̵߳��ã�
private:
	//�ں���������
	bool AFCKernelStart();
	//��ʱ��֪ͨ��Ϣ
	bool WindowTimerMessage(UINT uTimerID);
	//�������ݴ����߳�
	static unsigned __stdcall LineDataHandleThread(LPVOID pThreadData);
	//WINDOW ��Ϣѭ���߳�
	static unsigned __stdcall WindowMsgThread(LPVOID pThreadData);
	//���ڻص�����
	static LRESULT CALLBACK WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif