#pragma once
#ifndef CHATCONTROL_HEAD_H
#define CHATCONTROL_HEAD_H
#include "UserListCtrl.h"

#include "UseBoardCastWnd.h"
#include "SkinMgr.h"

//������ƿ�
class CChatControl : public CWnd
{
	//��������
public:
	CToolTipCtrl					m_ToolTip;							//������ʾ
	CComboBox						m_InputMessage;						//������Ϣ
	CNormalBitmapButtonEx			m_BtSend;							//���Ͱ�ť
	CNormalBitmapButtonEx			m_BtExpression;						//���鰴ť
	CNormalBitmapButtonEx			m_BtBoardCast;						//����
	int								m_iSendY,m_iSendX;
	int								m_iExpX,m_iExpY;
	COLORREF						m_bkColor;
	CGameImage						m_send,m_exp, m_boardcast, m_bkImg;

	CUseBoardCastWnd				*m_BoardCastWnd;
	//��������
public:
	//���캯��
	CChatControl();
	//��������
	virtual ~CChatControl();

	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//��ȡ����ؼ�
	CComboBox * GetTalkInput() { return &m_InputMessage; };

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	
	void SetBkColor(COLORREF color);
	
	void SetImage(TCHAR path1[],TCHAR path2[], TCHAR path3[]);
	
	void SetImage(TCHAR bkimg[],TCHAR path1[],TCHAR path2[], TCHAR path3[]);
	//��Ϣ����
public:
	//����֪ͨ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//�ػ�����
	afx_msg void OnPaint();

	//��ť��Ϣ
public:
	//���Ͱ�ť
	void OnBnClickedSend();
	//���鰴ť
	void OnBnClickedExpression();
	//������ť
	void OnBnClickedAction();
	//���԰�ť
	void OnBnClickedAttrib();
	//�������������ļ�
	void ReLoadTalkINI();
	//���ؽ���
	void LoadSkin();

	//���Ȱ�ť
	void OnBnClickedBoardCast();


	void UseProp_Buy(int nPropIndex);
private:
	CSkinMgr m_skinmgr;
private:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

public:
	CWnd*	m_pGameRoom;
	void SetGameRoomWnd(CWnd* pWnd);
};
#endif
