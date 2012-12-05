#pragma once
#include "Stdafx.h"
#include "resource.h"
#include "afxwin.h"
#include "SkinMgr.h"
///@author yc.yan
///@version 1.0
///@date 2010.3.19
///

///
///CDlgRoomPW �Ի���
///
class CDlgRoomPW : public CDialog
{
	DECLARE_DYNAMIC(CDlgRoomPW)

public:
	CDlgRoomPW(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRoomPW();

// �Ի�������
	enum { IDD = IDD_GAME_ROOM_PW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	/// ��ͼ
	afx_msg void OnPaint();
	/// �˳�������Ӧ
	afx_msg void OnBnClickedQuit();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
private:
	/// ����Ƥ��
	void LoadSkin();
	/// ��ʼ������Ƥ��
	void InitBtnSkin();
public:
	TCHAR m_szPassWord[13]; ///<����
	TCHAR m_szPassWordConfirm[13];///<����
private:
	Bitmap              * m_pBtmBackGround; ///<����ͼƬ
	CNormalBitmapButtonEx m_BtnOK; ///<ȷ����ť
	CNormalBitmapButtonEx m_BtnCancel; ///<ȡ����ť
	CNormalBitmapButtonEx m_BtnQuit; ///<�˳���ť


	HBRUSH m_bkBrush;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	CSkinMgr m_skinmgr;

private:
	//����
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
};
