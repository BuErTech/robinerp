#pragma once

#include "Stdafx.h"
#include "Resource.h"

//��Ϸ������Ϣ��
class CGameDaoSHiDlg : public CCoolView//CGameFaceGo
{

	//��������
public:
	enum				{ IDD = IDD_DAO_SHI };			//ָ����ȷ�϶Ի���

	//�ؼ�����
public:	
	CNormalBitmapButtonEx			m_btOk;								//ȷ����ť
	CNormalBitmapButtonEx			m_btCancel;							//ȡ����ť
	CWnd*							m_pPP;
	CGameImage						m_bk;
	//��������
public:
	//���캯��
	CGameDaoSHiDlg(CWnd * pParent=NULL);
	//��������
	virtual ~CGameDaoSHiDlg();
	//ȡ����ť
	virtual void OnCancel();
	//ȷ����ť
	virtual void OnOK();	
	//���öԻ���Ĵ�С
	void AutoSize();
	//���öԻ�����Ϣ
	void SetInfo(CString &str);

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:	
};
