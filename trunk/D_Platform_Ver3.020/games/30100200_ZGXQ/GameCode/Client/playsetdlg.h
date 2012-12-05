#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "afxcmn.h"

//�˿�������Ϣ
struct tagGameSetInfo
{
	int			iJuTimeMinute;//��ʱ��
	int			iJuTimeSecond;//��ʱ��
	int			iBuTimeMinute;//��ʱ��
	int			iBuTimeSecond;//��ʱ��
	int			iDuMiaoMinute;//�����
	int			iDuMiaoSecond;//������
	int			iGameMoney;	  //��Ϸָ����

	
	tagGameSetInfo()
	{
		iJuTimeMinute = 10;
		iJuTimeSecond = 15;
		iBuTimeMinute = 3;
		iBuTimeSecond = 15;
		iDuMiaoMinute = 5;
		iDuMiaoSecond = 25;	
		iGameMoney = 0;
	}
};

//�ƾ�������
class CPlaySetDlg : public CCoolView//CGameFaceGo//CAFCBaseDialog
{

	//��������
public:
	tagGameSetInfo		m_GameSetInfo;					//��Ϸ������Ϣ
	enum				{ IDD = IDD_SET_PLAY };			//�Ի�������

	//�ؼ�����
public:
	CNormalBitmapButtonEx			m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx			m_btCancel;						//ȡ����ť
	CWnd*							m_pPP;
	CGameImage						m_bk;
	bool                            m_bRequireCoachMoeny;
	//��������
public:
	//���캯��
	CPlaySetDlg(CWnd * pParent=NULL);
	//��������
	virtual ~CPlaySetDlg();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȡ����ť
	virtual void OnCancel();
	//ȷ����ť
	virtual void OnOK();
	//������Ϸ����
	void SetGameSetInfo(int iCardCount);
	//�����Ի����С
	void AutoSize();
	//��ȡ��Ϸ����
	tagGameSetInfo * GetGameSetInfo();

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_SpinCtrl;
public:	
};
