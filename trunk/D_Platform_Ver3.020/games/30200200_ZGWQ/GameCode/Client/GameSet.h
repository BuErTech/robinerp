#pragma once

#include "Stdafx.h"
#include "Resource.h"
//#include "Control\\MacButtons.h"

//��Ϸ���öԻ����� 
class CGameSet : public CCoolView//CGameFaceGo
{

	//��������
public:
	bool								m_bSound;						//����Ч��
	bool								m_bShowUser;					//�Ƿ���ʾ�û� 
	bool								m_bEnableWatch;					//�Ƿ������Թ�
	bool								m_bRightPower;
	CNormalBitmapButtonEx				m_Cancel;
	CNormalBitmapButtonEx				m_Ok;
	enum								{ IDD = IDD_GAME_SET };

	CButton								m_check_1;
	CButton								m_check_2;
	CButton								m_check_3;
	CButton								m_check_4;
		
	CWnd								*m_pParent;

	//�������� 
public:
	//���캯��
	CGameSet(CWnd * pParent=NULL);
	//��������
	virtual ~CGameSet();
	//��ʼ���Ի���
	virtual BOOL OnInitDialog();

protected:
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	//ȷ����ť
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
