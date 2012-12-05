#pragma once
#ifndef SHORTMESSAGE_HEAD_H
#define  SHORTMESSAGE_HEAD_H


//#include "StdAfx.h"
#include "Resource.h"
#include "Afxcmn.h"
#include "clientcomstruct.h"
#include "AFCImage.h"
#include "afcbutton.h"
#include "messagehandle.h"
#include "afcdialog.h"

//class CGameRoomEx;

#define  WM_GM_TALK       WM_USER + 1113 

class SendMsgUserStruct
{
public:
	SendMsgUserStruct()
	{
		ZeroMemory(this, sizeof(SendMsgUserStruct));
	}

public:
	BYTE						bDeskNO;							///��Ϸ����
	BYTE						bGameMaster;						///����ȼ�
	long int					dwUserID;							///ID ����
	int							dwAccID;							///ACC ����
	char						nickName[61];						///�û��ǳ�
};

//���Ͷ���Ϣ��
class EXT_CLASS CShortMessage : public CGameFaceGo
{
	HBRUSH m_bkBrush;

	//��������
public:
	CWnd								* m_pParentWnd;						//������
		
	//����״̬��Ϣ
	long int							m_dwLastTalkTime;					//˵��ʱ��
	CString								m_strLastTalk;						//��������

	//��Ϣ����
	long int							m_dwTargetUserID;					//�Ի��û�
	int									m_dwTargerAccID;					//ACC ����
	CString								m_strTargerName;					//�û�����
	CString								m_strRoomName;						//��������
	SendMsgUserStruct					m_TargetUserItem;				//Ŀ���û�
	bool								m_bIsInRoom;						//�������������ڻ�����Ϸ����������
	long int							m_dwGamePower;
	//�ؼ�����
public:
	CEdit								m_SendMessage;						//���ʹ���
	CToolTipCtrl						m_ToolTip;							//������ʾ
	CNewMiniDlg							m_RecvMessage;						//���ܴ���
	CNormalBitmapButtonEx				m_BtSend;							//���Ͱ�ť
	CNormalBitmapButtonEx				m_BtExpression;						//���鰴ť
	///CNormalBitmapButtonEx			m_BtHistory;						//��ʷ��ť
	CNormalBitmapButtonEx				m_BtMin;							//�˳���ť
	CNormalBitmapButtonEx				m_BtClose;							//�˳���ť
	CNormalBitmapButtonEx				m_BtClose2;							//�˳���ť
	CLookingClass						m_MessageHandle;					//��Ϣ����
	CGameTalkText						m_ExpressWnd;						//���鴰��
	CGameImage							m_bk;
	CAFCColorResource					m_colorresource;
	DECLARE_MESSAGE_MAP()

	//��������
public:
	//���캯��
	CShortMessage(CWnd * pParentWnd);
	//��������
	virtual ~CShortMessage();
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
protected:
	//�ؼ��󶨺���
	virtual void DoDataExchange(CDataExchange * pDX); 
	//ȷ����ť
	virtual void OnOK();
	//ȡ����ť
	virtual void OnCancel();
	//��Ϣ���ͺ���
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ܺ��� 
public:
	//������Ϣ
	void SetTalkInfo(SendMsgUserStruct * pTargerUserItem, long int dwGamePower, TCHAR * szRoomName);

	int m_iGmID;
	void SetTalkInfo(int iGmID);
	//����λ��
	void FixControlSize(int iWidth, int iHegiht);
	//�յ���Ϣ
	void OnRecvMessage(TCHAR * szSendName, TCHAR * szMessage, COLORREF crColor);

	//��ť����
public:
	//���鰴ť
	void OnBnClickedExpression();

	//��Ϣ����
public:
	//�滭����
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//���Ͱ�ť
	afx_msg void OnBnClickedSendShort();
	//��ʷ��¼
	afx_msg void OnBnClickedHistory();
	//�˳���ť
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMin();
	//��ȡλ��
	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//����Ҽ�������Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//������Ϣ
	LRESULT OnExpression(WPARAM wparam, LPARAM lparam);
	//ͷ����Ϣ
	LRESULT OnHitLogo(WPARAM wparam, LPARAM lparam);

	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void TransUserInfo(UserItemStruct *pIn, SendMsgUserStruct *pOut);
};

#endif