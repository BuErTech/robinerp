#pragma once
#ifndef LOGONDIALOG_HEAD_H
#define LOGONDIALOG_HEAD_H

#include "Stdafx.h"
#include "GamePlace.h"
#include "LogoSelect.h"
#include "RegisterNewUserDlg.h"
#include "NewControl\\TStatic.h"
#include "SoftKey.h"
//#include "NewControl\\TStatic.h"
//#include "NewControl\\MacButtons.h"
//#include "NewControl\\BtnST.h"

/*******************************************************************************************************/

//��½��ʽ
#define LK_BY_NAME						0									//���ֵ�½
#define LK_BY_ACC						1									//ACC ��½
#define LK_BY_MOBILE					2									//�ֻ���½

//��½�Ի�����
class CLongonDialog : public CLoadFileBmp
{
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
	static TCHAR						m_szName[61];						//�û�����
	static TCHAR						m_szScrPass[61];					//�û�ԭ����
	static char							m_szMD5Pass[50];					//���ܺ������
	static char							m_szRegSQL[210];					//",email='123'"//�û�ע�ḽ����Ϣ��SQL
	static int							m_dwAccID;							//ACC ����
	static int							m_dwMobile;							//�ֻ�����
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

	int									logadvx;							//IE���x
	int									logadvy;							//IE���y
	int									logadvw;							//IE�����
	int									logadvh;							//IE���߶�
	CString								szLogAdvUrl;						//IE����ַ������Ϊ��ʱ��ʾû��

	CSoftKey						*	hSoftKey;

	bool								m_bisgetid;
	bool								m_bshow;
	TCHAR m_szUrl[300];
	TCHAR m_szcpUrl[300];
	TCHAR m_szerrUrl[300];
	TCHAR m_code[100];
	CString m_regurl;
	//������Ϣ
	UINT								m_uMaxHide;							//���߶�
	UINT								m_uMaxWidth;						//�����
	CRect								m_NormalWindowRect;					//��������
	
	//�ؼ�����
public:
	CTrueEdit							m_PassWord;							//�����
	CLogoSelect							m_LogoSelect;						//ͷ��ѡ��
	CNormalBitmapButtonEx				m_btQuit;							//�˳���ť
	CNormalBitmapButtonEx				m_btLogon;							//��½��ť
	CNormalBitmapButtonEx				m_btRegister;						//���밴ť
	//CNormalBitmapButtonEx				m_btUpdateList;						//�����б�
	CNormalBitmapButtonEx				m_btDeleteName;						//ɾ������
	CNormalBitmapButtonEx				m_btLogoSelect;						//ͷ��ѡ��
	CNormalBitmapButtonEx				m_btFindPassword;						//ͷ��ѡ��
	CNormalBitmapButtonEx				m_btApplyPassword;						//ͷ��ѡ��
	CNormalBitmapButtonEx				m_btNewHelp;						//���ְ���

	CNormalBitmapButtonEx				m_btSoftkey;

	CToolTipCtrl						m_ToolTip;							//������ʾ
	CIEClass							m_Web;
	//CTransparentStatic					m_static_1;
	CTransparentStatic					m_static_2;
	CTransparentStatic					m_static_3;
	CTransparentStatic					m_static_Name;
	CTransparentStatic					m_state10;
	CTransparentStatic					m_state5;
	CButton							m_check_1;
	//CButton							m_check_2;
	CButton							m_check_2;
	CHyperLink						m_copyright;
	CGameImage						m_bkimage;
	COLORREF						m_bkcolor;
	//��������	
public:
	//���캯��
	CLongonDialog();   
	//��������
	virtual ~CLongonDialog();
	//��ʼ������
	virtual BOOL OnInitDialog();
	HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor);
protected:
	//ȷ��
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange * pDX);    
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	DECLARE_MESSAGE_MAP()

	//�ڲ�����
private:
	//��ȡע�����Ϣ
	bool LoadRegisterData(); //OK
	//���õ�½ģʽ
	bool SetLogonType(BYTE bNewLogonType, BYTE bOldLogonType); //OK HALF
	//��������
	bool UpdatePassWord(CString strInputInfo); //OK
	//��д�û�
	static bool FillUserName(CComboBox * pComBoxName);  //OK
	//��д��½������
	static bool FillServer(CComboBox * pComBoxServer); //OK
	//��ȡ������Ϣ
	static CString GetUserPass(const TCHAR * szKindInfo, UINT uType); //OK

	//���ܺ���
public:
	//д��������Ϣ
	static bool WriteUserPass(const TCHAR * szKindInfo, TCHAR * szPass, UINT uType); //OK
	//��������
	static CString EncryptPass(const TCHAR * szPass); //OK
	//�⿪����
	static CString CrevassePass(const TCHAR * szEncryPass); //OK

	//��Ϣ����
public:
	//������Ϣ
	LRESULT OnHitLogoMessage(WPARAM wparam, LPARAM lparam);
	//�������û�
	afx_msg void OnBnClickedRegUser();
	//ʹ�ô��������
	//afx_msg void OnBnClickedUseProxy();
	////���ô����Ƿ�ʹ�ð�ȫ��֤
	//afx_msg void OnBnClickedUsePass();
	//�ʺŵ�½
	afx_msg void OnBnClickedAccounts();
	////ACC ��½
	//afx_msg void OnBnClickedAccID();
	////�ֻ���½
	//afx_msg void OnBnClickedMobile();
	//ѡ��ı�
	afx_msg void OnCbnSelchangeInput();
	//ɾ����ť
	afx_msg void OnBnClickedDelete();
	//����ͷ��
	afx_msg void OnBnClickedSelectLogo();
	//ɾ����ť
	afx_msg void OnBnClickedFindPassword();

	//���ְ���
	afx_msg void OnBnClickedNewHelp();
	
	//����ͷ��
	afx_msg void OnBnClickedApplyPassword();
	afx_msg void OnBnClickedSoftkey();
	//
	afx_msg void OnPaint();
protected:
	virtual void OnCancel();
//public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void LoadSkin(void);
#ifdef AI_MACHINE
	void AILogin();
#endif
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#endif
/*******************************************************************************************************/
