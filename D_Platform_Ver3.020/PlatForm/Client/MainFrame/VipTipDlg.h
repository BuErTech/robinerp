#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" 
// CVipTipDlg �Ի���

class CVipTipDlg : public CGameFaceGo
{
	DECLARE_DYNAMIC(CVipTipDlg)

public:
	CVipTipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVipTipDlg();

// �Ի�������
	enum { IDD = IDD_VIPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//�������� VIP
	afx_msg void OnBnClickedOk();
	//ȡ��
	afx_msg void OnBnClickedQuxiao();
public:
	//�ر�
	afx_msg void OnBnClickedClosebtn();
public:
	 CGameImage  m_backPic;     //����ͼ
	 CString     m_titelStr;    //����
	 CRect       m_rectInfoPic; //��ʾͼƬ��ʾ����
    CRect       m_rectTitleText;   //������������

	CNormalBitmapButtonEx m_buyBtn;
	CNormalBitmapButtonEx m_cancelBtn;
	CNormalBitmapButtonEx m_closeBtn;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	CSkinMgr m_skinmgr;

	void LoadSkin();
public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

	  // ���ô���Բ�� 2012.10.15 yyf
	  void SetWndRgn(void);
};
