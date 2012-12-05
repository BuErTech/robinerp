#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameCard.h"

//������Ϣ��ʾ��
class CBackCardDlg : public CGameFaceGo//CAFCBaseDialog
{

	//��������
public:
	int					m_iBackCount;					//������Ŀ
	BYTE				m_iBackCard[8];					//��������
	CUpGradeGameCard			m_BackCard;						//�Լ����˿�
//	CAFCButton			m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx m_btOk;
	enum				{ IDD = IDD_BACK_CARD };		//�Ի�������

	//��������
public:
	//���캯��
	CBackCardDlg(CWnd* pParent = NULL);
	//��������
	virtual ~CBackCardDlg();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���õ���
	void SetBackCard(BYTE iCardList[], int iCardCount);

protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
};
