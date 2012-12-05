#pragma once
#include "afxwin.h"
#include "SkinMgr.h"
#include "resource.h"
// CPassDesk �Ի���

class CPassDesk : public CDialog
{
	DECLARE_DYNAMIC(CPassDesk)

public:
	CPassDesk(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassDesk();
	CGameImage							m_bk;				//����


// �Ի�������
	enum { IDD = IDD_PASS_DESK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_pass;
public:
	afx_msg void OnPaint();
public:
	virtual BOOL OnInitDialog();
public:
	CNormalBitmapButtonEx m_BtOk;
public:
	CNormalBitmapButtonEx m_BtCancel;

private:
	CSkinMgr m_skinmgr;

	void LoadSkin();
public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
};
