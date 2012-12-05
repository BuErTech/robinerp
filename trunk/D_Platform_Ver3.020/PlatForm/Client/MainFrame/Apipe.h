/*
Module : APIPE.H
Purpose: Defines the interface for an MFC wrapper classe
for Win32 Anonymous Pipes
*/

#ifndef __APIPE_H__
#define __APIPE_H__

#include < windows.h > 

//�����ܵ� �ص��ӿ� 2012.08.17 yyf
interface IAnonymousPipe
{ 
	virtual  void ReadAnonymousPipe(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead) = 0;//�������ܵ��ص��ӿ�
};
//�����ܵ� ��.2012.08.17 yyf
//Wrapper class to encapsulate an anonymous pipe
class CAnonymousPipe : public CObject
{
public:
	//Constructors / Destructors
	CAnonymousPipe();
	~CAnonymousPipe();


	//General functions
	BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten);	
	void ReadAnonymousPipe(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead);
	BOOL Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead);
	BOOL Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD& dwBytesRead, 
		DWORD& dwTotalBytesAvail, DWORD& dwBytesLeftThisMessage);
	BOOL Close();

	//State accessors
	BOOL IsOpen() const { return (m_hWrite != INVALID_HANDLE_VALUE); };

	//Diagnostics / Debug support
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_DYNAMIC(CAnonymousPipe)

	//Creation & Opening
	BOOL Create(LPSECURITY_ATTRIBUTES lpPipeAttributes1 = NULL, DWORD dwSize1 = 0,LPSECURITY_ATTRIBUTES lpPipeAttributes2 = NULL, DWORD dwSize2 = 0);

	HANDLE m_hWrite;//ʵ�ʲ�����д���
	HANDLE m_hRead;//ʵ�ʲ����Ķ����
	HANDLE m_hFatherWrite;//������д���
	HANDLE m_hFatherRead;//�����̶����
	HANDLE m_hChildWrite;//�ӽ���д���
	HANDLE m_hChildRead;//�ӽ��̶����
	IAnonymousPipe *piAnonymousPipe;//���ص��Ľӿ�ָ��.
	LPCTSTR strProcessName;//��������
	bool    bAnonymousPipeThread;//�����߳̿�����
    PROCESS_INFORMATION pi;
public:
	void SetAnonymousPipe(IAnonymousPipe * iAnonymousPipe);//��ʼ�����ص��Ľӿ�ָ��  2012.08.17 yyf
	// //��һ������ 2012.08.17 yyf
	bool OpenProcess(LPCSTR lpApplicationName);
	// //�����ӽ��� ��д���  2012.08.17 yyf
	void SetChildPipe(void); 
	// ��ʼ�߳�
	void StartThread(void);
public:
	// ��ʼ�������̹ܵ�
	void InitFatherPipe(IAnonymousPipe *  iAnonymousPipe);
public:
	// ��ʼ���ӽ��̹ܵ�
	void InitChildPipe(IAnonymousPipe * iAnonymousPipe);
	DWORD GetProcessIdFromName(LPCTSTR name);//��������У����ؽ��̵�PID��û���з���0 
	PROCESS_INFORMATION GetProcessInfo() const { return pi; } 
};

//---------ͬ���� �� -------------2012.08.22 yyf ����.---begin
class InstanceLock;

//ͬ���� �ࡣ�Զ����룬�˳��ٽ���.
class InstanceLockBase
{
	friend class InstanceLock;
public:
	CRITICAL_SECTION cs;

	void Lock()
	{
		::EnterCriticalSection( & cs);
	} 

	void Unlock()
	{
		::LeaveCriticalSection( & cs);
	} 
	 
	InstanceLockBase()
	{
		::InitializeCriticalSection( & cs);
	}
	~InstanceLockBase()
	{
		::DeleteCriticalSection( & cs);
	}
};

//ͬ���������ࡣͨ������ InstanceLockBase ���ã����в���.
class InstanceLock
{
	InstanceLockBase * _pObj;
public :
	InstanceLock(InstanceLockBase * pObj)
	{
		_pObj = pObj;
		if (NULL != _pObj)
			_pObj -> Lock();
	}
	~ InstanceLock()
	{
		if (NULL != _pObj)
			_pObj -> Unlock();
	}
};

//---------ͬ���� �� -------------2012.08.22 yyf ����.---end

#endif //__APIPE_H__