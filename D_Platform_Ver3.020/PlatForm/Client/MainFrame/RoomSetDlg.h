#pragma once
#ifndef ROOMSETDLG_HEAD_H
#define ROOMSETDLG_HEAD_H

#include "StdAfx.h"
#include "Resource.h"
#include "..\UILibrary2003\CconfigDlg.h" 
//#include "NewControl\\MacButtons.h"

//��������ģʽ
#define INVALI_ALL					0			//������������
#define INVALI_ONLY_FRIEND			1			//ֻ������������
#define INVALI_NO					2			//�������κ�����

/*******************************************************************************************************/

//��ť����
struct StructButtonItem
{
	UINT											uButtonID;						//��ť ID
	CString											strButtonText;					//��ť����
};

//���ð�ť����
class CAFCButtonBar : public CWnd
{
	//��������
protected:
	INT_PTR											m_uHotItem;						//������
	INT_PTR											m_uActiveItem;					//������
	CArray<StructButtonItem,StructButtonItem>		m_ButtonInfoArray;				//��ť����

	//��������
public:
	//���캯��
	CAFCButtonBar();
	//��������
	virtual ~CAFCButtonBar();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//���밴ť
	INT_PTR AddButton(UINT uButtonID, TCHAR * szButtonTitle);
	//ɾ����ť
	INT_PTR DeleteButton(UINT uButtonID);
	//���ü�����
	void SetActiveButton(UINT uButtonID);

	//�ڲ�����
private:
	//��������
	INT_PTR HitTest(CPoint MousePoint);
	//�滭����
	void DrawButtonItem(CDC * pDC, CRect & rcDraw, INT_PTR uIndex);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�������Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	//����Ҽ�����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/*******************************************************************************************************/

//���öԻ��������
class CBaseSetDlg : public CGameFaceGo
{
	//��������
public:
	//���캯��
	CBaseSetDlg(); 
	//��������
	virtual ~CBaseSetDlg();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//ȷ��
	virtual void OnOK() { return; }
	//ȡ��
	virtual void OnCancel() { GetParent()->PostMessage(WM_CLOSE,0,0); }

	//�ӿں���
public:
	//�������
	virtual bool SaveSetting()=0;

	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
};

/*******************************************************************************************************/

//����Ի�����
class CGameRuleSetDlg : public CBaseSetDlg
{


	HBRUSH m_bkBrush;
	//��������
public:
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
	long int							m_dwLowPoint;						//��;���ֵ
	long int							m_dwHighPoint;						//��߾���ֵ 

	//�ؼ�����
public:
	CTrueEdit							m_CutInput;							//������
	CTrueEdit							m_HightPoint;						//��߾���ֵ
	CTrueEdit							m_LessPoint;						//��;���ֵ
	CTrueEdit							m_PassWord;							//�����
	CGameImage							m_bk;//����

	//��������
public:
	//���캯��
	CGameRuleSetDlg(); 
	//��������
	virtual ~CGameRuleSetDlg();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange * pDX);    
	//�������
	virtual bool SaveSetting();

	//��Ϣ����
public:
	//�õ�����
	afx_msg void OnSetFocus(CWnd * pOldWnd);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/*******************************************************************************************************/

//����Ի�����
class CRoomRuleSetDlg : public CBaseSetDlg
{
	//��������
public:
	BYTE								m_bSaveTalk;						//�����¼
	BYTE								m_bShowUser;						//��ʾ�û�
	BYTE								m_bShowMessage;						//��ʾ��Ϣ

	//��������
public:
	//���캯��
	CRoomRuleSetDlg(); 
	//��������
	virtual ~CRoomRuleSetDlg();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange * pDX);    
	//�������
	virtual bool SaveSetting();
};

/*******************************************************************************************************/

//���öԻ���
class CRoomSetDlg : public CLoadFileBmp
{
	//��������
public:
	BYTE								m_bSaveTalk;						//�����¼
	BYTE								m_bShowUser;						//��ʾ�û�
	BYTE								m_bShowMessage;						//��ʾ��Ϣ
	//��������
protected:
	int										m_iActiveIndex;					//����λ��
	CBaseSetDlg								* m_pActiveSet;					//�����

	//�ؼ�����
public:
	CGameRuleSetDlg							m_GameRuleSet;					//��������
	CNormalBitmapButtonEx							m_BtOK;							//ȷ����ť
	CNormalBitmapButtonEx							m_BtCancel;						//ȡ����ť
	CNormalBitmapButtonEx							m_BtClose;						//JMod-�رհ�ť-20090515
	CGameImage								m_bk; 
	CconfigDlg                              m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
	CRect							m_rect;//��Ȩ��Ϣ
	//��������
public:
	//���캯��
	CRoomSetDlg(CWnd * pParent=NULL);
	//��������
	virtual ~CRoomSetDlg();

	DECLARE_MESSAGE_MAP()

	//���غ���
protected:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ��
	virtual void OnOK();

	//���ܺ���
protected:
	//�����ؼ� 
	void FixControlSize(int iWidth, int iHeight);

	//��Ϣ����
public:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//������ť
	LRESULT OnHitButton(WPARAM wparam, LPARAM lparam);
	//�������
	virtual bool SaveSetting();
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnBnClickedCancel2();
	  // ���ô���Բ�� 2012.10.17 yyf
	  void SetWndRgn(void);
};

#endif
/*******************************************************************************************************/
