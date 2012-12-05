#pragma once

#ifndef ROOMSETWND_HEAD_H
#define ROOMSETWND_HEAD_H

#include "StdAfx.h"

//��������ģʽ
#define INVALI_ALL					0			//������������
#define INVALI_ONLY_FRIEND			1			//ֻ������������
#define INVALI_NO					2			//�������κ�����

class CRoomSetWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CRoomSetWnd();
	virtual ~CRoomSetWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

public:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void OnOK();

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

public:

	BzDui::CPaintManagerUI m_pm;

//�ؼ�����
public:

	bool								m_bAllowSetDeskPwd;					//�Ƿ�����������������
	BYTE								m_bSaveTalk;						//�����¼
	BYTE								m_bShowUser;						//��ʾ�û�
	BYTE								m_bShowMessage;						//��ʾ��Ϣ

public:
	BYTE								m_bSameIP;							//��ͬ IP
	BYTE								m_bSameIPNumber;					//����ǰ��λIP��ͬ�������Ϸ
	BYTE								m_bPass;							//��������
	BYTE								m_bLimitCut;						//���ƶ���
	BYTE								m_bLimitPoint;						//���ƾ���ֵ
	BYTE								m_bCutPercent;						//������
	BYTE								m_bInvaliMode;						//����ģʽ
	TCHAR								m_szPass[61];						//��������
	long int							m_dwLowPoint;						//��;���ֵ�����֣�
	long int							m_dwHighPoint;						//��߾���ֵ�����֣�
	long int							m_dwLowMoney;						//��;���ֵ����ң�
	long int							m_dwHighMoney;						//��߾���ֵ����ң�
	bool								m_bPlayBkMusic;						//���ű�������

	int									m_nRetFlag;							//1��ʾȷ����0��ʾȡ��
};





#endif
/*******************************************************************************************************/