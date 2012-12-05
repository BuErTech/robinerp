#pragma once


// CCounter

class CCounter : public CWnd
{
	DECLARE_DYNAMIC(CCounter)

public:
	CCounter();
	virtual ~CCounter();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLookClose();

	afx_msg void OnLeaveNum() ; 
	afx_msg void OnHaveNum() ; 

	void  DrawHistoryInfo(CDC *pDC);    
	void  ReSetDlg();

public:
	CGameImage			          m_bglook;
	CNormalBitmapButtonEx         m_btnLookclose;  //�رհ�ť
	CNormalBitmapButtonEx         m_btnleaveNum;  //�رհ�ť
	CNormalBitmapButtonEx         m_btnHaveNum;  //�رհ�ť
		
	BOOL                          m_bShowLeaveNum;  //�Ƿ���ʾʣ������
	
	int                           m_iHistoryCard[20] ; //��¼ÿ���Ƴ�������
	
};


