#pragma once
#ifndef USERPROP_HEAD_H
#define USERPROP_HEAD_H
#include "Resource.h"
#include "afcbutton.h"
#include "afcfunction.h"

//��Ϸ���öԻ����� 
class EXT_CLASS CUserProp : public CDialog//CGameFaceGo
{
//��������
public:
	CNormalBitmapButtonEx				m_Cancel;
	CNormalBitmapButtonEx				m_Ok;
	enum								{ IDD = IDD_PROP };

	CWnd								*m_pParent;
	CGameImage							m_bk;			//����ͼ
	TCHAR								m_szName[MAX_PEOPLE][21];
	int									m_iCurCount;
	int									m_iCurDest;
	int									m_iCurIndex;							//��ǰ��������
	CRect								m_iRect[2];
	//�������� 
public:
	//���캯��
	CUserProp(CWnd * pParent=NULL);
	//��������
	virtual ~CUserProp();
	//��ʼ���Ի���
	virtual BOOL OnInitDialog();
	//�����û��ǳ�
	virtual void AddUser(TCHAR *szUserName);
	//��ʼ��
	void InitUserList();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	//
	virtual void OnOK();
	//
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMove(int x, int y);
};

#endif