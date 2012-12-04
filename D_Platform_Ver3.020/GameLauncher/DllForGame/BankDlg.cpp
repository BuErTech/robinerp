// BankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BankDlg.h"
//#include "MainRoomEx.h"

extern EXT_CLASS TCHAR *GlbGetNumString(TCHAR str[], __int64 nNum, int nPower, bool bHasSpace, TCHAR* chSpace);

// CBankDlg dialog

IMPLEMENT_DYNAMIC(CBankDlg, CDialog)

CBankDlg::CBankDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOGBANK, pParent)
	, m_hWndParent(pParent->m_hWnd)
	, m_BankMoney(0)
	, m_RoomMoney(0)
	, m_UserID(0)
	, m_RoomName("")
	, m_uComType(0)
	, m_pMeUserInfo(NULL)
{
	m_bkBrush=NULL;
}

CBankDlg::~CBankDlg()
{
	DeleteObject(m_bkBrush);
}

void CBankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtCancel);
	DDX_Control(pDX, IDC_CHECKOUT,m_btCheckOut);
	DDX_Control(pDX, IDC_BTN_TRANSFER,m_btTransfer);
	DDX_Control(pDX, IDC_QUERY,m_btQuery);
}

void CBankDlg::SetBankMoney(int count)
{
	m_BankMoney=count;
	//TCHAR buffer[50];
	//ltoa(m_BankMoney,buffer,10);
	//SetDlgItemTextA(IDC_EDIT_MONEY,buffer);
}
void CBankDlg::SetRoomMoney(int count)
{
	m_RoomMoney=count;
	//TCHAR buffer[50];
	//ltoa(m_RoomMoney,buffer,10);
	//SetDlgItemTextA(IDC_EDIT_MONEY_ROOM,buffer);
}
int CBankDlg::GetBankMoney()
{
	return m_BankMoney;
}
void CBankDlg::GetUserID(long UserID)
{
	m_UserID=UserID;
}
//��ʼ������
BOOL CBankDlg::OnInitDialog()
{
	__super::OnInitDialog();
	TCHAR buffer[128], szTip[128];

	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "skin.bcf");
	CString key=Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	//skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\");
	skinfolder.Format("%s%s", "..\\", f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\"));
	CGameImage	m_bt;

	wsprintf(path,"%sdialog\\msg_bk.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	//wsprintf(path,"%sdialog\\WalletCancle_bt.BMP",skinfolder);
	//m_bt.SetLoadInfo(path,true);
	//m_btcancel.LoadButtonBitmap(path,false);
//	CGameImage		m_bt;
	HRGN hRgn2;
	wsprintf(path,"%sdialog\\win_close.bmp",skinfolder);
	m_BtCancel.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//hRgn2=AFCBmpToRgn(m_bt,m_bt.GetPixel(0,0),RGB(1,1,1));
	//if(hRgn2)
	//	m_BtCancel.SetWindowRgn(hRgn2,true);
	m_BtCancel.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//wsprintf(path,"%sdialog\\OutMoney_bt.bmp",skinfolder);
	//m_bt.SetLoadInfo(path,true);
	wsprintf(path,"%slogon\\button.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
//	CGameImageHelper bthelp(&m_bt);
	hRgn2=AFCBmpToRgn(m_bt,m_bt.GetPixel(0,0),RGB(1,1,1));
	m_btCheckOut.LoadButtonBitmap(path,false);
	if(hRgn2)
		m_btCheckOut.SetWindowRgn(hRgn2,true);
	//m_btCheckOut.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//wsprintf(path,"%sdialog\\InMoney_bt.bmp",skinfolder);
	//m_bt.SetLoadInfo(path,true);
	//m_btCheckIn.LoadButtonBitmap(path,false);
	//if(hRgn2)
	//	m_btCheckIn.SetWindowRgn(hRgn2,true);

	m_btQuery.LoadButtonBitmap(path,false);
	if(hRgn2)
		m_btQuery.SetWindowRgn(hRgn2,true);
	//m_btCheckIn.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	
	m_btTransfer.LoadButtonBitmap(path,false);
	if(hRgn2)
		m_btTransfer.SetWindowRgn(hRgn2,true);
	//m_btTransfer.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	ZeroMemory(buffer,sizeof(buffer));

	GlbGetNumString(buffer, m_BankMoney, Glb().m_nPowerOfGold/*, true*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	SetDlgItemTextA(IDC_EDIT_MONEY,buffer);
	GlbGetNumString(buffer, m_RoomMoney, Glb().m_nPowerOfGold/*, true*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	SetDlgItemTextA(IDC_EDIT_MONEY_ROOM,buffer);
	ZeroMemory(buffer,sizeof(buffer));
	switch(Glb().m_nPowerOfGold)
	{
	case 1:
		_tcscat( buffer, TEXT("ʮ"));
		break;
	case 2:
		_tcscat( buffer, TEXT("��"));
		break;
	case 3:
		_tcscat( buffer, TEXT("ǧ"));
		break;
	case 4:
		_tcscat( buffer, TEXT("��"));
		break;
	}
	_tcscpy(szTip, "��ȡ��С��λ��");
	_tcscat(szTip, buffer);
	_tcscat( buffer, TEXT("���"));
	SetDlgItemTextA(IDC_STATIC_TIP, szTip);

	m_bkBrush=CreateSolidBrush(m_bkimage.GetPixel(32,55));
	//_tcscpy( buffer, TEXT("1"));
	//for (int i=0; i<Glb().m_nPowerOfGold; ++i)
	//{
	//	_tcscat( buffer, TEXT("0"));
	//}
	//wsprintf( szTip, "X %s", buffer);
	//SetDlgItemTextA(IDC_UNIT, szTip);
	return true;
}
BEGIN_MESSAGE_MAP(CBankDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECKOUT, &CBankDlg::OnBnClickedCheckout)
	ON_BN_CLICKED(IDC_BTN_TRANSFER, &CBankDlg::OnBnClickedTransfer)
	ON_BN_CLICKED(IDC_QUERY, &CBankDlg::OnBnClickedQuery)
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_CHECK_NUM, &CBankDlg::OnEnChangeCheckNum)
END_MESSAGE_MAP()


// CBankDlg message handlers

void CBankDlg::OnBnClickedCheckout()
{
	// TODO: Add your control notification handler code here
	if(m_BankMoney<=0)
	{
		if(IDOK == AFCMessageBox("�����޽�ң����ֵ!","����",AFC_YESNO))
		{
			CString s=CBcfFile::GetAppPath ();/////����·��
			CBcfFile f( s + "../bzgame.bcf");
			CString key="BZW";
			CString url;
			TCHAR szUrl[20];
			wsprintf(szUrl,"bankurl");
			url=f.GetKeyVal(key,szUrl,Glb().m_CenterServerPara.m_strHomeADDR);
			//IE(url);
			ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
		}
		return;
	}

	/// ȡ�ñ༭������ַ���
	TCHAR szMsg[256];
	CString strNum;
	GetDlgItemText(IDC_CHECK_NUM, strNum);
	TCHAR szTail[16];
	_tcscpy(szTail, TEXT(""));
	for (int i=0; i<Glb().m_nPowerOfGold; ++i)
	{
		_tcscat( szTail, TEXT("0"));
	}
	/// �������̫��
	if (strNum.GetLength()<Glb().m_nPowerOfGold)
	{
		wsprintf(szMsg, "���޷�ȡ��������Ľ�ң�����������%dλ����ĩ%dλΪ %s ����������", Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// �������̫��
	if (strNum.GetLength()>10+Glb().m_nPowerOfGold)
	{
		wsprintf(szMsg, "�����������̫�󣬳�������Χ�������벻����%d%s����ĩ%dλΪ %s ����������", 0x7FFFFFFF, szTail, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	/// ���ĩ��λ������
	CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
	if (strSubNum != szTail)
	{
		TCHAR szMsg[256];
		wsprintf(szMsg, "���޷�ȡ��������Ľ�ң�������ĩ%dλΪ %s ����������", Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// ��ȡǰNλ
	strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
	__int64 i64Num = atol(strSubNum.GetBuffer());
	if (i64Num>0x7FFFFFFF)
	{
		wsprintf(szMsg, "�����������̫�󣬳�������Χ�������벻����%d%s����ĩ%dλΪ %s ����������", 0x7FFFFFFF, szTail, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	/// ȡ���ַ���������
	MSG_GR_S_CheckMoney *pCheckMoney = new MSG_GR_S_CheckMoney();
	memset(pCheckMoney,0,sizeof(MSG_GR_S_CheckMoney));
	pCheckMoney->i64Count=i64Num;
	pCheckMoney->UserID=m_UserID;	
	GetDlgItem(IDC_CHECKOUT)->EnableWindow(false);
	//m_TCPSocket->SendData(&checkMoney,sizeof(checkMoney),MDM_GR_MONEY,ASS_GR_CHECK_OUT,0);
	::PostMessage(m_hWndParent, WM_BANK_CHECK_OUT, (WPARAM)pCheckMoney, sizeof(MSG_GR_S_CheckMoney));
}



//ת��
void CBankDlg::OnBnClickedTransfer()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "..\\bzgame.bcf");
	CString key="BZW";
	CString url;
	TCHAR szUrl[20];
	wsprintf(szUrl,"bankurl");
	url=f.GetKeyVal(key,szUrl,Glb().m_CenterServerPara.m_strHomeADDR);
	TCHAR surl[512];
	wsprintf( surl, "%s?userid=%d&token=%s&x=%d", url, m_pMeUserInfo->GameUserInfo.dwUserID, Glb().m_TML_SN, rand()%0x7fff);
	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return;
}


///��ѯ
void CBankDlg::OnBnClickedQuery()
{
	CString s=CBcfFile::GetAppPath();/////����·��
	CBcfFile f( s + "..\\bzgame.bcf");
	CString key="BZW";
	CString url;
	TCHAR szUrl[20];
	wsprintf(szUrl,"bankurl_Query");
	url=f.GetKeyVal(key,szUrl,Glb().m_CenterServerPara.m_strHomeADDR);
	TCHAR surl[512];
	wsprintf( surl, "%s?userid=%d&token=%s", url, m_pMeUserInfo->GameUserInfo.dwUserID, Glb().m_TML_SN);
	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return;
}

void CBankDlg::GetRoomName(TCHAR * roomName)
{
	m_RoomName=roomName;
}
void CBankDlg::getUserItem(UserItemStruct * pMeUserInf)
{
	m_pMeUserInfo=pMeUserInf;
}
bool CBankDlg::GetRoomType(int uComType)
{
	if(uComType<1||uComType>3)
	{
		AFCMessageBox("�������ʹ���!");
		return false;
	}
	m_uComType=uComType;
	return true;
}

void CBankDlg::UpdateUserMoney(int OptType, long nMoney)
{
	if( OptType == 0 ) //ȡ��
	{
		m_BankMoney -= nMoney;
		m_RoomMoney +=nMoney;
	}
	else //����
	{
		m_BankMoney += nMoney;
		m_RoomMoney -= nMoney;
	}
	if( GetSafeHwnd() != NULL )
	{
		CString strBank, strRoom;
		TCHAR szNum[128];
		GlbGetNumString(szNum, m_BankMoney, Glb().m_nPowerOfGold/*, true*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strBank.Format("%s", szNum);
		GlbGetNumString(szNum, m_RoomMoney, Glb().m_nPowerOfGold/*, true*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strRoom.Format("%s", szNum);
		GetDlgItem(IDC_EDIT_MONEY)->SetWindowText(strBank);
		GetDlgItem(IDC_EDIT_MONEY_ROOM)->SetWindowText(strRoom);
	}
}

void CBankDlg::OnPaint()
{
	CPaintDC dc(this);
	CGameImageHelper	handle(&m_bkimage);
	handle.BitBlt(dc.GetSafeHdc(),0,0);

//	CFont Font;//,*OldFont;
//	Font.CreateFont(15,10,0,0,0,0,0,0,GB2312_CHARSET
//	,3,2,1,2,TEXT("����"));
//	CWnd *pWnd=GetDlgItem(IDC_EDIT_MONEY);
//	CDC *pDC=pWnd->GetDC();
//	pDC->SelectObject(&Font);
//	pWnd->SetFont(&Font,true);
////	DeleteObject(pDC->SelectObject(OldFont));
//	ReleaseDC(pDC);
//
//	pWnd=GetDlgItem(IDC_EDIT_MONEY_ROOM);
//	pDC=pWnd->GetDC();
//	pDC->SelectObject(&Font);
//	pWnd->SetFont(&Font,true);
////	DeleteObject(pDC->SelectObject(OldFont));
//	ReleaseDC(pDC);

	CDC *pdc=this->GetWindowDC();
	if(!pdc)
		return;
	CRect rc;
	this->GetWindowRect(&rc);
	Graphics g(pdc->m_hDC);

	CString s="����";
	Font font(s.AllocSysString(),12,0,UnitPixel);
	SolidBrush brush(Color(255,255,255));
	s="��������";
	g.DrawString(s.AllocSysString(),-1,&font,PointF(8,8),0,&brush);
	g.ReleaseHDC(pdc->m_hDC);

}
LRESULT CBankDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetBkColor((HDC)wParam, m_bkimage.GetPixel(32,55));
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CBankDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	
}

void CBankDlg::OnEnChangeCheckNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//long lNum = GetDlgItemInt(IDC_CHECK_NUM);
	//lNum *= 100;
	////GetDlgItem(IDC_CHECK_NUM)->SetWindowText(lNum);
	//SetDlgItemInt(IDC_CHECK_NUM, lNum);
}
