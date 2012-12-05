#ifndef CAFCRoomRightWnd_HEAD_FILE
#define CAFCRoomRightWnd_HEAD_FILE



#include "UserListCtrl.h"
#include "ChatControl.h"
#include "WebWnd.h"
#include "afxwin.h"
#include "SkinMgr.h"

#define WM_LOADSKIN					WM_USER+2
#define IDM_SKIN					WM_USER+3
#define IDM_SEND_ROOMMSG			WM_USER+5							//���ͷ���������Ϣ
#define CHAR_HEIGHT					22							//����߶�
#define IDM_HIDE_RIGHT				WM_USER+10					//�����ұ�
//#define PROP_PEER_PAGE	            4       // PengJiLin, 2010-10-12
#define PROP_PEER_PAGE	            8       // PengJiLin, 2010-10-12
// PengJiLin, 2010-10-12
#define PROP_PEER_PAGE_S	        3

class  CRoomRightWnd:public CGameFaceGo
{
	CRgn		m_Rgn;		///< ��������
	int pannelHeight;		///< ���߰�ťһ���ĸ߶�
	COLORREF PropBgcolor;
	int	m_curPropPage;
	bool m_bInit;

	/// ������صİ�ť�ĺ�����
	int yPropRow1;  ///��һ�е���Yλ��
	int yPropRow2;  ///�ڶ��е���Yλ��
	int xPropBuy;	///< ����ť
	int xPropPrev;	///< ǰһҳ��ť
	int xProp1;		///< ��һ�����߰�ť
	int xProp2;		///< �ڶ������߰�ť
	int xProp3;		///< ���������߰�ť
    int xProp4;     // PengJiLin, 2010-10-11, ���ĵ��߰�ť
	int xPropNext;	///< ��һҳ��ť

	/// �Ӵ��ڵ�λ�ã�����ȷ�����߿��λ��
	CRect			m_rcUserList;		///<�û��б����ھ���
	CRect			m_rcTalkMessage;	///<�����¼���ھ���
	CRect			m_rcAdvertise;		///<���ؼ����ھ���
	CRect			m_rcSendTalk;		///<������������ھ���


	//��Ϣ����
protected:
	int										m_advHeight;												//���λ�ĸ߶�
	CString									m_strAdvertise;												//����ַ
	CWebWnd									*webWnd;

	CGameImage								m_userlc,m_userrc,m_usertc,m_userbc;						//���������
	CGameImage								m_userlt,m_userrt,m_userlb,m_userrb;						//����������Ľ�
	CGameImage								m_talklc,m_talkrc,m_talktc,m_talkbc;						//������
	CGameImage								m_talklt,m_talkrt,m_talklb,m_talkrb;						//�������Ľ�
	CGameImage								m_rightspace;												//�߼�
	CGameImage								m_rightspace2;												//�߼�

	bool									m_bHideList;						//�����б�


    // PengJiLin, 2010-9-28
    CGameImage  m_propBackImage;        // ����������ͼ�������
    CRect       m_propBackRect;


    // PengJiLin, 2010-10-8
    UINT        m_adjPropHeight;        // �������߶ȵ���
    int         m_iBackImageLeft;       // ����ͼ��λ��
    int         m_iBackImageWidth;
    int         m_iBackImageHeight;


    // PengJiLin, 2010-10-8, �̳����߰�ť
    CNormalBitmapButtonEx   m_btPropPrev_S;
    CNormalBitmapButtonEx   m_btPropNext_S;
    CNormalBitmapButtonEx   m_btProp1_S;
    CNormalBitmapButtonEx   m_btProp2_S;
    CNormalBitmapButtonEx   m_btProp3_S;

    int m_curPropPage_S;

    // PengJiLin, 2010-10-11, �̳�����λ��
    int xPropPrev_S;
    int xProp1_S;
    int xProp2_S;
    int xProp3_S;
    int xPropNext_S;

	//CGameImage								m_infolc,m_inforc,m_infotc,m_infobc;	//����ͷ���߽�
	//CGameImage								m_infolt,m_infort,m_infolb,m_inforb;	//����ͷ���߽�
	//CGameImage								m_infol,m_infor;						//����ͷ�������
	//CGameImage								m_LogoImg,m_PersonalInfo;				//����ͷ����ͷ��

	enum {IDD=IDD_ROOM_RIGHT_WND};
//	CComboBox								m_InputMessage;							//������Ϣ
public:
	CWnd									*m_pMainWnd;											//������ָ��
	CUserListCtrl							m_UserList;								//�û��б�
	CNewMiniDlg								m_TalkMessage;						//������Ϣ
	CNormalBitmapButtonEx					m_BtHideRight;						//���ذ�ť
	CToolTipCtrl							m_ToolTip;							//��ʾ��Ϣ

	TCHAR									szPropEmptyImage[MAX_PATH];
	CNormalBitmapButtonEx					m_btPropBuy;
	CNormalBitmapButtonEx					m_btPropPrev;
	CNormalBitmapButtonEx					m_btProp1;
	CNormalBitmapButtonEx					m_btProp2;
	CNormalBitmapButtonEx					m_btProp3;
    CNormalBitmapButtonEx                   m_btProp4;      // PengJiLin, 2010-10-11, ����4�����߰�ť
	CNormalBitmapButtonEx                   m_btProp5;
	CNormalBitmapButtonEx                   m_btProp6;
	CNormalBitmapButtonEx                   m_btProp7;
	CNormalBitmapButtonEx                   m_btProp8;
	CNormalBitmapButtonEx					m_btPropNext;
	UINT									m_BtPropArray[PROP_PEER_PAGE]; // PengJiLin, 2010-10-12. //Ϊ�˷���,���Խ��ĸ���ť�ŵ�������


	void OnBnClickedPropBuy();
	void OnBnClickedPropPrev();	//��������һ����ǰ
	void OnBnClickedPropNext();	//							
    void OnBnClickedLeftArrow(void);//�ڶ���  ��ǰ
    void OnBnClickedRightArrow(void);
	void OnBnClickedProp1();
	void OnBnClickedProp2();
	void OnBnClickedProp3();
    void OnBnClickedProp4();    
	void OnBnClickedProp5(); 
	void OnBnClickedProp6(); 
	void OnBnClickedProp7(); 
	void OnBnClickedProp8(); 


	//���ݴ����id�ţ���ʾ��ͬ�ĵ���  yjj 090309
	void OnPropBuy(UINT uPropId = 1);


	CChatControl							m_ChatControl;						//�������
//	CGameTalkText							m_ExpressWnd;						//���鴰��
	CAFCColorResource						m_colorresource;
	CLookingClass							m_MessageHandle;					//��Ϣ����
	//��������
public:
	//���캯��
	CRoomRightWnd();
	//���캯��
	CRoomRightWnd(UINT uDialogTemplata); 
	//��������
	virtual ~CRoomRightWnd();
	DECLARE_MESSAGE_MAP()
	//���ܺ���
public:
	//�ڲ�����
private:
	/// ������С
	bool FixControlStation(bool bShow=true);
	/// �����棬��OnPaint�е���
	/// @param pDC �豸������
	/// @param cx �ͻ�����
	/// @param cy �ͻ�����
	bool DrawViewFrame(CDC *pDC, int cx, int cy);
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);

	//��Ϣ���� 
public:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC){return true;}

	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
public:
	void LoadSkin(void);
	//��������λ��
	//void AutoSize(int cx,int cy);
public:
	//�����б�ť
	void OnBnClickedHideList();
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//������Ϣ
	LRESULT OnSendRoomMsg(WPARAM wparam, LPARAM lparam);

	//����
	LRESULT OnExchangeSkin(WPARAM wparam, LPARAM lparam);
	
	//����Ҽ��û��б���Ϣ
	afx_msg void OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//�������û��б���Ϣ
	afx_msg void OnNMLblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//���˫���û��б���Ϣ
	afx_msg void OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);


	BOOL OnToolTipNotify(UINT id, NMHDR * pTTTStruct, LRESULT * pResult );	//��ӦToolTip��Ϣ
	////����Ҽ������б���Ϣ
	//afx_msg void OnNMRclickFriendList(NMHDR * pNMHDR, LRESULT * pResult);
	////���˫�������б���Ϣ
	//afx_msg void OnNMDblclkFriendList(NMHDR * pNMHDR, LRESULT * pResult);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//�ض�talk.ini
	LRESULT OnReLoadTalkINI(WPARAM wparam, LPARAM lparam);

public:
	void OnSetPropImage(void);

    // PengJiLin, 2010-10-12, �̳����߹���
    void OnSetPropImage_S(void);

    void OnBnClickedBtnProp1S(void);
    void OnBnClickedBtnProp2S(void);
    void OnBnClickedBtnProp3S(void);
    void BuyAndUseProp(int nPropIndex);

public:
	void UserProp(int nPropIndex);

	void UserProp_Buy(int nPropIndex);
private:
	CSkinMgr m_skinmgr;


public:
	CWnd*	m_pGameRoom;
	void SetGameRoomWnd(CWnd* pWnd);
};

#endif