#include "StdAfx.h"
#include "AfcRegistry.h"

//���캯��
CGameGoMemory::CGameGoMemory(HKEY hRegKey) : m_hRegKey(hRegKey)
{
}

//��������
CGameGoMemory::~CGameGoMemory(void)
{
	Close();
}

//��
bool CGameGoMemory::Open(TCHAR * szSubKey)
{
	HKEY hRegKey=NULL;
	long int lReturn=RegOpenKeyEx(m_hRegKey,szSubKey,0L,KEY_ALL_ACCESS,&hRegKey);
	if (lReturn==ERROR_SUCCESS)
	{
        m_hRegKey=hRegKey;
		return true;
	}
	return false;
}

//�ر�
void CGameGoMemory::Close()
{
	if (m_hRegKey)
	{
		RegCloseKey(m_hRegKey);
		m_hRegKey=NULL;
	}
	return;
}

//�����Ӽ�
bool CGameGoMemory::CreateKey(TCHAR * szSubKey)
{
	HKEY hRegKey=NULL;
	DWORD dwDispostion=0;
	long int lReturn=RegCreateKeyEx(m_hRegKey,szSubKey,0L,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hRegKey,&dwDispostion);
	if (lReturn==ERROR_SUCCESS)
	{
		m_hRegKey=hRegKey;
		return true;
	}
	return false;
}

//ɾ��
bool CGameGoMemory::DeleteKey(HKEY hRegKey, TCHAR * szSubKey)
{
	long int lReturn=RegDeleteKey(hRegKey,szSubKey);
	return (lReturn==ERROR_SUCCESS);
}

//ɾ��
bool CGameGoMemory::DeleteValue(TCHAR * szValueName)
{
	long int lReturn=RegDeleteValue(m_hRegKey,szValueName);
	return (lReturn==ERROR_SUCCESS);
}

//����
bool CGameGoMemory::SaveKey(TCHAR * szFileName)
{
	long int lReturn=RegSaveKey(m_hRegKey,szFileName,NULL);
	return (lReturn==ERROR_SUCCESS);
}

//�ָ�
bool CGameGoMemory::RestoreKey(TCHAR * szFileName)
{
	long int lReturn=RegRestoreKey(m_hRegKey,szFileName,REG_WHOLE_HIVE_VOLATILE);
	return (lReturn==ERROR_SUCCESS);
}

//��ȡ
bool CGameGoMemory::Read(TCHAR * szValueName, CString & strOutVal)
{
	return false;
}

//��ȡ
bool CGameGoMemory::Read(TCHAR * szValueName, TCHAR * szOutVal, UINT uBufferSize)
{
	return false;
}

//��ȡ
bool CGameGoMemory::Read(TCHAR * szValueName, DWORD & dwOutVal)
{
	DWORD dwType=0,dwDest=0;
	DWORD dwSize=sizeof(DWORD);
	long int lReturn=RegQueryValueEx(m_hRegKey,szValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	if (lReturn==ERROR_SUCCESS)
	{
		dwOutVal=dwDest;
		return true;
	}
	return false;
}

//��ȡ
bool CGameGoMemory::Read(TCHAR * szValueName, int & iOutVal)
{
	DWORD dwType=0,dwDest=0;
	DWORD dwSize=sizeof(DWORD);
	long int lReturn=RegQueryValueEx(m_hRegKey,szValueName,NULL,&dwType,(BYTE *)&dwDest,&dwSize);
	if (lReturn==ERROR_SUCCESS)
	{
		iOutVal=(int)dwDest;
		return true;
	}
	return false;
}

//д��
bool CGameGoMemory::Write(TCHAR * szSubKey, TCHAR * szWriteVal)
{
	long int lReturn=RegSetValueEx(m_hRegKey,szSubKey,0L,REG_SZ,(const BYTE *)szWriteVal,lstrlen(szWriteVal)+1);
   	return (lReturn==ERROR_SUCCESS);
}

//д��
bool CGameGoMemory::Write(TCHAR * szSubKey, DWORD dwWriteVal)
{
	long int lReturn=RegSetValueEx(m_hRegKey,szSubKey,0L,REG_DWORD,(const BYTE *)&dwWriteVal,sizeof(DWORD));
   	return (lReturn==ERROR_SUCCESS);
}

//д��
bool CGameGoMemory::Write(TCHAR * szSubKey, int nWriteVal)
{
	DWORD dwValue=(DWORD)nWriteVal;
	long int lReturn=RegSetValueEx(m_hRegKey,szSubKey,0L,REG_DWORD,(const BYTE *)&dwValue,sizeof(DWORD));
   	return (lReturn==ERROR_SUCCESS);
}

