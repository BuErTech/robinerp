#pragma once

#include "Stdafx.h"
#include "Resource.h"

//����λ��
class CFindDesk : public CAFCBaseDialog
{
	//��������
public:
	bool							m_bFindOne;								//��������
	bool							m_bFindTwo;								//��������
	bool							m_bFindNoAllNull;						//��������
	bool							m_bFindAllNull;							//��������
	BYTE							m_bLastFindDesk;						//���ҽ��

	//�ؼ�ָ��
protected:
	CWnd							* m_pParent;							//��������

	//�ؼ�����
public:
	CAFCPicButton					m_BtCancel;								//ȡ����ť
	CAFCPicButton					m_BtSitDown;							//���°�ť
	CAFCPicButton					m_BtFind;								//���Ұ�ť

	//��������
public:
	//���캯��
	CFindDesk(CWnd * pParent);   
	//��������
	virtual ~CFindDesk();

	//���غ���
public:
	//�ؼ��󶨺���
	virtual void DoDataExchange(CDataExchange* pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȡ��
	virtual void OnCancel();
	//ȷ��
	virtual void OnOK();

	//��Ϣ����
protected:
	//����
	afx_msg void OnBnClickedSitdown();

	DECLARE_MESSAGE_MAP()
};
