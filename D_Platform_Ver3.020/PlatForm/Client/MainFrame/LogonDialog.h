#pragma once

#ifndef LOGONDLG_HEAD_H
#define LOGONDLG_HEAD_H

#include "StdAfx.h"
#include <vector>

#include "GamePlace.h"
#include "LogoSelect.h"
#include "RegisterNewUserDlg.h"
#include "NewControl\\TStatic.h"
#include "SoftKey.h"
#include "RegSucDlg.h"
#include "SkinMgr.h"

//��½��ʽ
#define LK_BY_NAME						0									//���ֵ�½
#define LK_BY_ACC						1									//ACC ��½
#define LK_BY_MOBILE					2									//�ֻ���½
#define LK_BY_ID						3									//ID��½


class CLogonDialog : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CLogonDialog();
	virtual ~CLogonDialog(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void InitSkin();

	void Notify(BzDui::TNotifyUI& msg);

	void CenterWindow();

public:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void OnOK();

public:

	static DWORD __stdcall ThreadDownConnFile(LPVOID param);

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

private:
	//�Ƿ�ͬ������
	bool IsApproveAgreement();

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

//�ڲ�����
private:

	//��ȡע�����Ϣ
	bool LoadRegisterData();

	//���õ�½ģʽ
	bool SetLogonType(BYTE bNewLogonType, BYTE bOldLogonType);

	//��������
	bool UpdatePassWord(CString strInputInfo);

	//��д�û�
	static bool FillUserName(BzDui::CComboUI* pComBoxName, BzDui::CEditUI* pEditName = NULL);

	//��д��½������
	static bool FillServer(BzDui::CComboUI* pComBoxName);

	//��ȡ������Ϣ
	static CString GetUserPass(const TCHAR* szKindInfo, UINT uType);

	//��ȡ��½ʱ����Ϣ
	static DWORD GetUserLogonTime(const TCHAR* szKindInfo, UINT uType);

//���ܺ���
public:

	//д��������Ϣ
	static bool WriteUserPass(const TCHAR* szKindInfo, TCHAR* szPass, UINT uType);

	//д���½ʱ����Ϣ
	static bool WriteUserLogonTime(const TCHAR* szKindInfo, UINT uType);

	//��������
	static CString EncryptPass(const TCHAR* szPass);

	//�⿪����
	static CString CrevassePass(const TCHAR* szEncryPass);

//��Ϣ����
public:

	//�������û�
	void OnBnClickedRegUser();

	//���߰���
	void OnBnClickedHelp();

	//QQ���߰���
	void OnBnClickedQQHelp();

	//ʹ�ô��������
	//afx_msg void OnBnClickedUseProxy();

	//���ô����Ƿ�ʹ�ð�ȫ��֤
	//afx_msg void OnBnClickedUsePass();

	//�ʺŵ�½
	void OnBnClickedAccounts();

	//ACC ��½
	//afx_msg void OnBnClickedAccID();

	//�ֻ���½
	//afx_msg void OnBnClickedMobile();
	//ѡ��ı�

	//ɾ����ť
	void OnBnClickedDelete();

	//�������������빦�ܰ�ť
	void OnBnClickedFindPassword();

	//�鿴��������
	void OnBnClickedTermOfService();

	//����ͷ��
	void OnBnClickedSelectLogo();

	//���ְ���
	void OnBnClickedNewHelp();

	//����ͷ��
	void OnBnClickedApplyPassword();
	void OnBnClickedSoftkey();

	//�û�����¼
	void OnBnClickedLogonByName();
	//�û�ID��¼
	void OnBnClickedLogonByID();

#ifdef AI_MACHINE
	void AILogin();
#endif

public:

	BzDui::CPaintManagerUI m_pm;

	bool								bMouseDown;
	CPoint								savePoint;
	HBRUSH								m_bkBrush;
	//��������
public:
	//�û���Ϣ
	static bool							m_bInit;							//��ʼ����־
	static BYTE							m_bRemberPass;						//��ס����
	static BYTE							m_bUseProxy;						//ʹ�ô���
	static BYTE							m_bRegUser;							//ע���û�
	static BYTE							m_bLogonType;						//��½ģʽ
	static BYTE							m_bBoy;								//�Ƿ��к�
	static UINT							m_bLogoID;							//ͷ�� ID
	static int							m_iUserID;						//�û�ID
	static TCHAR						m_szName[61];						//�û�����
	static TCHAR						m_szScrPass[61];					//�û�ԭ����
	static char							m_szMD5Pass[50];					//���ܺ������
	static char							m_szRegSQL[210];					//",email='123'"//�û�ע�ḽ����Ϣ��SQL
	static int							m_dwAccID;							//ACC ����
	static int							m_dwMobile;							//�ֻ�����
	static TCHAR						m_szIDCard[64];						//֤�����
	static TCHAR						m_szMobileVCode[8];					//�ֻ���֤��
	static TCHAR						m_nickname[100];
	static TCHAR						m_szRecommenderName[61];
	//������Ϣ
	static BYTE							m_bSock5;							//SOCK 5 ��־
	static BYTE							m_bUserCheck;						//��Ҫ����
	static UINT							m_uProxyPort;						//����˿�
	static TCHAR						m_szGameServer[51];					//��Ϸ IP
	static TCHAR						m_szProxyServer[51];				//���� IP
	static TCHAR						m_szProxyName[61];					//��������
	static TCHAR						m_szProxyPass[61];					//��������

	static bool							m_bIsShow;
	static bool							m_bUpdatedConnFile;					//�Ƿ��Ѹ��·������б�

	int									logadvx;							//IE���x
	int									logadvy;							//IE���y
	int									logadvw;							//IE�����
	int									logadvh;							//IE���߶�
	CString								szLogAdvUrl;						//IE����ַ������Ϊ��ʱ��ʾû��

	bool								m_bisgetid;
	bool								m_bshow;
	TCHAR m_szUrl[300];
	TCHAR m_szcpUrl[300];
	TCHAR m_szerrUrl[300];
	TCHAR m_code[100];
	CString m_regurl;

//����ؼ�
public:

	//Serverѡ���
	BzDui::CComboUI* m_pServerCob;

	//ID�����
	BzDui::CComboUI* m_pUserIDCob;
	BzDui::CEditUI* m_pIDEdit;

	BzDui::CControlUI* m_pBkImgae;
	BzDui::CButtonUI* m_pThreetName;
	BzDui::CButtonUI* m_pThreetID;

	//���������
	BzDui::CEditUI* m_pPwdEdit;

	//��ס����ѡ���
	BzDui::COptionUI* m_pSavePwd;

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;

	//�Ƿ�֧��ID��¼
	bool m_bLoginByID;

private:

	//�������б���
	struct TSvrNode
	{
		char szLoginIP[32];
		char szName[64];
		TSvrNode(){::memset(this,0,sizeof(TSvrNode));}
	};

	//�������б�
	static std::vector<TSvrNode> m_vtServer;

private:
	CSkinMgr m_skinmgr;
	
	void ChangeSkin();
};





#endif
/*******************************************************************************************************/