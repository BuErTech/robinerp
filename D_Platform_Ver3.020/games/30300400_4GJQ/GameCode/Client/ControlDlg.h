#pragma once

#include "StdAfx.h"
#include "Resource.h"

//���ƶԻ���
class CControlDlg : public CAFCRgnDialog
{
	//��������
private:
	long int							m_dwMaxMoney;					//�����

	//��������
public:
	CAFCPicButton						m_BtPoint1;						//���밴ť
	CAFCPicButton						m_BtPoint5;						//���밴ť
	CAFCPicButton						m_BtPoint10;					//���밴ť
	CAFCPicButton						m_BtPoint50;					//���밴ť
	CAFCPicButton						m_BtPoint100;					//���밴ť
	CAFCPicButton						m_BtPoint500;					//���밴ť
	CAFCPicButton						m_BtFollow;						//��ע��ť
	CAFCPicButton						m_BtSelect;						//ѡע��ť
	CAFCPicButton						m_BtBegin;						//��ʼ��ť
	CAFCPicButton						m_BtGiveUp;						//������ť
	CAFCPicButton						m_BtShowHand;					//�����ť
	CWnd								* m_pHandleWnd;					//������

	//�������� 
public:
	//���캯��
	CControlDlg();
	//��������
	virtual ~CControlDlg();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel() { return; }

	DECLARE_MESSAGE_MAP()

	//���غ���
protected:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���ܺ���
public:
	//���ô���
	void SetHandleWnd(CWnd * pHandleWnd) { m_pHandleWnd=pHandleWnd; } ;
	//��������
	void SetEnableControl(bool bEnable, bool bShowHand);
	//���ý��
	void SetMaxMoney(long int dwMaxPoint);

	//��Ϣ����
public:
	//��ʼ
	void OnBegin();
	//λ�ú���
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//����Ҽ�������Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
