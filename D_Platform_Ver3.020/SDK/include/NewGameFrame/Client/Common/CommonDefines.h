#pragma once

#include <windows.h>
#include <tchar.h>

#ifdef GRAPH_EXPORTS
#define GRAPH_API __declspec(dllexport)
#else
#define GRAPH_API __declspec(dllimport)
#endif GRAPH_EXPORTS

#pragma warning ( disable: 4996)

#define WM_IPC_MESSAGE				(WM_USER+999)						//IPC ��Ϣ

#ifdef _UNICODE
#	define tcsscanf swscanf
#else
#	define tcsscanf sscanf
#endif


/// ��ȫɾ�����ͷ�ָ��

#define SafeDelete( p )\
	if (NULL!=p)	\
	{				\
		delete p;	\
		p = NULL;	\
	}

#define SafeDeleteArray( p )\
	if (NULL!=p)	\
	{				\
		delete []p;	\
		p = NULL;	\
	}
#define SafeRelease( p ) \
	if (NULL != p)		\
	{					\
		p->Release();	\
		p = NULL;		\
	}

#define SafeClose( p ) \
	if (NULL != p)		\
	{					\
		CloseHandle(p);	\
		p = NULL;		\
	}

/// �Զ����ַ�����
class Tstring
{
public:
	/// Ĭ�Ϲ��캯��
	Tstring() : m_nLen(1)
	{
		m_pBuf = new TCHAR[1];
		m_pBuf[0] = TEXT('\0');
	}

	/// �����ַ���ָ��Ĺ��캯��
	Tstring(const TCHAR *p)
	{
		if (p==NULL)
		{
			m_pBuf = new TCHAR[1];
			m_pBuf[0] = TEXT('\0');
			m_nLen = 1;
			return;
		}
		m_nLen = (int)_tcslen(p) + 1;
		m_pBuf = new TCHAR[m_nLen];
		_tcscpy(m_pBuf, p);
	}
	/// ��������
	~Tstring(){SafeDeleteArray(m_pBuf);m_nLen = 0;}

	/// �������캯��
	Tstring(const Tstring &rhs)
	{
		m_nLen = (int)_tcslen(rhs.m_pBuf) + 1;
		m_pBuf = new TCHAR[m_nLen];
		_tcscpy(m_pBuf, rhs.m_pBuf);
	}
	/// ��ֵ������
	Tstring &operator=(const Tstring &rhs)
	{
		int nLen = (int)_tcslen(rhs.m_pBuf)+1;
		if (m_nLen<nLen)
		{
			SafeDeleteArray(m_pBuf);
			m_pBuf = new TCHAR[nLen];
			_tcscpy(m_pBuf, rhs.m_pBuf);
			m_nLen = nLen;
		}
		else
		{
			_tcscpy(m_pBuf, rhs.m_pBuf);
		}
		return *this;
	}

	bool operator<(const Tstring &rhs) const
	{
		return _tcscmp(m_pBuf, rhs.m_pBuf)<0;
	}
	bool operator==(const Tstring &rhs) const
	{
		return _tcscmp(m_pBuf, rhs.m_pBuf)==0;
	}
	bool operator>(const Tstring &rhs) const
	{
		return _tcscmp(m_pBuf, rhs.m_pBuf)>0;
	}
	/// ���ַ���ĩβ�����һ�ַ���, +������
	Tstring operator+(const TCHAR *szParam) const
	{
		Tstring ret(*this);
		if (szParam == NULL)
		{
			return ret;
		}
		int nLen = (int)_tcslen(m_pBuf) + (int)_tcslen(szParam) + 1;
		if (ret.m_nLen<nLen)
		{
			TCHAR *p = ret.m_pBuf;
			ret.m_pBuf = new TCHAR[nLen];
			_tcscpy(ret.m_pBuf, p);
			SafeDeleteArray(p);
			_tcscat(ret.m_pBuf, szParam);
			ret.m_nLen = nLen;
		}
		else
		{
			_tcscat(ret.m_pBuf, szParam);
		}
		return ret;
	}
	/// ���ַ���ĩβ�����һ�ַ���, +=������
	Tstring &operator+=(const TCHAR *szParam)
	{
		if (szParam == NULL)
		{
			return *this;
		}
		int nLen = (int)_tcslen(m_pBuf) +(int) _tcslen(szParam) + 1;
		if (m_nLen<nLen)
		{
			TCHAR *p = m_pBuf;
			m_pBuf = new TCHAR[nLen];
			_tcscpy(m_pBuf, p);
			SafeDeleteArray(p);
			_tcscat(m_pBuf, szParam);
			m_nLen = nLen;
		}
		else
		{
			_tcscat(m_pBuf, szParam);
		}
		return *this;
	}
	/// �Ӻ���ǰ��һ���ַ�
	int ReverseFind(TCHAR ch) const
	{
		int nLen = (int)_tcslen(m_pBuf);
		for (int i=nLen-1; i>=0; --i)
		{
			if (m_pBuf[i] == ch)
			{
				return i;
			}
		}
		return -1;
	}
	/// ���س���
	int GetLength(){return (int)_tcslen(m_pBuf);}
	/// ��ȡ���ָ�����ȵ��ַ���
	Tstring Left(int nPos) const
	{
		Tstring ret(*this);
		ret.m_pBuf[nPos] = TEXT('\0');
		return ret;
	}
	/// ����const TCHARָ��
	operator const TCHAR *() const
	{
		return m_pBuf;
	}
	/// ��ʽ���ַ���
	Tstring &Format(const TCHAR *szFormat, ...)
	{
		va_list arg;
		va_start( arg, szFormat );

		TCHAR szBuf[512];
		wsprintf( szBuf, szFormat, arg );
		int nLen = (int)_tcslen(szBuf)+1;
		if (m_nLen<nLen)
		{
			SafeDeleteArray(m_pBuf);
			m_pBuf = new TCHAR[nLen];
			_tcscpy(m_pBuf, szBuf);
			m_nLen = nLen;
		}
		else
		{
			_tcscpy(m_pBuf, szBuf);
		}
		return *this;
	}
    const TCHAR *GetBuffer()
    {
        return m_pBuf;
    }

private:
	TCHAR *m_pBuf;	///< ������
	int m_nLen;		///< ����������
};

/// �����ڽṹ�����ʼ����Ա�ĺ궨�壬�����г�Ա������ֵ0
/// ����ʹ��ע�����ʹ�ñ���Ľṹ���ڲ������麯������
///
#define INIT_STRUCT( T ) \
	T()\
	{\
		ZeroMemory(this, sizeof(T));\
	}
/// ʾ������
struct Test_t
{
	int _nLen;
	char _pStr[256];
	INIT_STRUCT( Test_t );	///< �ṹ�����������꣬���൱�������һ����ʼ����Ա�Ĺ��캯��
};



