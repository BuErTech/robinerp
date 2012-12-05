#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "GameCard.h"

//��Ϸ�����Ի���
class GameEndInfoDlg : public CGameFaceGo
{

	//��������
public:
	CListCtrl			m_List;								//�б���ͼ
	CUpGradeGameCard			m_BackCardView;						//������ͼ
	CNormalBitmapButton			m_btKnow;							//ȡ����ť
	CNormalBitmapButton			m_btQuit;							//�˳���ť
	enum				{ IDD = IDD_GAME_END };				//�Ի�������

	//��������
public:
	//���캯��
	GameEndInfoDlg(CWnd *pParent);   
	//��������
	virtual ~GameEndInfoDlg();
	//������Ϣ
	BOOL SetEndGameData(GameEndStruct * pGameEnd, UserItemStruct * pUserInfo[], int iPlayCardCount,CWnd * pParent);
	//��ʼ������
	virtual BOOL OnInitDialog();

protected:
	//ȡ��
	virtual void OnCancel();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	//�˳���ť
	afx_msg void OnBnClickedQuit();

	DECLARE_MESSAGE_MAP()
};
