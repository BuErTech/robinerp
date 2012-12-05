#pragma once
///#include "e:\20050611_pmbgame\newgame\�ͻ������\�����.2003\afcdialog.h"

#include "Stdafx.h"
#include "Resource.h"

class CChessManual :
	public CGameFaceGo
{
public:
	CChessManual(void);
	~CChessManual(void);

public:
	CButton				m_btOk;							///< ȷ����ť
	CButton				m_btCancel;						///< ȡ����ť
	enum					{ IDD = IDD_DLG_QI_PU };	///< �Ի�������
	CWnd *m_pParnet;

public:
	void SetQiPu(int iStartI, int iStartJ, int iEndI, int iEndJ, int iQiZi, int type, int iMeFlag);	///< ��������
	void ClearAll();
	void UpdateInfo();
private:
	void GetCharSet(int iOrder ,CString &str);

	int m_iBuCount;		///< �߲�����
	int m_iFlag;
	CString m_szString;

public:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	CString m_strQiPu;
};

