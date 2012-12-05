#pragma once
#include "afxwin.h"
#include "StdAfx.h"
#include "resource.h"
#include <list>
#include "SkinMgr.h"
#include "afxcmn.h"
#include "..\UILibrary2003\CconfigDlg.h" 
using namespace std;
/********************************************************************
	created:	2009/03/04
	created:	4:3:2009   16:51
	filename: 	�ͻ������\ϵͳ�����\AppealDlg.h
	file path:	�ͻ������\ϵͳ�����
	file base:	AppealDlg
	file ext:	h
	author:		yjj
	purpose:	Ͷ�߿�Ľ������

    Copyright szbzw 2009 - All Rights Reserved

*********************************************************************/
// CAppealDlg �Ի���

//yjj 090304 add this dlg for appeal

//����ftp�ϴ�����
typedef void (WINAPI *PFCALLBACK)(CString sFileName,long nFileSize);
__declspec(dllimport) int DownloadFile(CStringArray *csa,HWND pWnd,UINT message);
__declspec(dllimport) int UploadFile(CStringArray *csa,HWND pWnd,UINT message);
__declspec(dllimport) void FTPCancel();
__declspec(dllimport) void WINAPI GetServerList(CStringArray *csa,PFCALLBACK Func);


struct RecordFileStruct 
{
	CString strWholeName;   //��Ϸ��¼���ļ�����������
	CString strFileName;
	CString strGameName;    //��Ϸ����
	//wushuqun 2009.5.20
	//����·��
	CString strGamePath;

	CString strGameTime;    //��Ϸʱ��
	CTime   timeCreatTime;  //¼���ļ��Ĵ���ʱ��

	//��Ϊ����list �Զ����������У�����˶���
	bool operator>(RecordFileStruct& comp)
	{
		if (timeCreatTime >= comp.timeCreatTime)
		{
			return false;
		}
		return true;
	}
	bool operator<(RecordFileStruct& comp)
	{
		if (timeCreatTime <= comp.timeCreatTime)
		{
			return false;
		}
		return true;
	}
	bool operator==(RecordFileStruct& comp)
	{
		if (timeCreatTime != comp.timeCreatTime)
		{
			return false;
		}
		return true;
	}
};
class CAppealDlg : public CDialog
{
	DECLARE_DYNAMIC(CAppealDlg)

public:
	explicit CAppealDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAppealDlg();

// �Ի�������
	enum { IDD = IDD_DLG_APPEAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	//ͨ����ѡ���bcf logĿ¼�б���һ��bcf�ļ�
	static void WriteGameInfoFile(TCHAR* pstGameName,UINT uNameId);

	void OpenAppealDlg(TCHAR* pUserName);

protected:
	//��¼���ļ���ʱ������Ȼ��Ӧ���ؼ���  yjj 090304
	void ProcessRecordFile(const CString& strUserName);
	//��Ҫһ���ļ���ʹ��id����Ϸ��һһ��Ӧ
	CString GetGameName(DWORD dwHandle,const CString& uGameId);

	//�����ļ����õ�nameid
	CString GetFileNameID(const CString& strFileName);
	//�����ļ������õ��ļ�����ʱ��
	//CString GetFileTime(const CString& strFileName);



	
private:

	CNormalBitmapButtonEx m_btnCancle;
	CNormalBitmapButtonEx m_btnOk;
	CGameImage            m_imgDlgBk;
	CNormalBitmapButtonEx m_bnClose;


	CString   m_strFtpAddress;
	CString   m_strFtpUserName;
	CString   m_strFtpPwd;


    list<RecordFileStruct> m_fileList;

	CString m_strUserName;//����û���

	CListCtrl m_FileListCtrl;

	CString m_strInfoFileWholeName;
	CString m_strInfoFileName;

	//�û��б�
	//CListBox m_ListBox;
	afx_msg void OnPaint();
	
	bool m_bUpLoad;
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

private:
	void Init();
	//����
	void LoadSkin();
private:
	CSkinMgr m_skinmgr;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.15 yyf
public:
	  // ���ô���Բ�� 2012.10.15 yyf
	  void SetWndRgn(void);
};
