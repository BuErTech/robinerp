#include "StdAfx.h"
#include "ControlDlg.h"
#include "ClientGameDlg.h"

//�궨��
#define BT_WIDTH								40						//��ť���
#define BT_HEIGHT								22						//��ť�߶�
#define BT_SAPCE								5						//��ť���
#define BT_POINT_WIDTH							28						//������
#define BT_POINT_HEIGHT							28						//����߶�
#define BT_POINT_SAPCE							2						//������

BEGIN_MESSAGE_MAP(CControlDlg, CAFCRgnDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BEGIN,OnBegin)
END_MESSAGE_MAP()

//���캯��
CControlDlg::CControlDlg() : CAFCRgnDialog(IDD_CONTROL_DLG)
{
	m_dwMaxMoney=0;
	m_pHandleWnd=NULL;
}

//��������
CControlDlg::~CControlDlg()
{
}

//���ݰ󶨺���
void CControlDlg::DoDataExchange(CDataExchange * pDX)
{
	CAFCRgnDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BEGIN, m_BtBegin);
	DDX_Control(pDX, IDC_GIVE_UP, m_BtGiveUp);
	DDX_Control(pDX, IDC_SHOW_HAND, m_BtShowHand);
	DDX_Control(pDX, IDC_SELECT, m_BtSelect);
	DDX_Control(pDX, IDC_FOLLOW, m_BtFollow);
	DDX_Control(pDX, IDC_POINT_1, m_BtPoint1);
	DDX_Control(pDX, IDC_POINT_5, m_BtPoint5);
	DDX_Control(pDX, IDC_POINT_10, m_BtPoint10);
	DDX_Control(pDX, IDC_POINT_50, m_BtPoint50);
	DDX_Control(pDX, IDC_POINT_100, m_BtPoint100);
	DDX_Control(pDX, IDC_POINT_500, m_BtPoint500);
}

//��ʼ������
BOOL CControlDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	HINSTANCE hDllResource=GetModuleHandle(CLIENT_DLL_NAME);
	LoadDialogBitmap(hDllResource,IDB_CONTROL_BACK,RGB(255,255,255),RGB(255,255,255));
	m_BtPoint1.LoadButtonBitmap(hDllResource,IDB_POINT_1,false);
	m_BtPoint5.LoadButtonBitmap(hDllResource,IDB_POINT_5,false);
	m_BtPoint10.LoadButtonBitmap(hDllResource,IDB_POINT_10,false);
	m_BtPoint50.LoadButtonBitmap(hDllResource,IDB_POINT_50,false);
	m_BtPoint100.LoadButtonBitmap(hDllResource,IDB_POINT_100,false);
	m_BtPoint500.LoadButtonBitmap(hDllResource,IDB_POINT_500,false);

	return TRUE;
}

//λ�øı���Ϣ
void CControlDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�ƶ�����
	int iXBasePos=35,iYBasePos=7;
	SafeMoveWindow(&m_BtPoint1,iXBasePos,iYBasePos,BT_POINT_WIDTH,BT_POINT_HEIGHT);
	SafeMoveWindow(&m_BtPoint5,iXBasePos+BT_POINT_SAPCE+BT_POINT_WIDTH,iYBasePos,BT_POINT_WIDTH,BT_POINT_HEIGHT);
	SafeMoveWindow(&m_BtPoint10,iXBasePos+BT_POINT_SAPCE*2+BT_POINT_WIDTH*2,iYBasePos,BT_POINT_WIDTH,BT_POINT_HEIGHT);
	SafeMoveWindow(&m_BtPoint50,iXBasePos+BT_POINT_SAPCE*3+BT_POINT_WIDTH*3,iYBasePos,BT_POINT_WIDTH,BT_POINT_HEIGHT);
	SafeMoveWindow(&m_BtPoint100,iXBasePos+BT_POINT_SAPCE*4+BT_POINT_WIDTH*4,iYBasePos,BT_POINT_WIDTH,BT_POINT_HEIGHT);
	SafeMoveWindow(&m_BtPoint500,iXBasePos+BT_POINT_SAPCE*5+BT_POINT_WIDTH*5,iYBasePos,BT_POINT_WIDTH,BT_POINT_HEIGHT);

	//�ƶ���ť
	iXBasePos=228,iYBasePos=9;
	SafeMoveWindow(&m_BtFollow,iXBasePos,iYBasePos,BT_WIDTH,BT_HEIGHT);
	SafeMoveWindow(&m_BtSelect,iXBasePos+BT_SAPCE+BT_WIDTH,iYBasePos,BT_WIDTH,BT_HEIGHT);
	SafeMoveWindow(&m_BtShowHand,iXBasePos+BT_SAPCE*2+BT_WIDTH*2,iYBasePos,BT_WIDTH,BT_HEIGHT);
	SafeMoveWindow(&m_BtGiveUp,iXBasePos+BT_SAPCE*3+BT_WIDTH*3,iYBasePos,BT_WIDTH,BT_HEIGHT);
	SafeMoveWindow(&m_BtBegin,iXBasePos+BT_SAPCE*4+BT_WIDTH*4,iYBasePos,BT_WIDTH,BT_HEIGHT);

	return;
}

//����Ҽ�������Ϣ
void CControlDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	return;
}

//��ʼ
void CControlDlg::OnBegin()
{
	m_pHandleWnd->SendMessage(IDM_BEGIN_GAME,0,0);
	return;
}

//��������
void CControlDlg::SetEnableControl(bool bEnableControl, bool bShowHand)
{
	m_BtFollow.EnableWindow(bEnableControl);
	m_BtSelect.EnableWindow(bEnableControl);
	m_BtGiveUp.EnableWindow(bEnableControl);
	m_BtShowHand.EnableWindow(bShowHand);
	return;
}

//���ý��
void CControlDlg::SetMaxMoney(long int dwMaxPoint)
{
	m_dwMaxMoney=dwMaxPoint;
	m_BtPoint1.EnableWindow(dwMaxPoint>=1);
	m_BtPoint5.EnableWindow(dwMaxPoint>=5);
	m_BtPoint10.EnableWindow(dwMaxPoint>=10);
	m_BtPoint50.EnableWindow(dwMaxPoint>=50);
	m_BtPoint100.EnableWindow(dwMaxPoint>=100);
	m_BtPoint500.EnableWindow(dwMaxPoint>=500);
	return;
}


