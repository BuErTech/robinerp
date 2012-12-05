#include "StdAfx.h"
#include "BzLog.h"

char CBzLog::m_szRootName[64];

CBzLog::CBzLog(void)
{
	m_pLogFile = NULL;
	m_uGameID = 0;
	::memset(m_szLogID,NULL,sizeof(m_szLogID));
}

CBzLog::~CBzLog(void)
{
	ReleaseBzLog();
}

void CBzLog::init()
{
	::memset(m_szRootName,NULL,sizeof(m_szRootName));
}

//�����ļ���
bool CBzLog::CreateGameRoomDir(char* szName)
{
	TCHAR szPath[MAX_PATH];
	::strcpy(szPath,"Roomlogs");
	::CreateDirectory(szPath,NULL);
	::sprintf(szPath,"Roomlogs\\%s",szName);
	::CreateDirectory(szPath,NULL);

	strcpy(m_szRootName,szName);

	return true;
}

//������־
bool CBzLog::CreateGameRoomLog(const ComRoomInfo* pRoomInfo)
{
	if (m_pLogFile != NULL)
	{
		return false;
	}

	TCHAR szPathName[MAX_PATH];
	SYSTEMTIME t;

	::GetLocalTime(&t);
	memset(m_szName,NULL,MAX_PATH);

	//�γ���־ID
	wsprintf(m_szLogID,TEXT("%02d%02d%02d_%d(%d)"),t.wHour,t.wMinute,t.wSecond,\
		pRoomInfo->uNameID,pRoomInfo->uRoomID);
	wsprintf(m_szName,TEXT("%s.log"),m_szLogID);
	wsprintf(szPathName,TEXT("Roomlogs\\%s\\%s"),m_szRootName,m_szName);

	m_pLogFile = new CFile(szPathName,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if (m_pLogFile == NULL)
	{
		return false;
	}

	m_uGameID = pRoomInfo->uNameID;

	//�����־ͷ
	CString strOutput;
	strOutput.Format("��־: ��ϷID = %d ����� = %d ʱ�� = %04d.%02d.%02d %02d:%02d:%02d"
		,pRoomInfo->uNameID,pRoomInfo->uRoomID,t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond);
	Writeline(strOutput);

	Writeline("===============================================================================================================");

	Writeline("");
	
	return true;
}

//���������Ϣ
void CBzLog::OutputTraceStr(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage_t[BUFSIZ];
	char szMessage[BUFSIZ];
#ifdef WIN32
	_vsnprintf(szMessage_t, BUFSIZ - 1, lpszFormat, pArg);
#else
	vsnprintf(szMessage_t, BUFSIZ - 1, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage_t);

	SYSTEMTIME t;
	::GetLocalTime(&t);
	//�γ���־ID
	_stprintf(szMessage,TEXT("[%02d:%02d:%02d.%03d] %s"),t.wHour,t.wMinute,t.wSecond,t.wMilliseconds,szMessage_t);

	Writeline(szMessage);
}


//�ͷ���־
bool CBzLog::ReleaseBzLog()
{
	if (m_pLogFile)
	{
		m_pLogFile->Close();
		delete m_pLogFile;
		m_pLogFile = NULL;
		return true;
	}

	return false;
}

//д��־
bool CBzLog::Writeline(CString str)
{
	if (m_pLogFile == NULL)
	{
		return false;
	}

	m_pLogFile->Write(str.GetBuffer(),str.GetLength());
	m_pLogFile->Write(_TEXT("\r\n"),2);

	return true;
}

//��ȡ��־ID
const char* CBzLog::GetLogID()
{
	if (!m_pLogFile)
	{
		return NULL;
	}

	return m_szLogID;
}