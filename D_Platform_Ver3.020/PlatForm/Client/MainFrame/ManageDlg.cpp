#include "Stdafx.h"
#include "ManageDlg.h"
#include "GameRoomEx.h"

BEGIN_MESSAGE_MAP(CManageDlg, CGameFaceGo)
//	ON_WM_CTLCOLOR()
ON_WM_LBUTTONDOWN()
ON_WM_PAINT()
ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()

/******************************************************************************************************************/

//���캯��
CManageDlg::CManageDlg(CWnd * pParent, GameInfoStruct * pGameInfo) : CGameFaceGo(IDD_MANAGE_DLG,pParent)
{
	m_dwUserID=0L;
	m_pGameInfo=pGameInfo;
	m_bkBrush=NULL;
	
}

//��������
CManageDlg::~CManageDlg()
{
	DeleteObject(m_bkBrush);
}

//DDX/DDV ֧��
void CManageDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BtOK);
	DDX_Control(pDX, IDCANCEL, m_BtCancel);
}

//��ʼ������
BOOL CManageDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	if (CUserPower::CanBindAccoun(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_BIND_USER)->EnableWindow(TRUE);//���ʺ�
	if (CUserPower::CanBindGame(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_BIND_GAME)->EnableWindow(TRUE);//����Ϸ
	if (CUserPower::CanLimiteRoomTalk(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_ROOM_TALK)->EnableWindow(TRUE);
	if (CUserPower::CanLimiteGameTalk(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_GAME_TALK)->EnableWindow(TRUE);
	if (CUserPower::CanLimiteOtherSend(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_SEND_MSG)->EnableWindow(TRUE);
	if (CUserPower::CanLimiteOtherWatch(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_WATCH_GAME)->EnableWindow(TRUE);
	if (CUserPower::CanLimiteOtherPlay(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_PLAY_GAME)->EnableWindow(TRUE);
	//if (CUserPower::CanLimiteIP(m_pGameInfo->dwMasterPower)==true) GetDlgItem(IDC_IP_PLAY)->EnableWindow(TRUE);
	
//	int r,g,b;
	CString s=CBcfFile::GetAppPath ();//����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	/*// 2012.12.17 yyf ����
	wsprintf(path,"%sdialog\\manager_bk.bmp",skinfolder);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CManageDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/

	wsprintf(path,"%sdialog\\apply.bmp",skinfolder);
	m_BtOK.LoadButtonBitmap(path,false);
	wsprintf(path,"%sdialog\\abandon.bmp",skinfolder);
	m_BtCancel.LoadButtonBitmap(path,false);
	CRect ClientRect;
	GetClientRect(&ClientRect);
	//this->MapWindowPoints(this->GetParent(),&ClientRect);

	/*// 2012.12.17 yyf ����
	CGameImageHelper	help(&m_bk);
	MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());*/

	MoveWindow(ClientRect.left,ClientRect.top,cx,cy);// 2012.12.17 yyf

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	SetDlgItemText(IDC_STATIC_AU,fMsg.GetKeyVal("ManageDlg","Authority","Ȩ�޲�����"));
	SetDlgItemText(IDC_STATIC_USERNAME,fMsg.GetKeyVal("ManageDlg","UserName","�û�����"));
	SetDlgItemText(IDC_ROOM_TALK,fMsg.GetKeyVal("ManageDlg","NoChatInHall","��ֹ��������"));
	SetDlgItemText(IDC_WATCH_GAME,fMsg.GetKeyVal("ManageDlg","NoBesideSee","��ֹ�Թ���Ϸ"));
	SetDlgItemText(IDC_GAME_TALK,fMsg.GetKeyVal("ManageDlg","NoGameChat","��ֹ��Ϸ����"));
	SetDlgItemText(IDC_PLAY_GAME,fMsg.GetKeyVal("ManageDlg","ForbidGame","��ֹ������Ϸ"));
	SetDlgItemText(IDC_SEND_MSG,fMsg.GetKeyVal("ManageDlg","NoSendMsg","��ֹ������Ϣ"));
	SetDlgItemText(IDC_STATIC_OPNOTE,
		fMsg.GetKeyVal("ManageDlg","OperationNote",
		"��ѡ��Ҫ���õ�Ȩ�ޣ�Ȼ��ѡ����Ч��ʽ�����������ʱ�䣬��ȷ�����ϣˡ�\r\n��ȥ��Ȩ�޺���Ч��ʽ��������ʱ�䣬��ȷ�����ɽ����"));
	SetDlgItemText(IDC_BIND_USER,fMsg.GetKeyVal("ManageDlg","EffectEW","���κεط�����Ч"));
	SetDlgItemText(IDC_BIND_GAME,fMsg.GetKeyVal("ManageDlg","EffectSameTypeGame","ֻ��ͬ����Ϸ����Ч"));
	SetDlgItemText(IDC_BIND_ROOM,fMsg.GetKeyVal("ManageDlg","EffectInHall","ֻ�ڸô�����Ч"));
	SetDlgItemText(IDC_STATIC2,fMsg.GetKeyVal("ManageDlg","LockTime","����ʱ�䣺"));
	SetDlgItemText(IDC_STATIC3,fMsg.GetKeyVal("ManageDlg","Min","����"));


	m_bkBrush=CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(37,65));//m_bk
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	return TRUE;
}

//ȷ������
void CManageDlg::OnOK()
{
	//�������� 
	MSG_GR_SR_GamePowerSet PowerSet;
	memset(&PowerSet,0,sizeof(PowerSet));

	//Ч������
	if (m_dwUserID==0L) return;

	//Ч������
	if (((CButton *)GetDlgItem(IDC_ROOM_TALK))->GetCheck()==BST_CHECKED) CUserPower::SetCanRoomTalk(PowerSet.dwGamePowerID,FALSE);
	if (((CButton *)GetDlgItem(IDC_GAME_TALK))->GetCheck()==BST_CHECKED) CUserPower::SetCanGameTalk(PowerSet.dwGamePowerID,FALSE);
	if (((CButton *)GetDlgItem(IDC_SEND_MSG))->GetCheck()==BST_CHECKED) CUserPower::SetCanSendMessage(PowerSet.dwGamePowerID,FALSE);
	if (((CButton *)GetDlgItem(IDC_WATCH_GAME))->GetCheck()==BST_CHECKED) CUserPower::SetCanWatchGame(PowerSet.dwGamePowerID,FALSE);
	if (((CButton *)GetDlgItem(IDC_PLAY_GAME))->GetCheck()==BST_CHECKED) CUserPower::SetCanPlayGame(PowerSet.dwGamePowerID,FALSE);
	//if (((CButton *)GetDlgItem(IDC_IP_PLAY))->GetCheck()==BST_CHECKED) CUserPower::SetCanLimiteIP(PowerSet.dwGamePowerID,FALSE);

	//Ч������
	if (((CButton *)GetDlgItem(IDC_BIND_USER))->GetCheck()==BST_CHECKED) PowerSet.bBindKind|=0x04;
	if (((CButton *)GetDlgItem(IDC_BIND_GAME))->GetCheck()==BST_CHECKED) PowerSet.bBindKind|=0x02;
	if (((CButton *)GetDlgItem(IDC_BIND_ROOM))->GetCheck()==BST_CHECKED) PowerSet.bBindKind|=0x01;

	PowerSet.uLockTime=(UINT)GetDlgItemInt(IDC_TIME);
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	if (PowerSet.uLockTime==0)
	{
		//AFCMessageBox(fMsg.GetKeyVal("ManageDlg","LimitTimeErrorRewrite","����ʱ�������������д��"));
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("ManageDlg","LimitTimeErrorRewrite","����ʱ�������������д��"));
		GetDlgItem(IDC_TIME)->SetFocus();
		return;
	}
	if (PowerSet.uLockTime>14400)
	{
		//AFCMessageBox(fMsg.GetKeyVal("ManageDlg","LimitTenDays","����ʱ�䳬��10�죬��ʹ�ú�̨����ϵͳʵ�����Ĳ�����"));
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("ManageDlg","LimitTenDays","����ʱ�䳬��10�죬��ʹ�ú�̨����ϵͳʵ�����Ĳ�����"));
		GetDlgItem(IDC_TIME)->SetFocus();
		return;
	}	
	
	if (PowerSet.bBindKind==0)
	{
		//AFCMessageBox(fMsg.GetKeyVal("ManageDlg","SelectBindingType","��ѡ������ͣ�"));
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("ManageDlg","SelectBindingType","��ѡ������ͣ�"));
		return;
	}

	//��������
	PowerSet.bShowActive=FALSE;
	PowerSet.dwUserID=m_dwUserID;
	m_pGameInfo->pISocketSend->SendGameData(&PowerSet,sizeof(PowerSet),MDM_GR_MANAGE,ASS_GR_POWER_SET,0);

	DestroyWindow();
}

//ȡ������
void CManageDlg::OnCancel()
{
	DestroyWindow();
}

//���ù����û�
void CManageDlg::SetManageUser(UserItemStruct * pUserItem)
{
	if (pUserItem!=NULL)
	{
		TCHAR szBuffer[50];
		memset(szBuffer,0,sizeof(szBuffer));
		m_dwUserID=pUserItem->GameUserInfo.dwUserID;
		wsprintf(szBuffer,TEXT("%d"),pUserItem->GameUserInfo.dwAccID);
		SetDlgItemText(IDC_USER_NAME,pUserItem->GameUserInfo.nickName);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
	{
		m_dwUserID=0;
		SetDlgItemText(IDC_USER_NAME,TEXT(""));
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	return;
}

/******************************************************************************************************************/

//HBRUSH CManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CGameFaceGo::OnCtlColor(pDC, pWnd, nCtlColor);
//	switch(nCtlColor)
//	{
//	case WM_CTLCOLORSCROLLBAR :
//	case WM_CTLCOLORBTN:
//	case WM_CTLCOLORMSGBOX:
//	case WM_CTLCOLOREDIT:
//	case WM_CTLCOLORLISTBOX:
//	case WM_CTLCOLORDLG:
//	case WM_CTLCOLORSTATIC :
//		pDC->SetTextColor( RGB(255,0,0)) ;
//		pDC->SetBkColor(RGB(0,0,255));
//		return CreateSolidBrush(RGB(0,0,255));
//	}
//	return hbr;
//}

LRESULT CManageDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	//case WM_CTLCOLORSCROLLBAR :
	//case WM_CTLCOLORBTN:
	//case WM_CTLCOLORMSGBOX:
	//case WM_CTLCOLOREDIT:
	//case WM_CTLCOLORLISTBOX:
	//case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC :
		SetTextColor((HDC)wParam, RGB(0,0,0)) ;
		int id=::GetDlgCtrlID((HWND)lParam);
		if(id==IDC_BIND_USER||id==IDC_BIND_GAME||id==IDC_BIND_ROOM||id==IDC_STATIC2||id==IDC_STATIC3)
		{
			SetBkColor((HDC)wParam, m_configDlg.GetBKImageEX().GetPixel(68,214));//m_bk
			return (LRESULT)m_bkBrush;
		}
		SetBkColor((HDC)wParam, m_configDlg.GetBKImageEX().GetPixel(37,65));//m_bk
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CManageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	//CGameFaceGo::OnLButtonDown(nFlags, point);
}

void CManageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	//this->MapWindowPoints(this->GetParent(),&ClientRect);
	
	// 2012.12.17 yyf ����
	//CGameImageHelper	help(&m_bk);
	////this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	//help.BitBlt(dc.GetSafeHdc(),0,0);
	
	//�滭��ܵ�ͼ 2012.10.17 yyf
	m_configDlg.DrawViewFrame(&dc);
}
//����
LRESULT CManageDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	CString s=CBcfFile::GetAppPath ();//����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	/*// 2012.12.17 yyf ����
	wsprintf(path,"%sdialog\\manager_bk.bmp",skinfolder);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CManageDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	wsprintf(path,"%sdialog\\apply.bmp",skinfolder);
	m_BtOK.LoadButtonBitmap(path,false);
	wsprintf(path,"%sdialog\\abandon.bmp",skinfolder);
	m_BtCancel.LoadButtonBitmap(path,false);
	CRect ClientRect;
	GetClientRect(&ClientRect);

	/*// 2012.12.17 yyf ����
	CGameImageHelper	help(&m_bk);
	MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());*/
	
	MoveWindow(ClientRect.left,ClientRect.top,cx,cy);// 2012.12.17 yyf
	//OnPaint();
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	Invalidate();
	return LRESULT(0);
}

// ���ô���Բ�� 2012.10.15 yyf
void CManageDlg::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);
 
    //���Ͻ�
	m_arPoints[0].x = winRect.left;
	m_arPoints[0].y = winRect.top+3;
	m_arPoints[1].x = winRect.left+3;
	m_arPoints[1].y = winRect.top;

	//���Ͻ�
	m_arPoints[2].x = winRect.right-3;
	m_arPoints[2].y = winRect.top;
	m_arPoints[3].x = winRect.right;
	m_arPoints[3].y = winRect.top+3;

	//���½�
	m_arPoints[4].x = winRect.right;
	m_arPoints[4].y = winRect.bottom-4;
	m_arPoints[5].x = winRect.right-4;
	m_arPoints[5].y = winRect.bottom;

	//���½�
	m_arPoints[6].x = winRect.left+4;
	m_arPoints[6].y = winRect.bottom;
	m_arPoints[7].x = winRect.left;
	m_arPoints[7].y = winRect.bottom-4;

	HRGN hRgn = CreatePolygonRgn(m_arPoints, 8, WINDING);
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
}