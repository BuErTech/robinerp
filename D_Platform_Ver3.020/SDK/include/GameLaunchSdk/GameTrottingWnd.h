#pragma once

#ifndef PGameTrottingWnd_HEAD_H
#define PGameTrottingWnd_HEAD_H

#include "StdAfx.h"

static const int LENGTH		= 516;
static const int HEIGHT		= 24;
static const int ADJUST		= 33;
static const int IDT_PMD	= 0x2267;
static const int IDT_PIECE	= 0x2268;
static const int BUF_SIZE	= 4096;
static const int TEXT_SIZE	= 2048;

//���ֽڱ���ת���ɿ��ֽڱ���
void char2Wchar(char* pchar,wchar_t* wchar);

//���ֽڱ���ת���ɶ��ֽڱ���
void wchar2char(wchar_t* wchar,char* pchar);

class CGameTrottingWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CGameTrottingWnd(CWnd* pParent);
	virtual ~CGameTrottingWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

	void CenterWindow();

public:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	bool OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

public:	
	void init(HWND hwndParent);
	void AdjustPos(HWND hWnd);
	void ShowCtrl(bool bShow=true);
	void StartPmdLabel(int tmLen, char *szText, int tmSpeed=200, bool bDir=true);
	void StopPmdLabel();
	void SetLabelTextFont(char *szFont, int iSize);
public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

public:

public:

	BzDui::CPaintManagerUI m_pm;

//�ؼ�����
protected:

public:

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;			

private:
	POINT				m_AdjustPos;
	BzDui::CLabelUI		*m_pLabel;
	RECT				m_LabelRect;
	char				m_Text[TEXT_SIZE];
	char				m_szFont[32];
	int					m_iSize;
	POINT				m_LabelPos;
	bool				m_bDir;
};


#endif
/*******************************************************************************************************/