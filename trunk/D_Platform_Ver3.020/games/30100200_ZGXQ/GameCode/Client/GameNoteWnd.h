#pragma once

#include "StdAfx.h"

/// ��Ϸ��Ϣ��
class CGameNoteWnd : public CWnd
{
	/// ��������
protected:
	CFont				m_Font;						///< ��ʾ����
	CString				m_strMessage;				///< �ַ���Ϣ
	CGameImage			m_BackBmp;					///< ����

	/// ��������
public:
	/// ���캯��
	CGameNoteWnd();
	/// ��������
	virtual ~CGameNoteWnd();
	/// �Զ�������С
	void AutoSize();
	/// ������Ϣ
	void SetMessage(TCHAR * szMessage);

	/// ��Ϣ����
public:
	/// �ػ�����
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};
