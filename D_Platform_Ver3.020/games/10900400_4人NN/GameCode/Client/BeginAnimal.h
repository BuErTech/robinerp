#pragma once

#include "Resource.h"
// CBeginAnimal �Ի���

class CBeginAnimal : public CDialog
{
	DECLARE_DYNAMIC(CBeginAnimal)
private:
	CGameImage			m_bk;								//����ͼ
	
public:
	CBeginAnimal(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBeginAnimal();

// �Ի�������
	enum { IDD = IDD_BEGIN_ANIMAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//��ʼ������
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	void FixCardControl(int x,int y);
};
