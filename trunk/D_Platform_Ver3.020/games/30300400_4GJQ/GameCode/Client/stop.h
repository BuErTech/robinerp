#pragma once

#include "Stdafx.h"
#include "Resource.h"

//�����뿪�Ի�����
class CStop : public CGameFaceGo
{
//	DECLARE_DYNAMIC(CHaveThing)

	//��������
public:
	CNormalBitmapButtonEx				m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx				m_btCancel;						//ȡ����ť
	enum					{ IDD = IDD_STOP };			//�Ի�������
	CWnd				*m_pParnet;
	CGameImage			m_bk;			                //����ͼ


	//��������
public:
	//���캯��
	CStop(CWnd * pParent=NULL);   
	//��������
	virtual ~CStop();
	//��ʼ������
	virtual BOOL OnInitDialog();

protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);   
	void AutoSize();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
};
