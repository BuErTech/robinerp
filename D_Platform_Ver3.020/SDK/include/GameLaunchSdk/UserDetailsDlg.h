#pragma once


// CUserDetailsDlg �Ի���

class CUserDetailsDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserDetailsDlg)

public:
	CUserDetailsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserDetailsDlg();

// �Ի�������
	//enum { IDD = IDD_USER_DETAILS };

private:
	CNormalBitmapButtonEx  m_btClose;
	CGameImage  m_BackBitmap;
	UserItemStruct * m_pUserItem;
	WCHAR		m_wszBuffer[MAX_PATH];			// ���ַ����飨�滭png��
	COLORREF    m_clrLabel, m_clrText;
	CString     m_strMoneyType;                 // ��������
	CString     m_strCombatGainsFormat;         // ս�������ʽ
	TCHAR       szFileName[MAX_PATH];
	int			m_isShow;                       //�Ƿ���ʾս��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	void SetUserItem(UserItemStruct * pUserItem);
	void OnCancel();

public:
	UINT m_uGameNameID;
	UINT m_uComType;

	CSkinMgr	m_skinmgr;

public:
	void LoadSkin();
};
