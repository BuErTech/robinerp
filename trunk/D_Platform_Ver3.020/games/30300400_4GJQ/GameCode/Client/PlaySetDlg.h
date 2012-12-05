#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "afxcmn.h"

//�˿�������Ϣ
struct tagGameSetInfo
{
	//int			iBeginStation;					//��ʼλ��
	//int			iEndStation;					//����λ��
	int			iPlayCount;						//˼��ʱ��
	BYTE		iCardCount;						//�˿���Ŀ

	tagGameSetInfo()
	{
		//iBeginStation=0;
		//iEndStation=12;
		iPlayCount=1;
		iCardCount=1;
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
	CWnd*				m_pPP;
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
	//��ȡ��Ϸ����
	tagGameSetInfo * GetGameSetInfo();

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_SpinCtrl;
};
