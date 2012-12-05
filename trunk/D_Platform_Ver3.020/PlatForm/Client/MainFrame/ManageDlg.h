#pragma once

#ifndef MANAGEDLG_HEAD_H
#define MANAGEDLG_HEAD_H

#include "StdAfx.h"
#include "Resource.h"
#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" 
class CGameRoomEx;

/******************************************************************************************************************/

//���Ȩ�޿�����
class CManageDlg : public CGameFaceGo//CAFCBaseDialog
{
	//��������
protected:
	long int						m_dwUserID;							//�û� ID
	GameInfoStruct					* m_pGameInfo;						//��Ϸ��Ϣ

	HBRUSH m_bkBrush;

	//�ؼ�����
public:
//	CAFCPicButton					m_BtOK;								//Ӧ�ð�ť
	CNormalBitmapButtonEx					m_BtOK;								//Ӧ�ð�ť
	CNormalBitmapButtonEx					m_BtCancel;							//ȡ����ť
	CGameImage						m_bk;
	enum						{ IDD = IDD_MANAGE_DLG };
	
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
	DECLARE_MESSAGE_MAP()

	//��������
public:
	//���캯��
	CManageDlg(CWnd * pParent, GameInfoStruct * pGameInfo);
	//��������
	virtual ~CManageDlg();

	//���غ���
protected:
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange * pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();

	//���ܺ���
public:
	//���ù����û�
	void SetManageUser(UserItemStruct * pUserItem);
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnPaint();

private:
	CSkinMgr m_skinmgr;

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara); 
	  // ���ô���Բ�� 2012.10.15 yyf
	  void SetWndRgn(void);
};
#endif

/******************************************************************************************************************/
