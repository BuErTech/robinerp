#pragma once
#ifndef AFCMESSAGEB0X_HEAD_H
#define AFCMESSAGEB0X_HEAD_H

#define IDQUXIAO                        2

#include "Stdafx.h"
#include "afcdevhead.h"
#include "Resource.h"
#include "CconfigDlg.h"


enum MB_AFC_STYLE          //KT��Ϣ������
{
    AFC_CONFIRM = 500,       //ȷ�϶Ի���
    AFC_YESNO,               //��ʾ���ǡ������񡱶Ի���
    AFC_WARN                 //����Ի���
};

class _declspec(dllexport) ConfirmDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfirmDlg)

	HBRUSH	m_bkBrush;		///< ���ڱ���ɫ��ˢ��
public:
	ConfirmDlg(CString strInfo,CString strTitle = "ϵͳ��ʾ",UINT style = AFC_CONFIRM,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ConfirmDlg();

// �Ի�������
	enum { IDD = IDD_CONFIRMDLG };
	DWORD m_wuiHandle;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    //CString					m_backPicFile;
    CString					m_infoBmpFile;
    CBitmap					m_infoBmp;     //��ʾͼƬ
    CRect					m_rectTitle;     //����������
    CNormalBitmapButtonEx   m_confirmBtn;
    CNormalBitmapButtonEx   m_cancelBtn;
    CNormalBitmapButtonEx   m_closeBtn;

    CString     m_infoStr;     //��ʾ��Ϣ
    CString     m_titelStr;    //����
    UINT        m_msgBoxStyle; //��Ϣ����ʽ��ΪMB_AFC_STYLEֵ
    CGameImage  m_backPic;     //����ͼ
    CRect       m_rectInfoPic; //��ʾͼƬ��ʾ����
    CRect       m_rectTitleText;   //������������

	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
    CFont m_fontCaption;     //����������
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedNo();
    afx_msg void OnBnClickedClosebtn();

public:
	static void SetSkinKey(const CString& strkey); 
	  // ���ô���Բ�� 2012.10.15 yyf
	  void SetWndRgn(void);
private:
	static CString m_skinKey;
};


class TimerDlg :
	public ConfirmDlg
{
private:
	int m_sec;
public:
	TimerDlg(int sec, CString strInfo,CString strTitle = "ϵͳ��ʾ",UINT style = AFC_CONFIRM,CWnd* pParent = NULL);
public:
	~TimerDlg(void);
public:
	virtual BOOL OnInitDialog();
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void OnOK();
};


#endif