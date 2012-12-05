
#pragma once

#ifndef AFCEXCEPTION_H_2003_7
#define AFCEXCEPTION_H_2003_7

///***********************************************************************************************///

///ϵͳ����
#define EX_UNKNOWN_ERROR			0						///δ֪����
#define EX_VIRTUAL_MEM_LOW			1						///�����ڴ治��
#define EX_HEAP_MEM_LOW				2						///���ڴ治��

///���ݿ����
#define EX_DATA_CONNECT				100						///���ݿ����Ӵ���
#define EX_DATA_USER				101						///���ݿ����ִ���
#define EX_DATA_PASS				102						///���ݿ��������
#define EX_DATA_EXEC				103						///���ݿ�ִ�д���

///�������
#define EX_SOCKET_CREATE			200						///���罨������
#define EX_SOCKET_SEND_ERROR		201						///���緢�ʹ���
#define EX_SOCKET_RECV_ERROR		202						///������ܴ���

///�������
#define EX_SERVICE_START			300						///��������
#define EX_SERVICE_PAUSE			301						///������ͣ
#define EX_SERVICE_STOP				302						///����ֹͣ
#define EX_SERVICE_BUSY				303						///����æ
#define EX_SERVICE_UNKNOW			304						///δ֪������Ϣ

///�Զ������

///***********************************************************************************************///

///�쳣��
class AFX_EXT_CLASS CAFCException
{
	///��������
protected:
	UINT					m_uErrorCode;					///������
	bool					m_bAutoDelete;					///�Ƿ��Զ�ɾ��
	TCHAR					m_szMessage[255];				///������Ϣ
	static bool				m_bShowError;					///�Ƿ���ʾ����
	
	///��������
public:
	///���캯��
	inline CAFCException(TCHAR * szErrorMessage, UINT uErrorCode=EX_UNKNOWN_ERROR, bool bAutoDelete=true);
	///��������
	virtual ~CAFCException(void);

	///���ܺ���
public:
	///��ȡ�������
	virtual UINT GetErrorCode() const;
	///��ȡ������Ϣ
	virtual TCHAR const * GetErrorMessage(TCHAR * szBuffer, int iBufLength) const;
	///ɾ������
	virtual bool Delete();
	///�����Ƿ���ʾ����
	static bool ShowErrorMessage(bool bShowError);
};

#endif