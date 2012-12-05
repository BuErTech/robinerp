#pragma once
#ifndef QUEUEINROOM_HEAD_H
#define QUEUEINROOM_HEAD_H

#include "Stdafx.h"
#include "SkinMgr.h"

///�Ŷӻ��Ի�����
class CQueueInRoomDlg: public CLoadFileBmp
{
public:
	TCHAR								m_szShowMsg[30];					///<��ʾ��Ϣ
	CGameImage							m_bk;								///<����
	//Image *								m_bk;
	CNormalBitmapButtonEx				m_BtQueue;							///<�ŶӰ�ť			
	CNormalBitmapButtonEx				m_BtNoQueue;						///<���ŶӰ�ť
	CNormalBitmapButtonEx				m_BtExit;							///<�˳�
	COLORREF							m_MsgColor;							///<��ʾ��Ϣ��ɫ
	CFont								m_Font;								///<����
	RECT								m_rc;								///<��ʾ��Ϣ������
	int									m_iIndex;							///<��������
	UINT								m_uRoomID;							///<����ID



public:
	//����
	CQueueInRoomDlg();
	//����
	~CQueueInRoomDlg();
	///������Ϣ��ʾ
	bool SetMessageText(const TCHAR * szMessage);
	///���ٶԻ���
	void DestroyDlg();

public:
	virtual BOOL OnInitDialog();
	///ȷ��
	virtual void OnOK() { return; }
	///ȡ��
	virtual void OnCancel() { return; }
	
public:
	///��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	///�ŶӴ���
	afx_msg void OnQueue();
	///���ŶӴ���
	afx_msg void OnNoQueue();
	///�˳�����
	afx_msg void OnExit();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);


protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
	CSkinMgr m_skinmgr;

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

private:
	void ChangeSkin();
};

#endif