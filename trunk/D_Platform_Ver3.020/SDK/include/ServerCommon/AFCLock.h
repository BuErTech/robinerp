#pragma once

#ifndef AFCLOCK_H_2003_7
#define AFCLOCK_H_2003_7

class CAFCRWLock;
class CAFCSignedLock;

///����������
class AFX_EXT_CLASS CSignedLockObject
{
	///��������
private:
	int						m_iLockCount;						///��������
	CAFCSignedLock			* m_pLockObject;					///��������

	///��������
public:
	///���캯��
	CSignedLockObject(CAFCSignedLock * pLockObject, bool bAutoLock);
	///��������
	~CSignedLockObject();

public:
	///��������
	inline void Lock();
	///�������� 
	inline void UnLock();
};

///��ͨ������
class AFX_EXT_CLASS CAFCSignedLock
{
	///��Ԫ����
	friend class CSignedLockObject;

	///��������
private:
	CRITICAL_SECTION				m_csLock;					///�ؼ���

	///��������
public:
	///���캯��
	inline CAFCSignedLock() { ::InitializeCriticalSection(&m_csLock); }
	///��������
	inline ~CAFCSignedLock() { ::DeleteCriticalSection(&m_csLock); }

	///���ܺ���
private:
	///��������
	inline void Lock() { ::EnterCriticalSection(&m_csLock); }
	///�������� 
	inline void UnLock() { ::LeaveCriticalSection(&m_csLock); }
};

///�߳�ͬ����д��
class AFX_EXT_CLASS CAFCRWLock
{
	///��������
private:
	HANDLE							m_hReadLock;				///����
	HANDLE							m_hWriteLock[2];			///д��
	CRITICAL_SECTION				m_csLock;					///�ؼ���
	volatile long					m_iReadCount;				///������

	///��������
public:
	///���캯��
	CAFCRWLock(void);
	///��������
	~CAFCRWLock(void);

	///���ܺ���
public:
	///��д
	bool LockWrite(DWORD dwWaitTime=INFINITE);
	///����
	bool LockRead(DWORD dwWaitTime=INFINITE);
	///��д
	bool UnLockWrite();
	///���
	bool UnLockRead();
};

#endif