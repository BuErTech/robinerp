/*
Module : APIPE.CPP
Purpose: Defines the implementation for an MFC wrapper class
         for Win32 anonymous Pipes
*/


/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "apipe.h"
#include <Tlhelp32.h>// DWORD GetProcessIdFromName(LPCTSTR name) 2012.08.16 yyf
 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// �����ܵ��߳� ��ͣ�Ķ���Ϣ.
DWORD WINAPI  AnonymousPipeThread(LPVOID param)
{
	CAnonymousPipe * pAnPipe = (CAnonymousPipe *)param;

	BYTE BYTE_Read[10240] ;
	while(NULL != pAnPipe && pAnPipe->bAnonymousPipeThread)
	{
		DWORD dwRead;

		DWORD  dwBytesRead;
		DWORD  dwTotalBytesAvail;
		DWORD dwBytesLeftThisMessage; 

		if (pAnPipe->Peek((LPVOID)BYTE_Read,10240,  dwBytesRead, dwTotalBytesAvail, dwBytesLeftThisMessage))
		{
			if(pAnPipe->Read((LPVOID)BYTE_Read,10240,dwRead))
			{
				pAnPipe->ReadAnonymousPipe((LPVOID)BYTE_Read,dwRead);
			} 
			else
			{
				OutputDebugString("yyf: �����ܵ��߳�.��ʧ��.");
				Sleep(500);
			}
		}
		else
		{ 
			Sleep(500);
		}
	}
	OutputDebugString("yyf: �����ܵ��߳̽���.");
	return 0;
}
///////////////////////////// CAnonymousPipe Implementation ///////////////////

IMPLEMENT_DYNAMIC(CAnonymousPipe, CObject)

CAnonymousPipe::CAnonymousPipe()
{
  m_hWrite = INVALID_HANDLE_VALUE;//ʵ�ʲ�����д���
  m_hRead = INVALID_HANDLE_VALUE;//ʵ�ʲ����Ķ����
 
  m_hFatherWrite = INVALID_HANDLE_VALUE;//������д���
  m_hFatherRead = INVALID_HANDLE_VALUE;//�����̶����
  m_hChildWrite = INVALID_HANDLE_VALUE;//�ӽ���д���
  m_hChildRead = INVALID_HANDLE_VALUE;//�ӽ��̶����

  bAnonymousPipeThread = false;//�����߳̿�����
}

CAnonymousPipe::~CAnonymousPipe()
{
  Close();
}

BOOL CAnonymousPipe::Create(LPSECURITY_ATTRIBUTES lpPipeAttributes1 , DWORD dwSize1 ,LPSECURITY_ATTRIBUTES lpPipeAttributes2 , DWORD dwSize2 )
{
  ASSERT(!IsOpen());

  //�ܵ�1: ������д->�ӽ��̶� 
  BOOL bSuccess1 = ::CreatePipe(&m_hChildRead, &m_hFatherWrite, lpPipeAttributes1, dwSize1);
  if (!bSuccess1)
  {
    TRACE1("CAnonymousPipe::Create() failed, GetLastError returned %d\n", ::GetLastError());
  }

  //�ܵ�2: �ӽ���д->�����̶� 
  BOOL bSuccess2 = ::CreatePipe(&m_hFatherRead, &m_hChildWrite, lpPipeAttributes2, dwSize2);
  if (!bSuccess2)
  {
	  TRACE1("CAnonymousPipe::Create() failed, GetLastError returned %d\n", ::GetLastError());
  }
  
  if (bSuccess1 && bSuccess2)
  {//�������Ǹ�����,���Զ��õ��Ǹ����
	   m_hRead = m_hFatherRead;  //ʵ�ʲ����Ķ���� 
	   m_hWrite= m_hFatherWrite;//ʵ�ʲ�����д���
	   return true;
  }
  return false;
}

BOOL CAnonymousPipe::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten)
{
  ASSERT(m_hWrite != NULL); //Pipe must be open
  
  if(NULL == m_hWrite)
  {
	  return false;
  }
  BOOL bSuccess = ::WriteFile(m_hWrite, lpBuffer, dwNumberOfBytesToWrite, 
                              &dwNumberOfBytesWritten, NULL);
  if (!bSuccess)
    TRACE1("CAnonymousPipe::Write() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

BOOL CAnonymousPipe::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead)
{
	//OutputDebugString("yyf: update.exe CAnonymousPipe::Read(...)");
  ASSERT(m_hRead != NULL); //Pipe must be open
  if(NULL == m_hRead)
  {
	  return false;
  }

  BOOL bSuccess = ::ReadFile(m_hRead, lpBuffer, dwNumberOfBytesToRead, 
                             &dwNumberOfBytesRead, NULL);
  if (!bSuccess)
    TRACE1("CAnonymousPipe::Read() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

void CAnonymousPipe::ReadAnonymousPipe(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead)
{ 
	if(NULL != piAnonymousPipe )
	{//���ûص�
		piAnonymousPipe->ReadAnonymousPipe(lpBuffer, dwNumberOfBytesToRead);
	}  
}

BOOL CAnonymousPipe::Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD& dwBytesRead, 
                          DWORD& dwTotalBytesAvail, DWORD& dwBytesLeftThisMessage)
{
  ASSERT(m_hRead != NULL);

  if(NULL == m_hRead)
  {
	  return false;
  }

  BOOL bSuccess = ::PeekNamedPipe(m_hRead, lpBuffer, dwBufferSize, &dwBytesRead, 
                                  &dwTotalBytesAvail, &dwBytesLeftThisMessage);
  if (!bSuccess)
    TRACE1("CNamedPipe::Peek() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

BOOL CAnonymousPipe::Close()
{
	
  bAnonymousPipeThread = false;

  BOOL bSuccess = TRUE;
  if (IsOpen())
  { 
	  BOOL bSuccess1 = TRUE;
	  BOOL bSuccess2 = TRUE;
	  BOOL bSuccess3 = TRUE;
	  BOOL bSuccess4 = TRUE;
	  BOOL bSuccess5 = TRUE;
	  BOOL bSuccess6 = TRUE; 

	if (m_hChildRead != INVALID_HANDLE_VALUE)
	{
		bSuccess1 = CloseHandle(m_hChildRead);
	}
	if (m_hChildWrite != INVALID_HANDLE_VALUE)
	{
		bSuccess2 = CloseHandle(m_hChildWrite);
	}
	if (m_hFatherRead != INVALID_HANDLE_VALUE)
	{
		bSuccess3 = CloseHandle(m_hFatherRead);
	}
	if (m_hFatherWrite != INVALID_HANDLE_VALUE)
	{
		bSuccess4 = CloseHandle(m_hFatherWrite); 
	}
     
	bSuccess = (bSuccess1&bSuccess2&bSuccess3&bSuccess4);

	if (m_hFatherWrite == INVALID_HANDLE_VALUE && //������д���
		m_hFatherRead == INVALID_HANDLE_VALUE && //�����̶����
		m_hChildWrite == INVALID_HANDLE_VALUE&& //�ӽ���д���
		m_hChildRead == INVALID_HANDLE_VALUE//�ӽ��̶����
		)
	{//˵����ʱ�� �ӽ��̵Ĺر�.
		bSuccess5 = CloseHandle(m_hWrite);
		if (!bSuccess5)
		{
			TRACE1("CAnonymousPipe::Close() failed, GetLastError returned %d\n", GetLastError());
		}

		bSuccess6 = CloseHandle(m_hRead);
		if (!bSuccess6)
		{
			TRACE1("CAnonymousPipe::Close() failed, GetLastError returned %d\n", GetLastError());
		}

		bSuccess = (bSuccess5 & bSuccess6);
	}

	m_hWrite = INVALID_HANDLE_VALUE;//ʵ�ʲ�����д���
	m_hRead = INVALID_HANDLE_VALUE;//ʵ�ʲ����Ķ����

	m_hFatherWrite = INVALID_HANDLE_VALUE;//������д���
	m_hFatherRead = INVALID_HANDLE_VALUE;//�����̶����
	m_hChildWrite = INVALID_HANDLE_VALUE;//�ӽ���д���
	m_hChildRead = INVALID_HANDLE_VALUE;//�ӽ��̶����
  }

  return bSuccess;
}

#ifdef _DEBUG
void CAnonymousPipe::AssertValid() const
{
  CObject::AssertValid();
  ASSERT(IsOpen());
}
#endif

#ifdef _DEBUG
void CAnonymousPipe::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);

  CString sText;
  sText.Format(_T("Open=%d, Write Handle=%x, Read Handle=%x\n"), IsOpen(), m_hWrite, m_hRead);
  dc << sText;
}
#endif



void CAnonymousPipe::SetAnonymousPipe(IAnonymousPipe * iAnonymousPipe)
{ 
	 this->piAnonymousPipe = iAnonymousPipe; 
}

//2012.08.16 yyf ���ؽ��̵�PID
DWORD CAnonymousPipe::GetProcessIdFromName(LPCTSTR name) //��������У����ؽ��̵�PID��û���з���0 //�Լ������޸ĺ��������������еĽ��̵ĸ���
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


// //��һ������ 2012.08.17 yyf
bool CAnonymousPipe::OpenProcess(LPCSTR lpApplicationName)
{
	strProcessName = lpApplicationName;//��������
	if(0 == GetProcessIdFromName(strProcessName) && NULL != piAnonymousPipe)
	{//����û��ʼ

		m_hWrite = INVALID_HANDLE_VALUE;
		m_hRead = INVALID_HANDLE_VALUE;

		m_hFatherWrite = INVALID_HANDLE_VALUE;//������д���
		m_hFatherRead = INVALID_HANDLE_VALUE;//�����̶����
		m_hChildWrite = INVALID_HANDLE_VALUE;//�ӽ���д���
		m_hChildRead = INVALID_HANDLE_VALUE;//�ӽ��̶����

		bAnonymousPipeThread = false;//�����߳̿�����

		//����2�������ܵ���һ�����ڸ�����д->�ӽ��̶���һ�������ӽ���д->�����̶�.
		
		//�ܵ�1: ������д->�ӽ��̶�
		SECURITY_ATTRIBUTES sa1;
		sa1.bInheritHandle=TRUE;
		sa1.lpSecurityDescriptor=NULL;
		sa1.nLength=sizeof(SECURITY_ATTRIBUTES);

		//�ܵ�2: �ӽ���д->�����̶�
		SECURITY_ATTRIBUTES sa2;
		sa2.bInheritHandle=TRUE;
		sa2.lpSecurityDescriptor=NULL;
		sa2.nLength=sizeof(SECURITY_ATTRIBUTES);

		if(!Create(&sa1,0,&sa2,0))
		{
			// AfxMessageBox("���������ܵ�ʧ�ܣ�");
			return false;
		}

		STARTUPINFO sui;
		//PROCESS_INFORMATION pi;
		ZeroMemory(&sui,sizeof(STARTUPINFO));
		sui.cb=sizeof(STARTUPINFO);
		sui.dwFlags=STARTF_USESTDHANDLES;
		sui.hStdInput=m_hChildRead;//�ӽ���д���
		sui.hStdOutput=m_hChildWrite;//�ӽ��̶����
		sui.hStdError=GetStdHandle(STD_ERROR_HANDLE);
	
		if(!CreateProcess(strProcessName,NULL,NULL,NULL,
				TRUE,0,NULL,NULL,&sui,&pi))//"..\\Child\\Debug\\Child.exe"
		{
			CloseHandle(m_hRead);
			CloseHandle(m_hWrite);
			m_hRead=INVALID_HANDLE_VALUE;
			m_hWrite=INVALID_HANDLE_VALUE;

			CloseHandle(m_hChildRead);
			CloseHandle(m_hChildWrite);
			m_hFatherWrite = INVALID_HANDLE_VALUE;//������д���
			m_hFatherRead = INVALID_HANDLE_VALUE;//�����̶����
			m_hChildWrite = INVALID_HANDLE_VALUE;//�ӽ���д���
			m_hChildRead = INVALID_HANDLE_VALUE;//�ӽ��̶����
			//MessageBox("�����ӽ���ʧ�ܣ�");
			return false;
		}
		//else
		//{
		//	CloseHandle(pi.hProcess);
		//	CloseHandle(pi.hThread);
		//}

		//���������ܵ��̣߳���ʼ��
		bAnonymousPipeThread = true; 
		DWORD dwThreadID = 0;
		HANDLE hThread = CreateThread(0,0,AnonymousPipeThread,this,0,&dwThreadID);
		if (hThread)
		{
			OutputDebugString("yyf: �����ܵ��߳̿�ʼ.");
			::CloseHandle(hThread);
		} 
		 
		return true;
	}
	return false;
}

// //�����ӽ��� ��д��� 
void CAnonymousPipe::SetChildPipe(void)
{ 
	m_hRead=GetStdHandle(STD_INPUT_HANDLE);
	m_hWrite=GetStdHandle(STD_OUTPUT_HANDLE);
}




// ��ʼ�߳�
void CAnonymousPipe::StartThread(void)
{ 
		//���������ܵ��̣߳���ʼ��
		bAnonymousPipeThread = true; 
		DWORD dwThreadID = 0;
		HANDLE hThread = CreateThread(0,0,AnonymousPipeThread,this,0,&dwThreadID);
		if (hThread)
		{
			//OutputDebugString("yyf: update.exe �����ܵ��߳̿�ʼ.");
			::CloseHandle(hThread);
		}
		else
		{
			OutputDebugString("yyf: update.exe �����ܵ��߳�ʧ��.");
		}
}

// ��ʼ�������̹ܵ�
void CAnonymousPipe::InitFatherPipe(IAnonymousPipe *  iAnonymousPipe)
{
	SetAnonymousPipe(iAnonymousPipe);//��ʼ�� �����ܵ�  
}

// ��ʼ���ӽ��̹ܵ�
void CAnonymousPipe::InitChildPipe(IAnonymousPipe * iAnonymousPipe)
{
	 SetAnonymousPipe(iAnonymousPipe);//��ʼ�� �����ܵ� 
	 SetChildPipe();
	 StartThread();
}
