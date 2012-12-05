#pragma once

#include "Stdafx.h"
#include "Resource.h"

//��Ϸ������Ϣ��
class CGameInfoDlg : public CCoolView//CGameFaceGo
{
	//��������
public:
	enum				{ IDD = IDD_GAME_INFO };			// �Ի�������

	//�ؼ�����
public:	
	CNormalBitmapButtonEx			m_btOk;								//ȷ����ť
	CNormalBitmapButtonEx			m_btCancel;							//ȡ����ť
	CWnd*				m_pPP;
	CGameImage						m_bk;
	bool                       m_bRequireCoachMoeny;                  //�Ƿ���Ҫָ����
	//��������
public:
	//���캯��
	CGameInfoDlg(CWnd * pParent=NULL);
	//��������
	virtual ~CGameInfoDlg();
	//ȡ����ť
	virtual void OnCancel();
	//ȷ����ť
	virtual void OnOK();
	//������Ϸ����
	void SetGameSetInfo(int iJUMinute, int iJuSecond, int iBuMinute, int iBuSecond, int iDuMinute, int iDuSecond,int iGameMoney);
	//���öԻ���Ĵ�С
	void AutoSize();

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();	
};
