
#include "stdafx.h" 
#include "Socket32.h"
#include <assert.h>
#include <Tlhelp32.h>// DWORD GetProcessIdFromName(LPCTSTR name) 2012.08.16 yyf

// �����ܵ��߳� ��ͣ�Ķ���Ϣ.
DWORD WINAPI  SimpleSocketThread(LPVOID param)
{
	CSocket32 * SimpleSocket = (CSocket32 *)param;

	BYTE BYTE_Read[10240] ;
	while(NULL != SimpleSocket && SimpleSocket->bSimpleSocketThread)
	{  
		int nRead; 
		memset(&BYTE_Read,0,10240);
		nRead = SimpleSocket->Receive((void*)BYTE_Read,10240);
		if( nRead >0 )
		{ 
			SimpleSocket->ReadSimpleSocket((LPVOID)BYTE_Read,nRead);
		} 
		else
		{ 
			OutputDebugString("yyf: SimpleSocket->Receive(..) ���󣬿��ܶ���.");
			
			Sleep(500);

		} 
	} 
	return 0;
}
/**///////////////////////////////////////////////////////////////////////
// Construction/Destruction
/**///////////////////////////////////////////////////////////////////////
//����
CSocket32::CSocket32()
{
//#ifdef _WIN32
	WSAData wsaData;
	int err =WSAStartup(0x0202,&wsaData);
	if ( err != 0 )                           
		return;
	if ( LOBYTE( wsaData.wVersion ) != 2 ||HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup( );
		return; 
	}
//#endif
	m_hSocket = INVALID_SOCKET;

	bSimpleSocketThread = false; 
}
//����
CSocket32::~CSocket32()
{
	Close();
}
/**//**********************************************************************/
//1.�����׽���
BOOL CSocket32::Create( UINT nSocketPort, int nSocketType ,LPCTSTR lpszSocketAddress,bool bisServer )
{
	assert(m_hSocket == INVALID_SOCKET);//�׽��ֱ������ȳ�ʼ��Ϊ��
	m_hSocket = socket(AF_INET,nSocketType,IPPROTO_IP);//Ȼ���ٴ���
	if(    m_hSocket == INVALID_SOCKET)
		return FALSE;

	sockaddr_in addr = {AF_INET,htons(nSocketPort)};
	if (!lpszSocketAddress) //����û���ָ������ϵͳ����
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(lpszSocketAddress);

	if(!bisServer)
	{
        return TRUE;
	}
	if (!bind(m_hSocket,(sockaddr*)&addr,sizeof(addr)))
		return TRUE;//If no error occurs, bind returns zero
	Close();

	return FALSE;
}

//2.���ͣ����UDP��
int CSocket32::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszHostAddress , int nFlags)
{
	sockaddr_in addr = {AF_INET,htons(nHostPort),};
	assert(lpszHostAddress);//UDP ����ָ��Ŀ�ĵ�ַ
	addr.sin_addr.s_addr = inet_addr(lpszHostAddress);
	return sendto(m_hSocket,(char*)lpBuf,nBufLen,nFlags,(sockaddr*)&addr,sizeof(addr));
}

//3.���գ����UDP��
int CSocket32::ReceiveFrom( void* lpBuf, int nBufLen, char *rSocketAddress, UINT& rSocketPort, int nFlags )
{
	sockaddr_in from;//������һ����ʱ�����������ڴ棬������Ϣ��
	socklen_t fromlen = sizeof(from);//Ȼ����ܼ����ڴ泤��

	int nRet = recvfrom(m_hSocket,(LPSTR)lpBuf,nBufLen,nFlags,(sockaddr*)&from,&fromlen);
	if(nRet <= 0)
		return nRet;
	if(rSocketAddress)
	{
		strcpy(rSocketAddress,inet_ntoa(from.sin_addr));//out
		rSocketPort = htons(from.sin_port);//out
	}

	return nRet;
}

//4.��������(���TCP)
BOOL CSocket32::Accept( CSocket32& rConnectedSocket, LPSTR lpSockAddr ,UINT *nPort )
{
	sockaddr_in addr = {AF_INET};
	socklen_t nLen = sizeof(addr);
	rConnectedSocket.m_hSocket = accept(m_hSocket,(sockaddr*)&addr,&nLen);
	if(rConnectedSocket.m_hSocket == INVALID_SOCKET)
		return FALSE;
	if(lpSockAddr)
	{
		strcpy(lpSockAddr,inet_ntoa(addr.sin_addr));
		*nPort = htons(addr.sin_port);
	}
	return TRUE;    
}

//5.��������(���TCP)
BOOL CSocket32::Connect( LPCTSTR lpszHostAddress, UINT nHostPort )
{
	sockaddr_in addr = {AF_INET,htons(nHostPort)};
	addr.sin_addr.s_addr = inet_addr(lpszHostAddress);
	if (addr.sin_addr.s_addr == INADDR_NONE)//�������޹㲥��ַ�����ñ�����ַ֮һ
	{
		hostent * lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
			addr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			//WSAGetLastError();
			//    WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}
	return !connect(m_hSocket,(sockaddr*)&addr,sizeof(addr));
}

//6.�õ������׽��֣�IP��Port
BOOL CSocket32::GetSockName( char*  rSocketAddress, UINT& rSocketPort )
{
	sockaddr_in addr;
	socklen_t nLen = sizeof(addr);

	if(SOCKET_ERROR == getsockname(m_hSocket, (sockaddr*)&addr, &nLen))
		return FALSE;
	if(rSocketAddress)
	{
		strcpy(rSocketAddress,inet_ntoa(addr.sin_addr));
		rSocketPort = htons(addr.sin_port);
	}
	return TRUE;
}

//7.�õ��Է��׽������֣�IP��Port
BOOL CSocket32::GetPeerName(  char* rPeerAddress, UINT& rPeerPort )
{
	sockaddr_in addr;
	socklen_t nLen = sizeof(addr);

	if(SOCKET_ERROR == getpeername(m_hSocket, (sockaddr*)&addr, &nLen))
		return FALSE;
	if(rPeerAddress)
	{
		strcpy(rPeerAddress,inet_ntoa(addr.sin_addr));
		rPeerPort = htons(addr.sin_port);
	}
	return TRUE;
}
void CSocket32::ReadSimpleSocket(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead)
{ 
	if(NULL != pSimpleSocket )
	{//���ûص�
		pSimpleSocket->ReadSimpleSocket(lpBuffer, dwNumberOfBytesToRead);
	}  
}


void CSocket32::SetSimpleSocket(ISimpleSocket * pSimpleSocket)//��ʼ�����ص��Ľӿ�ָ��  2012.08.17 yyf
{
	this->pSimpleSocket = pSimpleSocket;
}
// //��һ������ 2012.08.17 yyf
bool CSocket32:: OpenProcess(LPCSTR lpApplicationName)
{
	strProcessName = lpApplicationName;//��������
	if(0 == GetProcessIdFromName(strProcessName) && NULL != pSimpleSocket && !bSimpleSocketThread)
	{//����û��ʼ

		bSimpleSocketThread = false; 
		STARTUPINFO sui;
		//PROCESS_INFORMATION pi;
		ZeroMemory(&sui,sizeof(STARTUPINFO));
		sui.cb=sizeof(STARTUPINFO);
		sui.dwFlags=STARTF_USESTDHANDLES; 
		sui.hStdError=GetStdHandle(STD_ERROR_HANDLE);

		if(!CreateProcess(strProcessName,NULL,NULL,NULL,
			TRUE,0,NULL,NULL,&sui,&pi))
		{
		 
			//MessageBox("�����ӽ���ʧ�ܣ�");
			return false;
		}
	    //����������
		if (Create(6000))
		{
			if (Listen())
			{   
				SOCKADDR_IN addrClient;
				int len = sizeof(SOCKADDR);
				//�ȴ��ͻ�������
				m_hSocketServer = m_hSocket;
				m_hSocket = accept(m_hSocketServer, (SOCKADDR*)&addrClient, &len);//sockConn�������ӵ��׽���
			} 
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		//���������ܵ��̣߳���ʼ��
		bSimpleSocketThread = true; 
		DWORD dwThreadID = 0;
		HANDLE hThread = CreateThread(0,0,SimpleSocketThread,this,0,&dwThreadID);
		if (hThread)
		{
			OutputDebugString("yyf: �����ܵ��߳̿�ʼ.");
			::CloseHandle(hThread);
		} 

		return true;
	}
	return true;
}
// //���ÿͻ���  2012.08.17 yyf
int CSocket32:: SetClientSimpleSocket(void)
{
 
	//�����׽���
	m_hSocket = socket( AF_INET,SOCK_STREAM, 0 );
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	//�������������������
	connect( m_hSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	return 0;
}
// ��ʼ�߳�
void CSocket32:: StartThread(void)
{
	bSimpleSocketThread = true;//�����߳̿�����
 
	DWORD dwThreadID = 0;
	HANDLE hThread = CreateThread(0,0,SimpleSocketThread,this,0,&dwThreadID);
	if (hThread)
	{ 
		::CloseHandle(hThread);
	}
	else
	{
		OutputDebugString("yyf: update.exe  �߳�ʧ��.");
	}
}
//2012.08.16 yyf ���ؽ��̵�PID
DWORD CSocket32::GetProcessIdFromName(LPCTSTR name) //��������У����ؽ��̵�PID��û���з���0 //�Լ������޸ĺ��������������еĽ��̵ĸ���
{ 
	PROCESSENTRY32 pe; 
	DWORD id = 0; 
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); 
	pe.dwSize = sizeof(PROCESSENTRY32); 
	if( !Process32First(hSnapshot,&pe) ) 
	{
		return 0; 
	}

	while(1) 
	{ 
		pe.dwSize = sizeof(PROCESSENTRY32); 
		if( Process32Next(hSnapshot,&pe)==FALSE ) 
			break; 
		//if(strcmp(pe.szExeFile,name) == 0) 
		//{ 
		//	id = pe.th32ProcessID; 
		//	break; 
		//} 
		if(_stricmp(pe.szExeFile,name) == 0) //���Դ�Сд��ĸ�Ƚ�
		{ 
			id = pe.th32ProcessID; 
			break; 
		} 

	}
	CloseHandle(hSnapshot); 
	return id; 
} 
//��ʼ���ͻ���.
bool CSocket32::InitClientSimpleSocket(ISimpleSocket * pSimpleSocket)
{
	SetSimpleSocket(pSimpleSocket);//��ʼ��   
	if (0 == SetClientSimpleSocket())
	{
		StartThread();
		return true;
	}
	
	return false;
}
