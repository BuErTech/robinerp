#pragma once

#ifndef CAFCTABCTRL_HEAD_FILE
#define CAFCTABCTRL_HEAD_FILE

#include "PublicDefine.h"
#include "AFCResource.h"
#include "AFCWindow.h"

//����ҳ���
#define TS_UP						0							//�Ϸ�
#define TS_DOWN						1							//�·�
#define TS_LEFT						2							//��
#define TS_RIGHT					3							//�ҷ�
#define WM_SHOWGAMEWINDOW			WM_USER+1					//���͸�����, �����Ƿ���ʾ��Ϸ
#define WM_LOADSKIN					WM_USER+2
#define TIMER_AD					1							//����ƶ���ʱ��
//�������ҳ
#define MAX_PAGE					20							//�������ҳ

#define IDM_SEND_ROOMMSG			WM_USER+5					//���ͷ���������Ϣ
#define IDM_HIDE_RIGHT				WM_USER+10					//�����ұ�
#define IDM_SHOW_LEFT				WM_USER+15					//���ػ���ʾ���
#define IDM_OPEN_BANK				WM_USER+19					//������
#define IDM_OPEN_IE					WM_USER+20					//��IE
#define IDM_MESSAGE_APPEAL          WM_USER+21                  //Ͷ����Ϣ
#define RIGHT_WND_WIDTH				230							//�Ҳര�ڿ��

//ҳ���ඨ��
class CNewGoGamePower//CNewGoGamePower
{
	friend class CNewGameClass;

	//��Ϣ����
private:
	bool							m_bAutoDelete;				//�Զ�ɾ��
	CRect							m_rcTitleRect;				//��ǩλ��

	//�ؼ�����
private:
	HICON							m_hIcon;					//ͼ����
public:
	CWnd							* m_pWnd;					//����ָ��
	CWnd							*m_pRightWnd;				//�ұߴ���ָ��
	CString							m_strTitle;					//��������

	CGameImage						m_n,m_a;

	COLORREF						m_cn,m_ca;

	//��������
private:
	//���캯��
	CNewGoGamePower();
	//��������
	~CNewGoGamePower();
	//ȡ�ÿ��
	UINT GetAreaWidth(CDC * pDC);
	//�滭����
	void Draw(CDC * pDC, UINT nViewStyle, bool bActive);
	//ɾ��ҳ��
	bool DeletePage();

	void ShowGameWindow(int nShow);
	
	void SetSkin(TCHAR  pathn[], TCHAR  patha[], COLORREF cn, COLORREF ca);
};

//����ҳ��
class EXT_CLASS CNewGameClass/*CNewGameClass*/ : public CWnd
{
	friend class CNewGoGamePower;
	
	bool m_bInit;
	//��Ϣ����
protected:
	bool							m_bCopyRight;				//�Ƿ��Ȩ
	UINT							m_uActivePage;				//����ҳ��
	UINT							m_uViewStyte;				//��ʾ����
	INT_PTR							m_uIndex[MAX_PAGE];			//ҳ������
	CPtrArray						m_PageArray;				//ҳ������

	CGameImage						m_PicBottom;				//���ͼƬ
	//��Ȩ����
	CHyperLink						m_copyright;
	CRect							m_rect;
	HCURSOR							m_cursor;
	TCHAR							m_szURL[300];
	//skin
//	CGameImage						m_l,m_r,m_c,m_t;
	CGameImage						m_top;						//���ϸ߶�
	int								m_x,m_y;
	TCHAR							m_pathn[MAX_PATH],m_patha[MAX_PATH];
	COLORREF						m_cn,m_ca,m_adcolor;
	CRect							m_adRect,m_addrawrect;//�������
	CSize							m_adsize;
	CString							m_ad;

	CGameImage						m_tabc,m_tabl,m_tabr;		//����ҳ����
	CGameImage						m_tabn;						//����ҳ��ǩ
	//��������
public:
	//���캯��
	CNewGameClass();
	//��������
	virtual ~CNewGameClass();

	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//��������
	bool CreateTabWnd(CWnd * pParentWnd, UINT uWndID);
	//���봰��
	UINT AddTabPage(CWnd * pWnd,TCHAR * szTitle, HICON hIcon=NULL, bool bAutoDelete=true, bool bActive=true);
	//���봰��
	UINT AddTabPage(CWnd * pWnd, CWnd *pRightWnd,TCHAR * szTitle, HICON hIcon=NULL, bool bAutoDelete=true, bool bActive=true);
	//ɾ��ҳ��
	bool RemoveTabPage(UINT nExtIndex);
	//��ʾ��Ȩ��Ϣ
	bool ShowCopyRight(bool bShow=true);
	//�����
	bool SetActivePageByExtIndex(UINT uExtIndex);
	
	void ShowAllWindow(int bShow);
	
	void SetViewStyle(UINT uStyle);
	//�ڲ�����
private:
	//�����
	bool SetActivePage(UINT uIndex);
	//������С
	bool FixTabPageSize();
	//�滭���
	void DrawTabFrame(CDC * pDC);
	//��ȡҳ��
	CNewGoGamePower * GetPageItem(UINT nExtIndex);
	
	//��Ϣ���� 
public:
	//�ػ�����
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��갴����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�õ�����
	afx_msg void OnSetFocus(CWnd * pOldWnd);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��ϷTAB�����˫����Ϣ
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
public:
	void LoadSkin(void);
public:
	void SetAd(TCHAR ad[]);
	//��������λ��
	void AutoSize(int cx,int cy);
	//�������ڴ�С
	void SetMainRoomWnd(CRect rect);
	//������Ϣ
	LRESULT OnSendRoomMsg(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT OnHideRight(WPARAM wparam, LPARAM lparam);
	//������
	LRESULT OpenBank(WPARAM wparam, LPARAM lparam);
	//������
	LRESULT OpenIE(WPARAM wparam, LPARAM lparam);
    //Ͷ��
	LRESULT OAppeal(WPARAM wparam, LPARAM lparam);
};

#endif