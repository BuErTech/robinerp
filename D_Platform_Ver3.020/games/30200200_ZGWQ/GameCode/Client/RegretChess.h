#pragma once
///#include "e:\20050611_pmbgame\newgame\�ͻ������\�����.2003\afcdialog.h"

#include "Stdafx.h"
#include "Resource.h"

class CRegretChess :
	public CGameFaceGo
{
public:
	CNormalBitmapButtonEx				m_btOk;							//ͬ�ⰴť
	CNormalBitmapButtonEx				m_btCancel;						//��ͬ�ⰴť
	enum					{ IDD = IDD_DLG_HUI_QI };			//�Ի�������
	CWnd *m_pParnet;
	CGameImage							m_bk;
public:
	CRegretChess(void);
	~CRegretChess(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
