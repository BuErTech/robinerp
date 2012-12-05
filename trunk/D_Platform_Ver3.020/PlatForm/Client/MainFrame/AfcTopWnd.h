#ifndef CAFCWNDTOP_HEAD_FILE
#define CAFCWNDTOP_HEAD_FILE

#include "SkinMgr.h"
#include "QuitAsk.h"
#include <vector>
#include "gif\WndAnimate.h"
using namespace std;

#define WM_LOADSKIN					WM_USER+2
#define IDM_SKIN					WM_USER+3
#define ID_REST_MAX					WM_USER+4
//#define IDM_OPEN_BANK				WM_USER+4

class CGamePlaceDlg;

class  CTopWndClass:public CWnd
{
	bool							bInitialed;

	//��Ϣ����
protected:
	CNormalBitmapButtonEx					m_Bt1;					//
	CNormalBitmapButtonEx					m_Bt2;					//
	CNormalBitmapButtonEx					m_Bt3;					//
	CNormalBitmapButtonEx					m_Bt4;					//
	CNormalBitmapButtonEx					m_Bt5;					//
	CNormalBitmapButtonEx					m_Bt6;					
	CNormalBitmapButtonEx					m_Bt7;
	CNormalBitmapButtonEx					m_Bt8;					//
	CNormalBitmapButtonEx					m_Bt9;					
	CNormalBitmapButtonEx					m_Bt10;

	CNormalBitmapButtonEx					m_Bt11;

	CGamePlaceDlg*							m_pPlasceDlg;


	//wushuqun 2009.7.15
	CGameImage  m_imageBt1,m_imageBt2,m_imageBt3,m_imageBt4,m_imageBt5,m_imageBt6,m_imageBt7,
		        m_imageBt8,m_imageBt9,m_imageBt10_1, m_imageBt10_2,m_imageBt11, m_imageRight;

	CNormalBitmapButtonEx					m_BtClose;					//����
	CNormalBitmapButtonEx					m_BtMin;					//���

	CNormalBitmapButtonEx					m_BtSkin;					//����

	CGameImage								m_ltlogo;					//�����Ϸlogo
	CIEClass								m_WebView;					//��ҳ��ͼ
	CGameImage								m_top,m_bottom,m_left,m_right;		//�����߿�
	CGameImage								m_tl,m_tr,m_bl,m_br;		//�����߽�
	

	CGameImage								m_topc,m_topl,m_topr;	//��������
	int										m_btnx,m_btny;				//������ť��ʼλ��
	int										m_topskx,m_topsky;			//����������ťλ��
	int										m_topminx,m_topminy;		//������С����ťλ��
	int										m_topclx,m_topcly;			//�����رհ�ťλ��
	int										m_toprestx,m_topresty;		//������󻯣���ԭ��ťλ��
	int										m_topImgRightXoffset;		//����һ�Ű�ť�ұߵı߽紦����ͼ��Xƫ��
	
	//����ť֮��Ŀռ�
	int										m_iRightBlank;				//����������ť�Ҳ����ײ��ֿ��
	int										m_iSpaceWidth;				//��ť��϶
	int										m_iAdX,m_iAdY,m_iAdWidth,m_iAdHeight;	//�������x,y
	int										m_iAdBtnX,m_iAdBtnY;			//��һ����ť��ʼλ��
	int										m_ilogX,m_ilogY;				//logoλ��
	int										m_iLockX, m_iLockY;			///< ������ťλ�� zxd20091218
	BOOL									m_bLockAccount;				//��ʾ��ǰ�ʺ��Ƿ��������� zxj 2009-11-13
	TCHAR									m_szLockPath[MAX_PATH];		//��������ͼƬĿ¼ zxj 2009-11-13
	TCHAR									m_szUnlockPath[MAX_PATH];	//�������ͼƬĿ¼ zxj 2009-11-13
	BOOL									m_bLogon;					//�Ƿ�ɹ���¼��Z������ zxj 2009-11-13
	//��������
public:
	CToolTipCtrl							m_ToolTip;						//������ť��ʾ
	//���캯��
	CTopWndClass();
	//��������
	virtual ~CTopWndClass();
	CNormalBitmapButtonEx					m_BtMax;					//���	
	CNormalBitmapButtonEx					m_BtRestore;				//��ԭ
	CWndAnimate                             m_toplogo;
	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//��������
	bool CreateWnd(CWnd * pParentWnd, UINT uWndID);
	//��ʾ��Ȩ��Ϣ
	//bool ShowCopyRight(bool bShow=true);
	//�ڲ�����
public:
	//������С
	bool FixControlStation();

	//��������������ID zxj 2009-11-12
	void SetLockType(int nLockType);

	//��������������ID zxj 2009-11-12
	void SetLockWndState(BOOL bType);
private:
	//�滭���
	void DrawFrame(CDC * pDC);
	//��Ϣ���� 
public:
	//�ػ�����
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
	LRESULT OnExchangeSkin(WPARAM wparam, LPARAM lparam);
	
public:
	void LoadSkin(void);

private:
	//���ƿ��
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);

protected:
	void OnHitMin();
	void OnHitMax();
	void OnHitRestore();
	void OnHitSkin();
	void OnHitClose();
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	//��������λ��
	//void AutoSize(int cx,int cy);
	//���ܰ�ť
	void OnBnClickedBt1();

	void OnBnClickedBt2();

	void OnBnClickedBt3();

	void OnBnClickedBt4();

	void OnBnClickedBt5();

	void OnBnClickedBt6();

	void OnBnClickedBt7();

	void OnBnClickedBt8();

	void OnBnClickedBt9();

	void OnBnClickedBt10();
	//�õ�֪ͨ�Ѿ��յ�flash��ַ
	void SetFlashAd();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/////////////////////////////////
	///Kylin �������У���������ֻ����Ϸ������ʾ����
	bool m_bHide;
	/////////////////////////////////

private:
	//���ص�Ƥ��
	CSkinMgr m_skinmgr;

public:
	CQuitAsk m_quitAskdlg;
};

#endif