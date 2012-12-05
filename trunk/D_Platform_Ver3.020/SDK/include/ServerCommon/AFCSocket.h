#ifndef AFCSOCLET_H_FILE_2003_07
#define AFCSOCLET_H_FILE_2003_07

#include <string>
using namespace std;

#include <AfxSock.h>
#include "AFCLock.h"
#include "AFCPtrArray.h"
#include "AFCInterface.h"
#include "../common/BaseMessage.h"



/**************************************************************************************************************/

// UDP SOCKET ���粿��

/**************************************************************************************************************/

//UDP ���� SOCKET ��
class AFX_EXT_CLASS CUDPSocket
{
	//��������
protected:
	SOCKET						m_hSocket;							//SOCKET ���

	//��������
public:
	//���캯��
	CUDPSocket(void);
	//��������
	virtual ~CUDPSocket(void);
	//���ͺ���
	inline int	SendData(void * pData, UINT uSize, TCHAR szSendIP[]);
	//���պ���
	inline int RecvData();
};

/**************************************************************************************************************/

// TCP SOCKET ���粿�� 

/**************************************************************************************************************/

//��˵��
class CTCPSocket;
class CTCPSocketManage;

//���涨��
#define SED_SIZE				150000							//��������С
#define RCV_SIZE				100000							//��������С
//���涨������Z����������W��10��֮1
#define SED_SIZE_Z				50000							//��������С
#define RCV_SIZE_Z				50000							//��������С

//��������
const UINT uBatchLineSize=180;

/**************************************************************************************************************/

#define NO_CONNECT				0								//û������
#define CONNECTING				1								//��������
#define CONNECTED				2								//�ɹ�����

//�ͻ��� SOKET
class AFX_EXT_CLASS CTCPClientSocket : public CWnd
{
	DECLARE_MESSAGE_MAP()

	//��������
private:
	int							m_iReadBufLen;					//����������
	BYTE						m_bConnectState;				//����״̬
	SOCKET						m_hSocket;						//SOCKET ���
	IClientSocketService		* m_pIService;					//����ӿ�
	BYTE						m_szBuffer[RCV_SIZE];			//���ݻ�����

	int									m_iCheckCode;

	//��������
public:
	//���캯��
	explicit CTCPClientSocket(IClientSocketService * pIService);
	//��������
	virtual ~CTCPClientSocket();
	//��ʼ�� SOCKET ����
	inline bool InitSocketWnd();
	//�Ƿ��Ѿ�����
	inline BYTE GetSocketState() { return m_bConnectState; }
	//�ر� SOCKET
	inline bool CloseSocket(bool bNotify=true);
	//���ӷ�����
	inline bool Connect(const TCHAR * szServerIP, UINT uPort);
	//���ӷ�����
	inline bool Connect(long int dwServerIP, UINT uPort);
	//���ͺ���
	inline int SendData(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//������ͺ���
	inline int SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	//����У���룬��������Կ
	void SetCheckCode(__int64 iCheckCode, int isecretkey);
	//�ڲ�����
private:
	//�ر� SOCKET ����
	inline bool CloseSocketWnd();
	//SOCKET ��Ϣ�������
	inline LRESULT	OnSocketNotifyMesage(WPARAM wParam, LPARAM lParam);

};

//������ SOCKET
class AFX_EXT_CLASS CListenSocket
{
	//��������
protected:
	SOCKET						m_hSocket;							//SOCKET ���

	//��������
public:
	//���캯��
	CListenSocket(void);
	//��������
	virtual ~CListenSocket(void);

	//���ܺ���
public:
	//���� SOCKET
	inline bool Create();
	//��������
	inline bool Listen();
	//�󶨵�ַ
	inline bool Bind(UINT uPort);
	//�ر� SOCKET
	inline bool CloseSocket();
	//��ȡ SOCKET ���
	inline SOCKET GetSafeSocket() { return m_hSocket; }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// Modified by zxd 20090804
// Ϊ��֧��Z�������������ͻ��1W�����С����������ͬʱ����Ӱ��W��������
// ����½�һ��ƽ�е�Socket�๩Zʹ�ã�����ԭ���๩Wʹ��
// ������ԭ�е��๩Wʹ��

//TCP SOCKET ��
class AFX_EXT_CLASS CTCPSocket
{
	friend class CTCPSocketManage;

	//�ṹ����
	struct OverLappedStruct		//SOCKET �ص� IO �ṹ
	{
		//��������
		OVERLAPPED				OverLapped;							//�ص��ṹ
		WSABUF					WSABuffer;							//���ݻ���
		UINT					uOperationType;						//��������
		DWORD					dwHandleID;							//���� ID ����
	};

	//��������
protected:
	UINT						m_uIndex;							//SOCKET ����
	DWORD						m_dwHandleID;						//�����ʶ
	char						m_szSendBuf[SED_SIZE];				//�������ݻ�����
	char						m_szRecvBuf[RCV_SIZE];				//���ݽ��ջ�����
	long int					m_lBeginTime;						//����ʱ��
	SOCKET						m_hSocket;							//SOCKET ���
	CTCPSocketManage			* m_pManage;						//SOCKET ������ָ��
	struct sockaddr_in			m_SocketAddr;						//��ַ��Ϣ

	//�ڲ�����
private:
	bool						m_bSending;							//�Ƿ�����
	DWORD						m_dwSendBuffLen;					//���ͻ���������
	DWORD						m_dwRecvBuffLen;					//���ջ���������
	CAFCSignedLock				m_csSendLock;						//��ȡͬ����
	CAFCSignedLock				m_csRecvLock;						//����ͬ����
	OverLappedStruct			m_SendOverData;						//���������ص��ṹ
	OverLappedStruct			m_RecvOverData;						//���������ص��ṹ

	//��������
public:
	//���캯��
	CTCPSocket(void);
	//��������
	virtual ~CTCPSocket(void);

	//���ܺ���
public:
	//��ȡ�����־
	inline DWORD GetHandleID() { return m_dwHandleID; }
	//�ϲ� SOCKET
	inline bool Attach(SOCKET hSocket, sockaddr_in & Addr, DWORD dwHandleID);
	//�������ݺ���
    inline int SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ݺ���
	inline int SendData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);

	//���ܺ���
public:
	//�ж��Ƿ�����
	inline bool IsConnect() { return (m_hSocket!=INVALID_SOCKET); }
	//��ȡ����ʱ��
	inline long int GetBeginTime() { return m_lBeginTime; };
	//��ȡ������Ϣ
	inline ULONG GetConnectData() { return m_SocketAddr.sin_addr.S_un.S_addr; }
	//��ȡ������
	 inline  CAFCSignedLock * GetRecvLock() { return &m_csRecvLock; }
	//��ȡ��������
	 inline  CAFCSignedLock * GetSendLock() { return &m_csSendLock; }
	//�ж��Ƿ�����
	inline bool IsConnectID(DWORD dwHandleID);

	//��������
protected:
	//���� ID ��������
	inline void SetIndex(UINT uIndex) { m_uIndex=uIndex; };
	//���ù�����ָ��
	inline void SetSocketManagePtr(CTCPSocketManage * pManage) { m_pManage=pManage; };
	//������ɺ���
	inline bool OnSendCompleted(DWORD dwThancferred, DWORD dwHandleID);
	//������ɺ���
	inline bool OnRecvCompleted(DWORD dwHandleID);
	//��ⷢ�����ݺ���
	inline bool CheckSendData();
	//Ͷ�ݽ��� IO
	inline bool RecvData();
	//�ر� SOCKET
	inline bool CloseSocket();
};

/**************************************************************************************************************/

//TCP ���� SOCKET ������
class AFX_EXT_CLASS CTCPSocketManage
{
	///int m_CurConnectCount;
	//�ṹ����
	struct BatchSendLineStruct	//���������̶߳��нṹ
	{
		//��������
		NetMessageHead			NetHead;						//���ݰ�ͷ
		char					szSendData[MAX_SEND_SIZE];		//���ͻ���
	};

	struct ThreadStartStruct	//�߳����������ṹ
	{
		//��������
		HANDLE					hCompletionPort;				//��ɶ˿�
		HANDLE					hThreadEvent;					//�߳��¼�
		CTCPSocketManage		* pSocketManage;				//SOCKET ����ָ��
	};

	//�ں˱���
protected:
	bool						m_bInit;						//��ʼ����־
	volatile bool				m_bRun;							//������־
	CListenSocket				m_ListenSocket;					//���� SOCKET
	BYTE						* m_pKeep;						//������������
	IServerSocketService		* m_pService;					//����ӿ�
	CAFCPtrArray<CTCPSocket>	m_SocketPtrArray;				//TCP SOCKET ����

	//���ò���
protected:
	bool						m_bCheckConnect;				//�Ƿ������
	BYTE						m_bMaxVer;						//���°汾
	BYTE						m_bLessVer;						//��Ͱ汾
	UINT						m_uCheckTime;					//�����߼��
	UINT						m_uListenPort;					//�����Ͽ�

public:
	UINT						m_iSecretKey;                   //����У�������Կ

	//��������
protected:
	UINT						m_uBatchCerrenPos;				//�������͵�
	UINT						m_uBatchLineCount;				//���Ͷ�����Ŀ
	CAFCSignedLock				m_csBacthSend;					//����ͬ����
	BatchSendLineStruct			m_SendLine[uBatchLineSize];		//�������Ͷ���

	//��ɶ˿ڱ���
private:
	UINT						m_uRSThreadCount;				//��д�߳���Ŀ
	HANDLE						m_hEventThread;					//���������¼�
	HANDLE						m_hCompletionPortBatch;			//������ɶ˿�
	HANDLE						m_hCompletionPortRS;			//��д��ɶ˿�
	HANDLE						m_hThreadKeep;					//SOCKET ά���߳�
	HANDLE						m_hThreadAccept;				//Ӧ���߳�

	//��������
public:
	//���캯��
	CTCPSocketManage();
	CTCPSocketManage(CTCPSocketManage&);
	CTCPSocketManage& operator=(CTCPSocketManage&); 
	//��������
	virtual ~CTCPSocketManage();

	//������
public:
	//��ʼ��
	virtual bool Init(UINT uMaxCount, UINT uListenPort, BYTE bMaxVer, BYTE bLessVer, int iSecretKey, IServerSocketService * pService);
	//ȡ����ʼ��
	virtual bool UnInit();
	//��ʼ����
	virtual bool Start(UINT uAcceptThreadCount, UINT uSocketThradCount);
	//ֹͣ����
	virtual bool Stop();
	//��Ϣ������
	virtual bool OnNetMessage(CTCPSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID);

	//���ܺ���
public:
	//�������ݺ���
    inline int SendData(UINT uIndex, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ݺ���
    inline int SendData(UINT uIndex, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ݺ���
    inline int SendData(CTCPSocket * pSocket, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ͺ���
	inline int SendDataBatch(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//�ر�����
	inline bool CloseSocket(UINT uIndex, DWORD dwHandleID);
	//�ر�����
	inline bool CloseSocket(CTCPSocket * pSocket, DWORD dwHandleID);
	//�ж��Ƿ�����
	inline bool IsConnectID(UINT uIndex, DWORD dwHandleID);
	//��ֵ�� IP �ַ�
	static TCHAR * ULongToIP(ULONG dwIP, TCHAR * szIP);
	//��ȡ���� IP ��ַ
	inline static bool GetLoaclIPAddr(TCHAR szAddrBuffer[], UINT uStation);
	//��ȡָ��index��������Ϣ
	string GetServerInfo(UINT uIndex);

	//�ں˺���
private:
	//���ɿ��� SOCKET
	inline CTCPSocket * TCPSocketNew();
	//�ͷŻ SOCKET
	inline bool TCPSocketDelete(CTCPSocket * pSocket);
	//SOCKET ����Ӧ���߳�
	inline static unsigned __stdcall ThreadAccept(LPVOID pThreadData);
	//SOCKET ���ݽ����߳�
	inline static unsigned __stdcall ThreadRSSocket(LPVOID pThreadData);
	//SOCKET �������������߳�
	inline static unsigned __stdcall ThreadBatchSend(LPVOID pThreadData);
	//SOCKET ά���߳�
	inline static unsigned __stdcall ThreadKeepActive(LPVOID pThreadData);
	//��ȡʱ�亯��
	inline static unsigned __int64 AFCGetTimer();
	//��ȡ���� ID ����
	inline static DWORD GetHandleID();
};

// Modified by zxd 20090804
// Ϊ��֧��Z�������������ͻ��1W�����С����������ͬʱ����Ӱ��W��������
// ����½�һ��ƽ�е�Socket�๩Zʹ�ã�����ԭ���๩Wʹ��
/////////////////////////////////////////////////////////////////////////////////////////////
// ������ԭ�е��๩Wʹ��
//---------------------------------------------------------------------------------------------------
// �������½����๩Zʹ��

/////////////////////////////////////////////////////////////////////////////////////////////
// Modified by zxd 20090804
// Ϊ��֧��Z�������������ͻ��1W�����С����������ͬʱ����Ӱ��W��������
// ����½�һ��ƽ�е�Socket�๩Zʹ�ã�����ԭ���๩Wʹ��

//TCP SOCKET ��
class AFX_EXT_CLASS CTCPSocketForZ
{
	friend class CTCPSocketManageForZ;

	//�ṹ����
	struct OverLappedStruct		//SOCKET �ص� IO �ṹ
	{
		//��������
		OVERLAPPED				OverLapped;							//�ص��ṹ
		WSABUF					WSABuffer;							//���ݻ���
		UINT					uOperationType;						//��������
		DWORD					dwHandleID;							//���� ID ����
	};

	//��������
protected:
	UINT						m_uIndex;							//SOCKET ����
	DWORD						m_dwHandleID;						//�����ʶ
	char						m_szSendBuf[SED_SIZE_Z];			//�������ݻ�����
	char						m_szRecvBuf[RCV_SIZE_Z];			//���ݽ��ջ�����
	long int					m_lBeginTime;						//����ʱ��
	SOCKET						m_hSocket;							//SOCKET ���
	CTCPSocketManageForZ		* m_pManage;						//SOCKET ������ָ��
	struct sockaddr_in			m_SocketAddr;						//��ַ��Ϣ

	//�ڲ�����
private:
	bool						m_bSending;							//�Ƿ�����
	DWORD						m_dwSendBuffLen;					//���ͻ���������
	DWORD						m_dwRecvBuffLen;					//���ջ���������
	CAFCSignedLock				m_csSendLock;						//��ȡͬ����
	CAFCSignedLock				m_csRecvLock;						//����ͬ����
	OverLappedStruct			m_SendOverData;						//���������ص��ṹ
	OverLappedStruct			m_RecvOverData;						//���������ص��ṹ

	//��������
public:
	//���캯��
	CTCPSocketForZ(void);
	//��������
	virtual ~CTCPSocketForZ(void);

	//���ܺ���
public:
	//��ȡ�����־
	inline DWORD GetHandleID() { return m_dwHandleID; }
	//�ϲ� SOCKET
	inline bool Attach(SOCKET hSocket, sockaddr_in & Addr, DWORD dwHandleID);
	//�������ݺ���
	inline int SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ݺ���
	inline int SendData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);

	//���ܺ���
public:
	//�ж��Ƿ�����
	inline bool IsConnect() { return (m_hSocket!=INVALID_SOCKET); }
	//��ȡ����ʱ��
	inline long int GetBeginTime() { return m_lBeginTime; };
	//��ȡ������Ϣ
	inline ULONG GetConnectData() { return m_SocketAddr.sin_addr.S_un.S_addr; }
	//��ȡ������
	inline  CAFCSignedLock * GetRecvLock() { return &m_csRecvLock; }
	//��ȡ��������
	inline  CAFCSignedLock * GetSendLock() { return &m_csSendLock; }
	//�ж��Ƿ�����
	inline bool IsConnectID(DWORD dwHandleID);

	//��������
protected:
	//���� ID ��������
	inline void SetIndex(UINT uIndex) { m_uIndex=uIndex; };
	//���ù�����ָ��
	inline void SetSocketManagePtr(CTCPSocketManageForZ * pManage) { m_pManage=pManage; };
	//������ɺ���
	inline bool OnSendCompleted(DWORD dwThancferred, DWORD dwHandleID);
	//������ɺ���
	inline bool OnRecvCompleted(DWORD dwHandleID);
	//��ⷢ�����ݺ���
	inline bool CheckSendData();
	//Ͷ�ݽ��� IO
	inline bool RecvData();
	//�ر� SOCKET
	inline bool CloseSocket();
};

/**************************************************************************************************************/

//TCP ���� SOCKET ������
class AFX_EXT_CLASS CTCPSocketManageForZ
{
	///int m_CurConnectCount;
	//�ṹ����
	struct BatchSendLineStruct	//���������̶߳��нṹ
	{
		//��������
		NetMessageHead			NetHead;						//���ݰ�ͷ
		char					szSendData[MAX_SEND_SIZE];		//���ͻ���
	};

	struct ThreadStartStruct	//�߳����������ṹ
	{
		//��������
		HANDLE					hCompletionPort;				//��ɶ˿�
		HANDLE					hThreadEvent;					//�߳��¼�
		CTCPSocketManageForZ	* pSocketManage;				//SOCKET ����ָ��
	};

	//�ں˱���
protected:
	bool						m_bInit;						//��ʼ����־
	volatile bool				m_bRun;							//������־
	CListenSocket				m_ListenSocket;					//���� SOCKET
	BYTE						* m_pKeep;						//������������
	IServerSocketServiceForZ	* m_pService;					//����ӿ�
	CAFCPtrArray<CTCPSocketForZ>	m_SocketPtrArray;				//TCP SOCKET ����

	//���ò���
protected:
	bool						m_bCheckConnect;				//�Ƿ������
	BYTE						m_bMaxVer;						//���°汾
	BYTE						m_bLessVer;						//��Ͱ汾
	UINT						m_uCheckTime;					//�����߼��
	UINT						m_uListenPort;					//�����Ͽ�
public:
	UINT						m_iSecretKey;					//����У�������Կ

	//��������
protected:
	UINT						m_uBatchCerrenPos;				//�������͵�
	UINT						m_uBatchLineCount;				//���Ͷ�����Ŀ
	CAFCSignedLock				m_csBacthSend;					//����ͬ����
	BatchSendLineStruct			m_SendLine[uBatchLineSize];		//�������Ͷ���

	//��ɶ˿ڱ���
private:
	UINT						m_uRSThreadCount;				//��д�߳���Ŀ
	HANDLE						m_hEventThread;					//���������¼�
	HANDLE						m_hCompletionPortBatch;			//������ɶ˿�
	HANDLE						m_hCompletionPortRS;			//��д��ɶ˿�
	HANDLE						m_hThreadKeep;					//SOCKET ά���߳�
	HANDLE						m_hThreadAccept;				//Ӧ���߳�

	//��������
public:
	//���캯��
	CTCPSocketManageForZ();
	CTCPSocketManageForZ(CTCPSocketManageForZ&);
	CTCPSocketManageForZ& operator=(CTCPSocketManageForZ&); 
	//��������
	virtual ~CTCPSocketManageForZ();

	//������
public:
	//��ʼ��
	virtual bool Init(UINT uMaxCount, UINT uListenPort, BYTE bMaxVer, BYTE bLessVer, int iSecretKey, IServerSocketServiceForZ * pService);
	//ȡ����ʼ��
	virtual bool UnInit();
	//��ʼ����
	virtual bool Start(UINT uAcceptThreadCount, UINT uSocketThradCount);
	//ֹͣ����
	virtual bool Stop();
	//��Ϣ������
	virtual bool OnNetMessage(CTCPSocketForZ * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uIndex, DWORD dwHandleID);

	//���ܺ���
public:
	//�������ݺ���
	inline int SendData(UINT uIndex, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ݺ���
	inline int SendData(UINT uIndex, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ݺ���
	inline int SendData(CTCPSocketForZ * pSocket, void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode, DWORD dwHandleID);
	//�������ͺ���
	inline int SendDataBatch(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//�ر�����
	inline bool CloseSocket(UINT uIndex, DWORD dwHandleID);
	//�ر�����
	inline bool CloseSocket(CTCPSocketForZ * pSocket, DWORD dwHandleID);
	//�ж��Ƿ�����
	inline bool IsConnectID(UINT uIndex, DWORD dwHandleID);
	//��ֵ�� IP �ַ�
	static TCHAR * ULongToIP(ULONG dwIP, TCHAR * szIP);
	//��ȡ���� IP ��ַ
	inline static bool GetLoaclIPAddr(TCHAR szAddrBuffer[], UINT uStation);

	//�ں˺���
private:
	//���ɿ��� SOCKET
	inline CTCPSocketForZ * TCPSocketNew();
	//�ͷŻ SOCKET
	inline bool TCPSocketDelete(CTCPSocketForZ * pSocket);
	//SOCKET ����Ӧ���߳�
	inline static unsigned __stdcall ThreadAccept(LPVOID pThreadData);
	//SOCKET ���ݽ����߳�
	inline static unsigned __stdcall ThreadRSSocket(LPVOID pThreadData);
	//SOCKET �������������߳�
	inline static unsigned __stdcall ThreadBatchSend(LPVOID pThreadData);
	//SOCKET ά���߳�
	inline static unsigned __stdcall ThreadKeepActive(LPVOID pThreadData);
	//��ȡʱ�亯��
	inline static unsigned __int64 AFCGetTimer();
	//��ȡ���� ID ����
	inline static DWORD GetHandleID();
};

// Modified by zxd 20090804
// Ϊ��֧��Z�������������ͻ��1W�����С����������ͬʱ����Ӱ��W��������
// ����½�һ��ƽ�е�Socket�๩Zʹ�ã�����ԭ���๩Wʹ��
// �������½����๩Zʹ��
/////////////////////////////////////////////////////////////////////////////////////////////


/**************************************************************************************************************/

#endif