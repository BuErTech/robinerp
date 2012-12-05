#pragma once

#ifndef CGAMEENDVIEW_HEAD_FILE
#define CGAMEENDVIEW_HEAD_FILE

#include "PublicDefine.h"
#include "AFCResource.h"

/*******************************************************************************************************/

//��Ϸ������ͼ
class EXT_CLASS CPalatesNeck/*C-GameEndView*/ : public CWnd
{
	//��������
protected:
	long int						m_dwGameTax;						//��Ϸ˰��
	long int						m_dwPoint[MAX_PEOPLE];				//��Ϸ�÷�
	CString							m_strName[MAX_PEOPLE];				//�û�����
	CSize							m_WndSize;							//���ڴ�С
	CGameImage						m_BackBmp;							//����ͼ��
	const bool						m_bShowTax;							//�Ƿ�˰��
	const BYTE						m_bGamePeople;						//��Ϸ����
	CFont							m_Font;

	//��������
public:
	//���캯��
	CPalatesNeck(BYTE bGamePeople, bool bShowTax);
	//��������
	virtual ~CPalatesNeck();

	//���ܺ���
public:
	//��ȡ��С����
	void GetWndSize(SIZE & Size);
	//������Ϣ
	void SetGamePoint(BYTE bDeskStation, TCHAR * szUserName, long int dwPoint);
	//����˰��
	void SetTax(long int dwGameTax);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/*******************************************************************************************************/

#endif