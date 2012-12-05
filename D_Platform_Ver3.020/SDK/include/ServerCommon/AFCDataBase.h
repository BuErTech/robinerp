#ifndef AFCDATABASE_H_FILE_2003_07
#define AFCDATABASE_H_FILE_2003_07

#include <afx.h>
#include <afxdisp.h>
#include "../common/ComStruct.h"
#include "AFCInterFace.h"
#include "AFCLock.h"
#include "DataLine.h"
#include "AFCException.h"
#include "icrsint.h"
#include "adoid.h"
//#include <Icrsint.h>
//
//  //#import "C:\Program Files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//
//#pragma warning (disable: 4146)
//#import "MsAdo15.dll" rename_namespace("ADOCG") rename("EOF","EndOfFile")
//using namespace ADOCG;

#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;



///��˵��
class CDataLine;
class CAFCDataBaseManage;

///***********************************************************************************************///


///***********************************************************************************************///

#ifndef STRUCT_DATABASELINEHEAD
#define STRUCT_DATABASELINEHEAD
///���ݿ����ݰ�ͷ�ṹ
struct DataBaseLineHead
{
	DataLineHead					DataLineHead;							///����ͷ
	UINT							uHandleKind;							///��������
	UINT							uIndex;									///��������
	DWORD							dwHandleID;								///�����ʶ
};
#endif//STRUCT_DATABASELINEHEAD
///���ݿ⴦����
class AFX_EXT_CLASS CAFCDataBaseManage
{
	///��������
protected:
	bool							m_bInit;								///��ʼ����־
	bool							m_bRun;									///���б�־
	IDataBaseHandleService			* m_pHandleService;						///���ݴ���ӿ�

	///��������
public:

	HANDLE							hDBLogon;								///��¼���ݿ�
	HANDLE							hDBNative;								///�������ݿ�

	///��Ϣ����
public:
	KernelInfoStruct				* m_pKernelInfo;						///�ں�����
	ManageInfoStruct				* m_pInitInfo;							///��ʼ������ָ��

	///��Ϣ����
protected:
	///CString							m_strLogonDataConnect;					///��½�ַ���
	///CString							m_strNativeDataConnect;					///�����ַ���

	///�ں˱���
protected:
	HANDLE							m_hThread;								///�߳̾��
	HANDLE							m_hCompletePort;						///��ɶ˿�
public:
	CDataLine						m_DataLine;								///���ݶ���

	///��������
public:
	///���캯��
	CAFCDataBaseManage();
	///��������
	virtual ~CAFCDataBaseManage(void);

	///������
public:
	///��ʼ������
	bool Init(ManageInfoStruct * pInitInfo, KernelInfoStruct * pKernelInfo, IDataBaseHandleService * pHandleService,IDataBaseResultService * pResultService);
	///ȡ����ʼ��
	bool UnInit();
	///��ʼ����
	bool Start();
	///ֹͣ����
	bool Stop();
	///���봦�����
	bool PushLine(DataBaseLineHead * pData, UINT uSize, UINT uHandleKind, UINT uIndex,DWORD dwHandleID);

	///���ܺ���
public:
	///�����������
	bool CheckSQLConnect();
	
	///�������ݿ� 2012-02-22 zht
	bool SQLConnectReset();

	int		m_sqlClass;
	CString	m_szServer;
	CString m_szAccount;
	CString m_szPassword;
	CString m_szDatabase;
	SHORT	m_nPort;
	CString m_szDetectTable;
	BOOL    m_bsqlInit;


	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	void SQLConnect();
	bool SQLExec(const TCHAR * szSQL);

	///�ڲ�����
private:
	///���ݿ⴦���߳�
	static unsigned __stdcall DataServiceThread(LPVOID pThreadData);
	///��־���ݿ⴦���߳�
	static unsigned __stdcall DataLogThread(LPVOID pThreadData);
};

///***********************************************************************************************///

///���ݿ⴦��ӿ���
class AFX_EXT_CLASS CDataBaseHandle : public IDataBaseHandleService
{
	///��������
protected:
	KernelInfoStruct						* m_pKernelInfo;				///�ں�����
	ManageInfoStruct						* m_pInitInfo;					///��ʼ������ָ��
	IDataBaseResultService					* m_pRusultService;				///�������ӿ�
	CAFCDataBaseManage						* m_pDataBaseManage;			///���ݿ����

	///��������
public:
	///���캯��
	CDataBaseHandle(void);
	///��������
	virtual ~CDataBaseHandle(void);

	///�ӿں���
public:
	///���ò���
	virtual bool SetParameter(IDataBaseResultService * pRusultService, CAFCDataBaseManage * pDataBaseManage, ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData);

	///���ܺ���
};

///***********************************************************************************************///

#endif