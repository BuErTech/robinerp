#pragma once
#include "explorer1.h"
#include "resource.h"
#include "SkinMgr.h"
// CBulletin �Ի���

class CBulletin : public CDialog
{
	DECLARE_DYNAMIC(CBulletin)

public:
	CBulletin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBulletin();
// �Ի�������
	enum { IDD = IDD_BULLETIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_ie;
	CNormalBitmapButtonEx m_btClose;
	CNormalBitmapButtonEx m_btClose2;
	CGameImage					m_BackBitmap;					//�Ի��򱳾�ͼ
public:
	virtual BOOL OnInitDialog();
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
public:
	void OpenBulletin(CString surl);
public:
	afx_msg void OnPaint();
	void OnCancel();
private:
	CSkinMgr m_skinmgr;
private:
	void Init();
	//����
	void LoadSkin();
};
