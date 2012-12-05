#ifndef CAFCGAMELISTWND_HEAD_FILE
#define CAFCGAMELISTWND_HEAD_FILE

#include "GameListCtrl.h"
#include "GamePlaceMessage.h"
#include "PersonalSet.h"
#include "IMMain.h"
#include "UserInfoWnd.h"
#include "DUIUserTabWnd.h"
#include "FinanceWnd.h"

#include "IMWnd.h"
#include "SkinMgr.h"
#include <queue>
using namespace std;

#define WM_LOADSKIN					WM_USER+2
#define IDM_SKIN					WM_USER+3
#define IDM_HIDE_LEFT				WM_USER+4

#define MESSAGE_TITLE					TEXT("ϵͳ��Ϣ")		//��Ϸ��Ϣ����

//#define IDM_HIDE_RIGHT				WM_USER+5
class  CGameListWnd:public CWnd
{
	bool m_bInit;
	CToolTipCtrl							m_toolTip;
	bool									bAddLogoTool;
	//��Ϣ����
protected:
	CGameImage								m_gamelistlc,m_gamelistrc,m_gamelisttc,m_gamelistbc;		//�ײ�����
	CGameImage								m_gamelistlt,m_gamelistlb,m_gamelistrt,m_gamelistrb;	//�Ľ�
	CGameImage								m_gamelistAdorn;						
	//װ������
public:
	CGameListCtrl							m_GameList;												//��Ϸ�б�
	CIMMain									m_IMList;												//��Ϸ�б�
	CGameListCtrl							m_HelpList;							//��������
	CPersonalSet							*personalSet;
	CRect									logoRect;
	MSG_GP_R_LogonResult					*m_PlaceUserInfo;			//�û���½����
	CUserInfoWnd*							m_pUserInfoWnd;				//�û����Ϲ����
	CUserTabDuWnd							m_UserTabDuWnd;
	CFinanceWnd*							m_pFinanceWnd;				//����״������

	CIMWnd									*m_pIMWnd;

	int										m_iShowBtRSpace;            //��ʾ���ذ�Ť�������Ŀ�϶
	CRect									m_rcBtns[5];				//�·���ťȺ��λ��
	CRect									m_rcFinanceBtn;				//����ťȺ��λ��

		//////////////////////////////////////////////////////////////////////////
	////////Kylin 20090107 ��ӹ��� TAB �ؼ�
	//CNewGameClass			m_FunListTabWnd;								
	CNormalBitmapButtonEx	m_btGameList;//��Ϸ

	CNormalBitmapButtonNew	m_btChongZhi;//��ֵ
	CNormalBitmapButtonNew	m_btUserInfo;//�û���Ϣ
	CNormalBitmapButtonNew	m_btIMList;	 //����
	CNormalBitmapButtonNew	m_btFascX;	 //����ֵ�һ�

	CNormalBitmapButtonNew	m_btMail;	 //�ʼ�
	CNormalBitmapButtonNew	m_btFinance;	 //����״��

	void OnGameList();
	void OnChongZhi();
	void OnUserInfo();
	void OnIMList();
	void OnFacExchange();
	void OnMailClick();
	CFunListCtrl			m_FunList;								//�����б�
	void FoucsTab(int id);
	afx_msg void OnNMDblclkFunList(NMHDR * pNMHDR, LRESULT * pResult);
	
	//////////////////////////////////////////////////////////////////////////
protected:
	CNormalBitmapButtonEx					m_Bt1;													//
	CNormalBitmapButtonEx					m_Bt2;													//
//	CNormalBitmapButtonEx					m_Bt3;													//
	//CNormalBitmapButtonEx					m_Bt4;													//

	bool									m_bHideList;											//�����б�

	CGameImage								m_infolc,m_inforc,m_infotc,m_infobc;	//����ͷ���߽�
	CGameImage								m_infolt,m_infort,m_infolb,m_inforb;	//����ͷ���߽�
	CGameImage								m_infol,m_infor,m_infoc;				//����ͷ�������
	CGameImage								m_space;								//��϶
	CGameImage								m_LogoImg,m_PersonalInfo;	//����ͷ����ͷ��
	int										m_infox,m_infoy;			//������λ��
	///////////////////////////////////////
	///////Kylin 20090105 �����ǳƣ�����ֵyƫ��	
	int										m_nickdy,m_meilidy;			//�ǳƣ�����ֵyƫ��
	///////////////////////////////////////
	
	int										m_picx,m_picy;				//����ͷ��λ��
	WCHAR									m_wszBuffer[MAX_PATH];			//���ַ����飨�滭png��
	COLORREF								m_titlecolor;
	CGameImage								m_leftspace,m_rightspace;												//�߼�


	queue<TMailItem>						m_Mails;
	//��������
public:
	//���캯��
	CGameListWnd();
	//��������
	virtual ~CGameListWnd();

	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//��������
	bool CreateWnd(CWnd * pParentWnd, UINT uWndID);
	//�ڲ�����
public:
	//������С
	bool FixControlStation();
private:
	//�滭���
	void DrawFrame(CDC * pDC);
	//��Ϣ���� 
public:
	//�ػ�����
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
	LRESULT OnExchangeSkin(WPARAM wparam, LPARAM lparam);
	
private:
	//���ƿ��
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);
public:
	void LoadSkin(void);
	void ReloadLogo();
	//��������λ��
	void AutoSize(int cx,int cy);
public:
	afx_msg void OnMoving(UINT nSide, LPRECT lpRect);
	//�����Ϸ�б�
	afx_msg void OnNMClickGameList(NMHDR *pNMHDR, LRESULT *pResult);
	//�һ���Ϸ�б�
	afx_msg void OnNMRclickGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//˫����Ϸ�б�
	afx_msg void OnNMDblclkGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//��Ϸ�б���ı�
	afx_msg void OnTvnSelchangedGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�����չ��
	afx_msg void OnTvnItemexpandingGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�����չ��
	afx_msg void OnTvnItemexpandedIMList(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�����չ��
	afx_msg void OnNMDblclkHelpList(NMHDR * pNMHDR, LRESULT * pResult);
	////���IM�б�
	//afx_msg void OnNMClickIMList(NMHDR *pNMHDR, LRESULT *pResult);
	////�һ�IM�б�
	//afx_msg void OnNMRclickIMList(NMHDR * pNMHDR, LRESULT * pResult);
	////˫��IM�б�
	//afx_msg void OnNMDblclkIMList(NMHDR * pNMHDR, LRESULT * pResult);
	//�б���Ϣ
	bool OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��ť
	void OnBnClickedBt1();
	//��������
	void OnBnClickedBt2();

	//��ʾ/���ظ��˲�����Ϣ
	void OnShowFinance();
	void OnHideFinance();

	
	//void OnBnClickedBt3();
	//
	//void OnBnClickedBt4();
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//�����û���Ϣ
	void  SetUserInfoWnd(MSG_GP_R_LogonResult *logoResult);
	//���¸���������Ϣ
	void UpdateMeInfo(__int64 i64Bank,BOOL IsUpdateBank);
	//���¸��˲Ƹ�
	void UpdateMeFortune(__int64 i64Money,__int64 i64Bank);
	//��ȡ��������ػ�����ʾ
	BOOL GetLeftShowOrHide(){return m_bHideList;}
	/// ǿ�������б�������״̬
	void ShowHideList(bool bHide)
	{
		m_Bt1.EnableWindow(!bHide);
	}

	//JianGuankun 2011.11.19 ���ظ������͵���Ϸ�б�
	//����Web��Ϸ
	void AddWebGameToList();
	//���ص���Exe��Ϸ
	void AddExeGameToList();
	//���ص���3D��Ϸ
	void Add3DGameToList();

	/////////////////////////////////////////////////////////

	//��ȡ��Ϸ�б��Ƿ�����
	bool GetIsHide();

	void AddMail(TMailItem mail);

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CSkinMgr m_skinmgr;

	void ChangeSkin();
};

#endif