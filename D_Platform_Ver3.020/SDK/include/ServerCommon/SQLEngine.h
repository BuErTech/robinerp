// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SQLENGINE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SQLENGINE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SQLENGINE_EXPORTS
#define SQLENGINE_API __declspec(dllexport)
#else
#define SQLENGINE_API __declspec(dllimport)
#endif

//ע�⣺һ�������У����ֻ����15������

//0x0**		���ݿⶨ��
//0x1**		�������
//>0x100	���

#define DB_TYPE_SQLSERVER	0x0			//SQL Server ���ݿ�
#define DB_TYPE_MYSQL		0x1			//My SQL ���ݿ�
#define DB_TYPE_ORACLE		0x2			//Oracle ���ݿ�

#define DB_ERR_NORESOURCE		0x400		//û���㹻����Դ
#define DB_ERR_CONNECT_FAILED	0x401		//�������ݿ�ʧ��
#define DB_ERR_GETRETURNVALUE	0x402		//����ִ��ֵʧ��

extern SQLENGINE_API int nSQLEngine;

SQLENGINE_API int fnSQLEngine(void);

/**************************
Function	:sqlAddConnection
Desc		:��ʼ�����ݿ⣬ֱ���ṩ������ʽ
Author		:Fred Huang 2008-11-25
Para		:
	szServer	:���ݿ������
	szAccount	:���ݿ�����ʺ�
	szPassword	:���ݿ��������
	szDatabase	:���ݿ���
	nPort		:���ݿ�˿�
				��MS SQL Server�У���nPort=0ʱʹ��1433�˿�
				��MySQL�У���nPortʱ��ʹ��3306�˿�
	nSQLClass		:ʹ�õ����ݿ�
				ȱʡΪʹ��MS SQL Server���ݿ�
	szDetectTable	:���ݿ��Զ���������ʹ�õı�
				Ŀǰ�汾δʵ��
return		:����һ�����ݿ������
			< 0x1000���������ݿ�ʧ��
**************************/
extern SQLENGINE_API HANDLE	sqlAddConnection(LPCTSTR szServer,LPCTSTR szAccount,LPCTSTR szPassword,LPCTSTR szDatabase,unsigned short nPort=0,int nSQLClass=DB_TYPE_SQLSERVER,LPCTSTR szDetectTable=TEXT(""));

/**************************
Function	:sqlAddConnection
Desc		:��ʼ�����ݿ⣬ʹ��bcf�ļ�
Author		:Fred Huang 2008-11-25
Para		:
	szBCFFile	:bcf�ļ���
				��������	:
					[DATABASECLASS]
					SQLCLASS=0

					[SQLSERVER]
					DBSERVER=127.0.0.1
					DBACCOUNT=sa
					DBPASSWORD=szbzw
					DBPORT=0
					DETECTTABLE=
					DATABASE=bzwoa

					[MYSQL]
					DBSERVER=localhost
					DBACCOUNT=root
					DBPASSWORD=szbzw
					DATABASE=bzw
					DBPORT=3306
					DETECTTABLE=
return		:����һ�����ݿ������
			< 0x1000���������ݿ�ʧ��
**************************/
extern SQLENGINE_API HANDLE sqlAddConnection(LPCTSTR szBCFFile);

/**************************
Function	:sqlExec
Desc		:ִ����ͨ��T-SQL �� p/SQL
Author		:Fred Huang 2008-11-25
Para		:
	hHandle		:���ݿ������
	szSQL		:����� T-SQL �� p/sql
	bReturnRecordsets	:�Ƿ���Ҫ���ؼ�¼��
						ȱʡΪ����Ҫ���ؼ�¼��
						ֻ��bReturnRecordsets=trueʱ���ſ���ͨ��sqlGetValueȡ�����ݿ��е�ֵ
return		:����һ����¼������
				=0��ִ��ʧ��
				=1��ִ�гɹ�������Ҫ���ؼ�¼��ʱ
				>1��ִ�гɹ�����¼������
**************************/

extern SQLENGINE_API HANDLE sqlExec(HANDLE hHandle,LPCTSTR szSQL,bool bReturnRecordsets=false);

/**************************
Function	:sqlSPSetName
Desc		:ִ�д洢����ǰ��Ҫ���ô洢��������
Author		:Fred Huang 2008-11-25
Para		:
	hHandle		:���ݿ������
	szSPName	:�洢���̵�����
	bReturnValue:�Ƿ��з���ֵ
				��Ϊ�洢�����Ƿ��з���ֵ���Բ���Ӱ��ܴ����Ա���Ҫ�ṩ��ȱʡΪ����Ҫ
				ע�⣺���Ǽ�¼��
return		:����һ���洢���̼�¼������
			=0��ִ��ʧ��
			>0��ִ�гɹ�����¼������
**************************/
extern SQLENGINE_API HANDLE	sqlSPSetName(HANDLE hHandle,LPCTSTR szSPName,bool bReturnValue=false);

/**************************
Function	:sqlSPAddPara
Desc		:��Դ洢����������ֲ���
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:�洢���̼�¼������
	szPara		:�������ƣ�����SQL Server��Ч��MySQLֻʹ�� lValue
	lValue		:����ֵ
return		:�Ƿ���ӳɹ�
Att			:��Ҫע����ǣ�����SQL Server����������ӿ���û��˳��
			������MySQL����������ӱ����ϸ�İ��洢���̵Ĳ���˳�����
**************************/
extern SQLENGINE_API bool	sqlSPAddPara(HANDLE hDatabase,HANDLE hRecord,LPCTSTR szPara,long lValue);						//long ���� 

/**************************
Function	:sqlSPAddPara_i64
Desc		:��Դ洢����������ֲ���
Author		:Jian Guankun 2011-11-29
Para		:
	hDatabase	:���ݿ������
	hRecord		:�洢���̼�¼������
	szPara		:�������ƣ�����SQL Server��Ч��MySQLֻʹ�� lValue
	lValue		:����ֵ
return		:�Ƿ���ӳɹ�
Att			:��Ҫע����ǣ�����SQL Server����������ӿ���û��˳��
			������MySQL����������ӱ����ϸ�İ��洢���̵Ĳ���˳�����
**************************/
extern SQLENGINE_API bool	sqlSPAddPara_i64(HANDLE hDatabase,HANDLE hRecord,LPCTSTR szPara,__int64 lValue);						//64λ���� ���� 


/**************************
Function	:sqlSPAddPara
Desc		:��Դ洢��������ַ�����
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:�洢���̼�¼������
	szPara		:�������ƣ�����SQL Server��Ч��MySQLֻʹ�� szValue
	szValue		:����ֵ
	szSize		:�����Ĵ�С������SQL Server��Ч��MySQLֻʹ�� szValue
return		:�Ƿ���ӳɹ�
Att			:��Ҫע����ǣ�����SQL Server����������ӿ���û��˳��
			������MySQL����������ӱ����ϸ�İ��洢���̵Ĳ���˳�����
**************************/
extern SQLENGINE_API bool	sqlSPAddPara(HANDLE h1,HANDLE h2,LPCTSTR szPara,LPCTSTR szValue,int szSize);		//�ַ�������
//ִ��SP���Ⱦ����������������ƺͲ���
//bReturnRecordsets ���Ƿ��з��ؼ�¼��

/**************************
Function	:sqlSPExec
Desc		:ִ�д洢����
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:�洢���̼�¼��������
	bReturnRecordsets	:�Ƿ���Ҫ���ؼ�¼��
						ע�⣬�Ƿ��з��ؼ�¼�������������ϴ�������Ҫ��ȷ
return		:�Ƿ�ִ�гɹ�
**************************/
extern SQLENGINE_API bool	sqlSPExec(HANDLE hDatabase,HANDLE hRecord,bool bReturnRecordsets=false);

/**************************
Function	:sqlSPClose
Desc		:�رմ洢���̼�¼����
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:�洢���̼�¼��������
**************************/
extern SQLENGINE_API void	sqlSPClose(HANDLE hDatabase,HANDLE hRecord);
//�ر�ĳ��Command

/**************************
Function	:sqlCloseRecord
Desc		:�ر�ĳ����¼������
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:��¼��������
**************************/
extern SQLENGINE_API void	sqlCloseRecord(HANDLE hDatabase,HANDLE hRecord);

/**************************
Function	:sqlCloseRecord
Desc		:�ر����ݿ���������еļ�¼�����󣬰�����ͨ��¼���ʹ洢���̶���
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
att			:���ر����ݿ�����
**************************/
extern SQLENGINE_API void	sqlCloseAllRecord(HANDLE hHandle);

/**************************
Function	:sqlCloseConnection
Desc		:�ر����ݿ����ӣ�����ر����ݿ������еļ�¼�����󣬰�����ͨ��¼���ʹ洢���̶���
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
**************************/
extern SQLENGINE_API void	sqlCloseConnection(HANDLE hHandle);

/**************************
Function	:sqlRelease
Desc		:�ر��������ݿ����ӣ�����ر����ݿ������еļ�¼�����󣬰�����ͨ��¼���ʹ洢���̶���
Author		:Fred Huang 2008-11-25
**************************/
extern SQLENGINE_API void	sqlRelease();
//ȡ��SP��ִ�з���ֵ��������bReturcValue=trueʱ��Ч

/**************************
Function	:sqlSPGetReturnValue
Desc		:ȡ�ô洢�����еķ���ֵ��ֻ����sqlSPSetName�� bReturnValue=true,�����ݿ�洢������ȷ�з���ֵʱ�ſ���
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:��¼��������
**************************/
extern SQLENGINE_API long	sqlSPGetReturnValue(HANDLE hDatabase,HANDLE hRecord);

/**************************
Function	:sqlEndOfFile
Desc		:��¼���Ƿ��ѵ����һ��
			��ͨ��¼���ʹ洢���̼�¼��ʹ��ͬһ����
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:��¼��������
**************************/
extern SQLENGINE_API bool	sqlEndOfFile(HANDLE hDatabase,HANDLE hRecord);

/**************************
Function	:sqlMoveNext
Desc		:�ƶ�����һ����¼
			��ͨ��¼���ʹ洢���̼�¼��ʹ��ͬһ����
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:��¼��������
**************************/
extern SQLENGINE_API void	sqlMoveNext(HANDLE hDatabase,HANDLE hRecord);

extern SQLENGINE_API long	sqlGetRecordCount(HANDLE hDatabase,HANDLE hRecord);
/**************************
Function	:sqlGetValue
Desc		:ȡ��¼���е�����
Author		:Fred Huang 2008-11-25
Para		:
	hDatabase	:���ݿ������
	hRecord		:��¼��������
	szFieldName	:�ֶ���
	value		:ȡֵ����
att			:���º�����Ϊͬһ����
**************************/
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,int &nValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,UINT &lValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,LONG &lValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,DOUBLE &dValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,bool &bValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,BYTE &dValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,DWORD &dValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,__int64 &llValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,CString &szValue);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,COleDateTime &tTime);
extern SQLENGINE_API bool	sqlGetValue(HANDLE h1,HANDLE h2,LPCTSTR szFieldName,TCHAR szBuffer[], UINT uSize);

/**************************
Function	:sqlSetCharacter
Desc		:�������ݿ��ַ�����ֻ��MySQL ��Ч����ʹ�õĻ���Ĭ��Ϊ gb2312
Author		:Fred Huang 2008-11-25
Para		:
	hHandle		:���ݿ������
	character	:�ַ�������
**************************/
extern SQLENGINE_API void	sqlSetCharacter(HANDLE hHandle,char *character);