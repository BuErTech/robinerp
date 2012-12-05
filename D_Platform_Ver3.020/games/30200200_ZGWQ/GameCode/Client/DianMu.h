#pragma once
///#include "d:\20050422_510k\�ͻ������\�����.2003\afcdialog.h"

#include "Stdafx.h"
#include "Resource.h"

class CDianMu :
	public CGameFaceGo
{
	//��������
protected:
	int									m_iDestX,
										m_iDestY;

	CString								m_strMessage;
	
public:
	CNormalBitmapButtonEx				m_btOk;				//ȷ����ť
	CNormalBitmapButtonEx				m_btCancel;			//ȡ����ť
	CNormalBitmapButtonEx				m_btContinueChess;

	enum							{IDD = IDD_DIAN_MU};		//�Ի�������
	CWnd								*m_pParent;
	CGameImage							m_bk;

public:
	CDianMu(void);
	~CDianMu(void);
	virtual BOOL OnInitDialog();

	void SetMessage(TCHAR *szMessage, int iDestX = 50, int iDestY = 30);
		
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnHitContinueChess();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
