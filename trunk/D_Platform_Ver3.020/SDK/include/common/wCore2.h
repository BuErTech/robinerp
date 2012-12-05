#pragma  once

// CClientSocket ����Ŀ��
typedef struct
{
	long	CustomID;
	long	ServerID;
	long	CustomServerTime;
}_TAG_LICENSEREQUEST;

typedef struct
{
	long	CustomID;
	long	ServerID;
	long	CustomServerTime;
	long	LicenseServerTime;
	long	ExpiresTime;
	short	Expiresed;
	short	Invalidated;
}_TAG_LICENSERESPONSE;

#define WM_LICENSE_RESPONSE	WM_USER+100

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	HWND		m_Hpwnd;
public:
	int iCount;
public:
	virtual void OnReceive(int nErrorCode);
};
class CCore : public CWnd
{
	DECLARE_DYNAMIC(CCore)

public:
	CCore();
	virtual ~CCore();

	CWnd *	pWnd;				//������
	UINT	msg;				//���������ڵ���Ϣ
	CString m_currentPath;		//��ǰĿ¼

	int		m_licenseCountIntime;	//ÿ����֤�ĵڼ���

	int		m_licenseServerCount;	//��֤����������
	CString m_licenseServer[10];	//��֤��������ַ
	short	m_licensePort[10];		//��֤�������˿�
	long	m_customID[10];			//CustomID
	long	m_serverID[10];			//serverID


	CClientSocket	m_socket;		//socket

	_TAG_LICENSEREQUEST reqData;	//��֤����ṹ

	CFile	logFile;				//д��־�ļ�

protected:
	DECLARE_MESSAGE_MAP()
public:
	void StartLicense(void);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void Initialize(void);
public:
	void NetLicense(void);
public:
	void StopService(int errCode);
	CString GetIniString(CString szApp, CString szKey, CString szDefault, CString szIniFile=TEXT(""));
	int GetIniLong(CString szApp, CString szKey, long lDefault, CString szIniFile=TEXT(""));
public:
	LRESULT OnLicenseResponse(WPARAM wParam, LPARAM lParam);
public:
	void LoadProfile(void);
};

bool coreProcess(LPCSTR szMsg);

bool coreInitialize(CWnd *pWnd,UINT msg);

CString coreGetCode();

CString coreGetString();

long coreGetLicenseExpires();

long coreGetTimeStamp();

long coreGetGameLicense(long gameID);

CString coreGetGameLicense(CString gameID);