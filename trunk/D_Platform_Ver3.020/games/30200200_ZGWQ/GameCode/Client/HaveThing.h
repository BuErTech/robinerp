#pragma once

#include "Stdafx.h"
#include "Resource.h"

//�����뿪�Ի�����
class CHaveThing : public CGameFaceGo
{
//	DECLARE_DYNAMIC(CHaveThing)

	//��������
public:
	CNormalBitmapButtonEx				m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx				m_btCancel;						//ȡ����ť
	enum					{ IDD = IDD_THING };			//�Ի�������
	CWnd							*m_pParnet;
	CGameImage						m_bk;
	//��������
public:
	//���캯��
	CHaveThing();   
	//��������
	virtual ~CHaveThing();
	//��ʼ������
	virtual BOOL OnInitDialog();

protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
