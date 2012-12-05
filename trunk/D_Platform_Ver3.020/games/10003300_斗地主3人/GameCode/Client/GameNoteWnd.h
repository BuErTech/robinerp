#pragma once

#include "StdAfx.h"

//��Ϸ��Ϣ��
class CGameNoteWnd : public CWnd
{
	//��������
protected:
//	CFont				m_Font;						//��ʾ����
//	CString				m_strMessage;				//�ַ���Ϣ
//	CBitmap				m_BackBmp;					//����
	CGameImage			m_bk;
	//��������
public:
	//���캯��
	CGameNoteWnd();
	//��������
	virtual ~CGameNoteWnd();
	//�Զ�������С
	void AutoSize();
	//������Ϣ
	void SetMessage(TCHAR * szMessage);

	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// PengJiLin, 2011-4-19, ����ʱ���������ͽ��ͼ����ʾ
// ----------------------------------------------------------------------------
class CGetMoneyWnd: public CWnd
{
protected:
    CGameImage			m_bk;
    CGameImage			m_Flag;     // ��ϲ ��־

    CString             m_strGetInfo;
    CString             m_strTimeInfo;
    CString             m_strCountInfo;

    int                 m_iStartX;
    int                 m_iStartY;
    int                 m_iStartSpace;
    int                 m_iFlagX;
    int                 m_iFlagY;
    COLORREF            m_cColor;

public:
    CGetMoneyWnd();
    virtual ~CGetMoneyWnd();
    void AutoSize();
    void SetShow(BOOL bShow);

    void SetMessage(CString& strGet, CString& strTime, CString& strCount, bool bInVideoRoom);

public:
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
};