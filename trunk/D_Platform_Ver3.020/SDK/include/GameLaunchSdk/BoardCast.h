#pragma once

#include "resource.h"
#include "Platform/AFCImage.h"
#include "Platform/AFCButton.h"
#include "SkinMgr.h"
// CBoardCast �Ի���

class CBoardCast : public CDialog
{
	DECLARE_DYNAMIC(CBoardCast)

public:
	CBoardCast(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBoardCast();
public:
	CGameImage				m_BackgroundImage3;	//С���Ƚ���
	CGameImage				m_BackgroundImage2;	//�����Ƚ���
	CNormalBitmapButtonEx	m_btCancel;
	CNormalBitmapButtonEx	m_btOk;
	CNormalBitmapButtonEx	m_btChangeColor;
	CWnd					*pParentWnd;
	int						m_nPropID;

	CString					m_strInput;
	UINT					m_uiInputLen;
	int						m_iBroadcastStyle;						//��������
	COLORREF				m_RGB;

	HBRUSH m_bkBrush;

    // PengJiLin, 2010-9-7, ���˿������߿���ʾ�Ի��򣬽��ô���
    BOOL    m_bUseForOther;     // �Ƿ���������������
    BOOL    m_bMouseInRect;     // ����Ƿ�����������
    BOOL    m_bIsTOut;          // �Ƿ�������, TRUE = ����, FALSE = ����
    CString m_strInfo;          // ��ʾ��Ϣ

    // PengJiLin, 2010-9-7, ģ�ⳬ���ӵ�����
    BOOL    m_bUseLink;         // �Ƿ�ʹ�ó�����
    int m_iLeft;
    int m_iTop;
    int m_iRight;
    int m_iBottom;

    // PengJiLin, 2010-9-7, ����
    CFont m_fontCaption;

    // PengJiLin, 2010-9-9, ���߹��ܽ���
    CGameImage				m_BackgroundImageProp;

private:
	CSkinMgr m_skinmgr;
// �Ի�������
	//enum { IDD = IDD_BOARDCAST };

public:
    // PengJiLin, 2010-9-9, ���ڵ�����ʾ��bUseLink�Ƿ�ʹ�ó�����
    void FuncForProp(BOOL bUseForOther, BOOL bUseLink, BOOL bIsTOut);
    void SetPropInfo(CString& strInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
	afx_msg void OnChangeColor();

	virtual void OnOK();
public:
	afx_msg void OnPaint();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEditInput();

    // PengJiLin, 2010-9-7
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void SetValue(int iBroadcastMsg);
	//����ɫ
	COLORREF GetDuduRGB();

public:
	void LoadSkin();
};
