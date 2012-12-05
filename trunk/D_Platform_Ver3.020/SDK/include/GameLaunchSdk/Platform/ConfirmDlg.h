#pragma once
#ifndef AFCMESSAGEB0X_HEAD_H
#define AFCMESSAGEB0X_HEAD_H

#define IDQUXIAO                        2

#include "Stdafx.h"
#include "afcdevhead.h"
#include "Resource.h"
#include "SkinMgr.h"


enum MB_AFC_STYLE          //KT��Ϣ������
{
	AFC_CONFIRM = 500,       //ȷ�϶Ի���
	AFC_YESNO,               //��ʾ���ǡ������񡱶Ի���
	AFC_WARN                 //����Ի���
};

class _declspec(dllexport) ConfirmDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfirmDlg)

public:
	ConfirmDlg(CString strInfo,CString strTitle = "ϵͳ��ʾ",UINT style = AFC_CONFIRM,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ConfirmDlg();

	// �Ի�������
//	enum { IDD = IDD_CONFIRMDLG };
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
private:
	CSkinMgr	m_skinmgr;
};

#endif