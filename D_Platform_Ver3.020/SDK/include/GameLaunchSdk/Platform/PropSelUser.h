#pragma once

#include "publicdefine.h"
#include "ClientComStruct.h"
//#include "AFCResource.h"
//#include "AFCImage.h"
#include "afxwin.h"
#include "AFCButton.h"
#include "..\SkinMgr.h"
// CPropSelUser �Ի���
#define UM_USEITEM_USERSELECTED		WM_USER+3421
class CPropSelUser : public CDialog
{
	DECLARE_DYNAMIC(CPropSelUser)
	COLORREF m_bkColor;													//������ɫ
	COLORREF m_txtColor;												//��ʾ������ɫ�����õ��Ǻ�ɫ������
	HBRUSH m_bkBrush;
public:
	CPropSelUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPropSelUser();
	int		dwUsedPropID;												//���ѡ����û�ID
	int		dwLocalUserID;												//��ǰ���û�ID
	UserItemStruct					* m_pUserItem[MAX_PEOPLE];			//�û���Ϣ
	int		maxUser;													//��ʾ�����ٸ��û�
	CRect   userRect[MAX_PEOPLE];										//�����û���λ��
	Image *userImg[MAX_PEOPLE];											//�����û���ͷ��
	Image *imgDialog;													//����
	int		userID[MAX_PEOPLE];											//�����û���ID��
	CString userName[MAX_PEOPLE];										//�����û����ǳ�
	CString propItemName;												//Ҫʹ�õĵ�������

// �Ի�������
	//enum { IDD = IDD_PROP_SEL_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	void drawSkin(void);
public:
	afx_msg void OnPaint();
public:
	void resetInformation(void);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	void Init(void);
public:
	CNormalBitmapButtonEx m_bnClose;
public:
	afx_msg void OnBnClickedClose();
private:
	CSkinMgr m_skinmgr;
public:
	void LoadSkin();
};
