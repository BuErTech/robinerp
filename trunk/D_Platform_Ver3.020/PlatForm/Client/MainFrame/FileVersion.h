#ifndef __FILEVERSION_H_
#define __FILEVERSION_H_

//�ļ��汾�Ա���
class CFileVersion
{ 
	//��������
protected:
	BYTE					m_bBuffer[2000];				//������
    DWORD					m_dwLengCharset;				//��Ϣ����

	//��������
public: 
	//���캯��
    CFileVersion();
	//��������
    ~CFileVersion(); 

	//���ܺ���
public: 
	//���ļ�
    bool Open(LPCTSTR lpszModuleName);
	//�ر��ļ�
    void Close();
	//��ȡ�汾
    bool GetFixedFileVersion(DWORD & dwMSVer, DWORD & dwLSVer);
	//��ȡ�汾
    bool GetFixedProductVersion(DWORD & dwMSVer, DWORD & dwLSVer);
	//��ȡ�汾
	CString GetFixedFileVersion();
	//��ȡ�汾
	CString GetFixedProductVersion();
	//��������
    bool GetFixedInfo(VS_FIXEDFILEINFO & vsffi);
	//��������
    CString QueryValue(LPCTSTR lpszValueName, DWORD dwLengCharset=0);
	//��������
    CString GetFileDescription()  { return QueryValue(TEXT("FileDescription")); }
	//��������
    CString GetFileVersion() { return QueryValue(TEXT("FileVersion")); }
	//��������
    CString GetInternalName() { return QueryValue(TEXT("InternalName")); }
	//��������
    CString GetCompanyName() { return QueryValue(TEXT("CompanyName")); } 
	//��������
    CString GetLegalCopyright() { return QueryValue(TEXT("LegalCopyright")); }
	//��������
    CString GetOriginalFilename() { return QueryValue(TEXT("OriginalFilename")); }
	//��������
    CString GetProductName() { return QueryValue(TEXT("ProductName")); };
	//��������
    CString GetProductVersion() { return QueryValue(TEXT("ProductVersion")); }
}; 

#endif

