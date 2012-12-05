#ifndef CAFCREGISTRY_HEAD_FILE
#define CAFCREGISTRY_HEAD_FILE

#include "PublicDefine.h"

//ע����д��
class EXT_CLASS CGameGoMemory//CGameGoMemory
{
	//��������
protected:
	HKEY					m_hRegKey;					//ע����ֵ

	//��������
public:
	//���캯��
	CGameGoMemory(HKEY hRegKey=HKEY_LOCAL_MACHINE);
	//��������
	virtual ~CGameGoMemory(void);

	//���ܺ���
public:
	//��
	bool Open(TCHAR * szSubKey);
	//�ر�
	void Close();
	//�����Ӽ�
	bool CreateKey(TCHAR * szSubKey);
	//ɾ��
	bool DeleteKey(HKEY hRegKey, TCHAR * szSubKey);
	//ɾ��
	bool DeleteValue(TCHAR * szValueName);
	//����
	bool SaveKey(TCHAR * szFileName);
	//�ָ�
	bool RestoreKey(TCHAR * szFileName);
	//��ȡ
	bool Read(TCHAR * szValueName, CString & strOutVal);
	//��ȡ
	bool Read(TCHAR * szValueName, TCHAR * szOutVal, UINT uBufferSize);
	//��ȡ
	bool Read(TCHAR * szValueName, DWORD & dwOutVal);
	//��ȡ
	bool Read(TCHAR * szValueName, int & iOutVal);
	//д��
	bool Write(TCHAR * szSubKey, TCHAR * szWriteVal);
	//д��
	bool Write(TCHAR * szSubKey, DWORD dwWriteVal);
	//д��
	bool Write(TCHAR * szSubKey, int nWriteVal);
};

#endif
