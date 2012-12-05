#pragma once

#ifndef FINDUSERDLG_HEAD_H
#define FINDUSERDLG_HEAD_H

#include "Afxcmn.h"
#include "UserListCtrl.h"
#include ".\\newcontrol\\tstatic.h"
#include "BitmapListCtrl.h"
#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" 
//�û����ҶԻ��� 
class CFindUserDlg : public CGameFaceGo
{
	//��������
protected:
	UINT							m_uComType;						//��Ϸ����
	CWnd							* m_pParentWnd;						//������

	HBRUSH m_bkBrush1, m_bkBrush2;

	//�ؼ�����
public:
	CTrueEdit						m_UserName;							//�û�����
//	CTrueEdit						m_UserAcc;							//�û� ACC
	CNormalBitmapButtonEx			m_BtFind;							//���Ұ�ť
	CNormalBitmapButtonEx			m_BtCancel;							//�رհ�ť
	CNormalBitmapButtonEx			m_BtClose;
	CUserListCtrl					m_UserList;	//�û��б�
	CTransparentStatic				m_static1;
	CGameImage						m_bk;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
	//��������
public:
	//���캯��
	CFindUserDlg(CWnd * pParentWnd, UINT uComType);
	//��������
	virtual ~CFindUserDlg();
	//�����û���Ϣ
	void UpdateUserItem(UserItemStruct * pUserItem);
	//ɾ���û���Ϣ
	void DeleteUserItem(UserItemStruct * pUserItem);

	//���غ���
public:
	//���ݰ�
	virtual void DoDataExchange(CDataExchange * pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ��
	virtual void OnOK();
	//ȡ��
	virtual void OnCancel() { DestroyWindow(); } 

	//��Ϣ����
protected:
	//��������Ҽ�
	afx_msg void OnNMRclickList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	CSkinMgr m_skinmgr;

	void Init();

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara); 
	  // ���ô���Բ�� 2012.10.17 yyf
	  void SetWndRgn(void);
};
#endif