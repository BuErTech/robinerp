#pragma once
//#include "e:\20050611_pmbgame\newgame\�ͻ������\�����.2003\afcdialog.h"

#include "Stdafx.h"
#include "Resource.h"

class CAdmitDefeat :
	public CGameFaceGo
{

	public:
	CNormalBitmapButtonEx					m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx					m_btCancel;						//ȡ����ť
	enum					{ IDD = IDD_REN_SHU };			//�Ի�������
	CWnd *m_pParnet;
	CGameImage							m_bk;
public:
	CAdmitDefeat(void);
	~CAdmitDefeat(void);
protected:
//	virtual void OnOK();
//	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	virtual void OnOK();
public:
	virtual BOOL ShowWindow(int nCmdShow);
};
