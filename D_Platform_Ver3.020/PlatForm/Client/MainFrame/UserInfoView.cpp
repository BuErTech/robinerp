#include "Stdafx.h"
#include "Resource.h"
#include "UserInfoView.h"

BEGIN_MESSAGE_MAP(CUserInfoView, CAFCBaseDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_UPDATE_INFO, OnBnClickedUpdate)
END_MESSAGE_MAP()

//���캯��
CUserInfoView::CUserInfoView(GetOrderName * pOrderName) : CAFCBaseDialog(IDD_USER_INFO), m_FlashView(this)
{
	m_pOrderName=pOrderName;
	return;
}

//��������
CUserInfoView::~CUserInfoView()
{
}

//���ݰ�
void CUserInfoView::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX,IDCANCEL,m_BtCancel);
	DDX_Control(pDX,IDC_HIGH_INFO,m_BtHighInfo);
	DDX_Control(pDX,IDC_UPDATE_INFO,m_BtUpdateInfo);
}

//��ʼ������
BOOL CUserInfoView::OnInitDialog()
{
	__super::OnInitDialog();

	//�ƶ��ؼ�
	CRect ClientRect;
	GetClientRect(&ClientRect);
	FixControlSize(ClientRect.Width(),ClientRect.Height());

	//���ÿؼ�
	m_FlashView.CreateFalsh(this);
	m_FlashView.ShowWindow(SW_SHOW);
	m_FlashView.MoveWindow(10,10,140,190);
	//m_FlashView.GetFlashControl().SetMovie(TEXT("http://Avatar.yhgame.cn/AvatarSystem/YHAvatar.SWF"));
//	m_FlashView.GetFlashControl().SetMovie(TEXT("http://192.168.0.55/Jamser/AvatarSwf/Avatar3.swf"));
//	m_FlashView.GetFlashControl().SetVariable(TEXT("pathvar"),TEXT("http://Avatar.yhgame.cn/AvatarSystem/"));
//	m_FlashView.GetFlashControl().SetVariable(TEXT("codevar"),TEXT("4-0-0-u-u-u-u-4-u-3-0-25-0-30-0-0-0-0-0-0-0-0-0-0-0-0"));

	return TRUE;
}

//�滭��Ϣ
void CUserInfoView::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CRect ClientRect;
	GetClientRect(&ClientRect);
	DrawBackFace(&dc,0,0,ClientRect.Width(),ClientRect.Height());

	return;
}

//�����û���Ϣ
void CUserInfoView::SetUserInfo(UserItemStruct * pUserItem)
{
	//��д�û�����
	CString strBuffer;
	SetDlgItemText(IDC_NAME,pUserItem->GameUserInfo.szName);
	strBuffer.Format(TEXT("%I64d"),pUserItem->GameUserInfo.dwAccID);
	SetDlgItemText(IDC_ACCID,strBuffer);
	SetDlgItemText(IDC_ORDER,m_pOrderName(pUserItem->GameUserInfo.dwPoint));
	strBuffer.Format(TEXT("%I64d"),pUserItem->GameUserInfo.dwPoint);
	SetDlgItemText(IDC_POINT,strBuffer);
	if (pUserItem->GameUserInfo.szClassName[0]==0) SetDlgItemText(IDC_CLASS,TEXT("û�м�������"));
	else SetDlgItemText(IDC_CLASS,pUserItem->GameUserInfo.szClassName);
	SetDlgItemInt(IDC_WIN,pUserItem->GameUserInfo.uWinCount);
	SetDlgItemInt(IDC_LOST,pUserItem->GameUserInfo.uLostCount);
	SetDlgItemInt(IDC_CUT,pUserItem->GameUserInfo.uCutCount);
	SetDlgItemInt(IDC_MID,pUserItem->GameUserInfo.uMidCount);
	SetDlgItemInt(IDC_EXPERIENCE,pUserItem->GameUserInfo.dwExperience);
	SetDlgItemInt(IDC_REPUTATION,pUserItem->GameUserInfo.dwReputation);
	strBuffer.Empty();
	if (pUserItem->GameUserInfo.bMember==1) strBuffer+=TEXT("��ͨ��Ա ");
	if (pUserItem->GameUserInfo.bMember==2) strBuffer+=TEXT("�߼���Ա ");
	if (pUserItem->GameUserInfo.bGameMaster==1) strBuffer+=TEXT("�ⲿ����Ա ");
	if (pUserItem->GameUserInfo.bGameMaster==2) strBuffer+=TEXT("�ڲ�����Ա ");
	SetDlgItemText(IDC_NOTE,strBuffer);

	//��ȡ�û�ͷ��


	return;
}

//λ����Ϣ
void CUserInfoView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�ƶ�����
	FixControlSize(cx,cy);

	return;
}

//����λ��
void CUserInfoView::FixControlSize(int iWidth, int iHeight)
{
	SafeMoveWindow(&m_FlashView,10,10,140,190);
	return;
}

//���°�ť
void CUserInfoView::OnBnClickedUpdate()
{
	//m_FlashView.GetFlashControl().SetVariable(TEXT("pathvar"),TEXT("http://Avatar.yhgame.cn/AvatarSystem/"));
	m_FlashView.GetFlashControl().SetVariable(TEXT("pathvar"),TEXT("http://192.168.0.55/Jamser/AvatarSwf/Avartar/"));
//	m_FlashView.GetFlashControl().SetVariable(TEXT("codevar"),TEXT("4-0-0-u-u-u-u-4-u-3-0-25-0-30-0-0-0-0-0-0-0-0-0-0-0-0"));
	m_FlashView.GetFlashControl().SetVariable(TEXT("userid"),TEXT("258"));
	m_FlashView.GetFlashControl().SetVariable(TEXT("sex"),TEXT("1"));
	return;
}
