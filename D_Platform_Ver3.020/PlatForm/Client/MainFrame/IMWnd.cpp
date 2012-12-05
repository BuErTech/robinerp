#include "IMWnd.h"
#include "IMWnd.h"
// IMWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IMWnd.h"


// CIMWnd �Ի���

IMPLEMENT_DYNAMIC(CIMWnd, CDialog)

CIMWnd::CIMWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CIMWnd::IDD, pParent)
{

}

CIMWnd::~CIMWnd()
{
	DeleteObject(m_bkBrush);
}

void CIMWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON2, m_Add);
	
	DDX_Control(pDX, IDC_BUTTON3, m_Set);

	DDX_Control(pDX, IDCANCEL2, m_Close);
}


BEGIN_MESSAGE_MAP(CIMWnd, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(IDC_BUTTON2, &CIMWnd::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &CIMWnd::OnBnClickedSet)
	ON_BN_CLICKED(IDCANCEL2, &CIMWnd::OnBnClickedClose)

	ON_NOTIFY(NM_CLICK, IDC_IM_LIST, OnNMClickIMList)
	ON_NOTIFY(NM_RCLICK, IDC_IM_LIST, OnNMRclickIMList)//�һ���Ϸ�б�
	ON_NOTIFY(NM_DBLCLK, IDC_IM_LIST, OnNMDblclkIMList)//˫����Ϸ�б�
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()


// CIMWnd ��Ϣ�������

BOOL CIMWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//m_bnBuy.LoadButtonBitmap(szButton,false);
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CIMWnd::Init()
{
	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
	

	//-------��ʼ�� m_configDlg-----------------------------
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CIMWndConfig\\configDlg.bcf",s,skinfolder);//Glb().m_Path
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	
 
	////-----------------------------------------------------
	//CGameImage	m_bt;
	//wsprintf(path,"%sdialog\\im_bk.png",skinfolder);
	//m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	////------���õ�ͼԲ��--
	//CGameImageHelper ImageHandle(&m_bkimage); 
	//int cx,cy;
	//HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	//if (hRgn!=NULL)
	//{
	//	SetWindowRgn(hRgn,TRUE);
	//	///{{ Added by zxd 20100709 �ͷ��������
	//	DeleteObject(hRgn);
	//	///Added by zxd 20100709 �ͷ��������}}
	//}
	////---------------------

	int cx,cy;
	//cx=ImageHandle.GetWidth();
	//cy=ImageHandle.GetHeight();
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	 
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	SetWindowText("���ѹ���");
	 

	TCHAR szButton[MAX_PATH];
	sprintf(szButton, "%s\\%sdialog\\normal_bt.BMP",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
	m_Add.LoadButtonBitmap(szButton,false);
	m_Set.LoadButtonBitmap(szButton, false);
	CGameImage bt;
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Add.MoveWindow(114, 28, bt.GetWidth() / 4, bt.GetHeight());
	m_Set.MoveWindow(182, 28, bt.GetWidth() / 4, bt.GetHeight());

	CRect rc;
	GetClientRect(&rc);
	wsprintf(szButton,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_Close.LoadButtonBitmap(szButton,false);
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Close.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	m_bkBrush = CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(100,100));//
}

void CIMWnd::LoadSkin()
{
	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	CGameImage	m_bt;

	//wsprintf(path,"%sdialog\\im_bk.png",skinfolder);
	//m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//-------��ʼ�� m_configDlg-----------------------------
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CIMWndConfig\\configDlg.bcf",s,skinfolder);//Glb().m_Path
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	
	//-----------------------------------------------

	TCHAR szButton[MAX_PATH];
	sprintf(szButton, "%s\\%sdialog\\normal_bt.BMP",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
	m_Add.LoadButtonBitmap(szButton,false);
	m_Set.LoadButtonBitmap(szButton, false);
	CGameImage bt;
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Add.MoveWindow(114, 28, bt.GetWidth() / 4, bt.GetHeight());
	m_Set.MoveWindow(182, 28, bt.GetWidth() / 4, bt.GetHeight());

	CRect rc;
	GetClientRect(&rc);
	wsprintf(szButton,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_Close.LoadButtonBitmap(szButton,false);
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Close.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());
	
	SetWndRgn();// ���ô���Բ��2012.10.15 yyf
}
LRESULT CIMWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case WM_CTLCOLORSTATIC:
		{
			SetTextColor((HDC)wParam, RGB(0,0,0)) ;
			SetBkColor((HDC)wParam, m_configDlg.GetBKImageEX().GetPixel(100,100));
			return (LRESULT)m_bkBrush;
		}
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CIMWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CLoadFileBmp::OnPaint()
	
	//CGameImageHelper	help(&m_bkimage);
	//help.BitBlt(dc.GetSafeHdc(),0,0);
	 
	
	//�滭��ܵ�ͼ
	m_configDlg.DrawViewFrame(&dc);
}

void CIMWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

afx_msg void CIMWnd::OnBnClickedAdd()
{
	addFriendDlg.DoModal();
}
afx_msg void CIMWnd::OnBnClickedSet()
{
	setFriendDlg.DoModal();
}
afx_msg void CIMWnd::OnBnClickedClose()
{
	OnCancel();
}

//�һ���Ϸ�б�`
void CIMWnd::OnNMDblclkIMList(NMHDR * pNMHDR, LRESULT * pResult)
{
	try
	{
		CPoint p;
		HTREEITEM hti=m_IMList->GetSelectedItem();
		if(hti)
		{
			if(m_IMList->GetParentItem(hti)!=0 && m_IMList->GetItemData(m_IMList->GetParentItem(hti))==3)
			{//����Ǻ������е�
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				
				//AFCMessageBox(fMsg.GetKeyVal("IMDlg","NoChatWithBadGuyList","�Բ�����������������е��û�����"),fMsg.GetKeyVal("IMDlg","Tip","��ʾ"));
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("IMDlg","NoChatWithBadGuyList","�Բ�����������������е��û�����"));
			}
			else
			{
				int itemDate=m_IMList->GetItemData(hti);
				if(itemDate>1000)
				{
#ifndef MAINROOM_USE_SHORTMSG
					int nImg;
					m_IMList->GetItemImage(hti,nImg,nImg);
					CIMSession *imSession=m_IMList->openIMSession(itemDate-1000,m_IMList->GetItemText(hti));
					if(imSession)
					{
						if(nImg==3 || nImg==5)
							imSession->setUserOnline(true);
						if(nImg==4 || nImg==6)
							imSession->setUserOnline(false);
						imSession->ShowWindow(SW_SHOW);
					}
#else
					int dwUserid=m_IMList->GetItemData(hti);
					if(dwUserid<1000)
						return;
					dwUserid-=1000;
					CString sname=m_IMList->GetItemText(hti);
					m_IMList->openShortMsg(dwUserid,sname.GetBuffer(sname.GetLength()));
#endif
				}

			}
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
	*pResult = 0;

}

void CIMWnd::OnNMClickIMList(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		// TODO: Add your control notification handler code here
		UINT u_Flag;
		HTREEITEM    hItem;
		CPoint pt;
		GetCursorPos(&pt);  //�����Ļ�������
		m_IMList->ScreenToClient(&pt);//ת������
		hItem = m_IMList->HitTest(pt,&u_Flag);
		if(hItem != NULL)
		{
			m_IMList->Expand(hItem,TVE_TOGGLE);
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
	*pResult = 0;
}

//�һ�IM�б�`
void CIMWnd::OnNMRclickIMList(NMHDR * pNMHDR, LRESULT * pResult)
{
	try
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CPoint point;
		GetCursorPos(&point);
		m_IMList->ScreenToClient(&point);
		HTREEITEM hti=m_IMList->HitTest(point);
		if(!hti)
			return;
		int dwUserid=m_IMList->GetItemData(hti);
		if(dwUserid<1000)
			return;
		dwUserid-=1000;
		CString sname=m_IMList->GetItemText(hti);
		CMenu menu;
		menu.CreatePopupMenu();
		MENUINFO mi;
		menu.GetMenuInfo(&mi);
		mi.cbSize=sizeof(MENUINFO);
		mi.fMask=MIM_BACKGROUND; 
		mi.hbrBack = CreateSolidBrush(RGB(0xD1,0xF1,0xFF));
		menu.SetMenuInfo(&mi);

		if((m_IMList->GetParentItem(hti)==m_IMList->htiFamilier)
			||(m_IMList->GetParentItem(hti)==m_IMList->htiUnfamilier))
		{
			menu.AppendMenu(MF_STRING,10,fMsg.GetKeyVal("IMDlg","Chat","����"));
		}
		menu.AppendMenu(MF_STRING,11,fMsg.GetKeyVal("IMDlg","Delete","ɾ��"));
		if(m_IMList->GetParentItem(hti)!=m_IMList->htiFamilier)
		{
			menu.AppendMenu(MF_STRING,21,fMsg.GetKeyVal("IMDlg","ToFriend","�������"));
		}
		if(m_IMList->GetParentItem(hti)!=m_IMList->htiBlack)
		{
			menu.AppendMenu(MF_STRING,22,fMsg.GetKeyVal("IMDlg","ToBadGuyList","���������"));
		}

		CPoint p; 
		::GetCursorPos(&p);
		int ret=menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD,p.x,p.y,this);

		switch(ret)
		{
		case 10:
			{
#ifndef MAINROOM_USE_SHORTMSG
				int nImg;
				m_IMList->GetItemImage(hti,nImg,nImg);
				CIMSession *imSession=m_IMList->openIMSession(dwUserid,sname);//
				if(imSession)
				{
					if(nImg==3 || nImg==5)
						imSession->setUserOnline(true);
					if(nImg==4 || nImg==6)
						imSession->setUserOnline(false);
					imSession->ShowWindow(SW_SHOW);
				}
#else
				m_IMList->openShortMsg(dwUserid,sname.GetBuffer(sname.GetLength()));
#endif
			}
			break;
		case 11:
			{
				//if(AFCMessageBox(fMsg.GetKeyVal("IMDlg","R_U_SureDelFriend","��ȷ��Ҫɾ���ú�����"),
				//	fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),AFC_YESNO)==IDOK)
				if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),true,fMsg.GetKeyVal("IMDlg","R_U_SureDelFriend","��ȷ��Ҫɾ���ú�����"))==IDOK)
					m_IMList->SetUserGroup(dwUserid,NULL,0);
			}
			break;
		case 21:
			m_IMList->SetUser(dwUserid,"",true);
			break;
		case 22:
			{
				//if(AFCMessageBox(fMsg.GetKeyVal("IMDlg","MoveFriendToBadList","��ȷ��Ҫ���ú��ѷŵ���������"),
				//	fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),AFC_YESNO)==IDOK)
				if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),true,fMsg.GetKeyVal("IMDlg","MoveFriendToBadList","��ȷ��Ҫ���ú��ѷŵ���������"))==IDYES)
				{
					m_IMList->SetUserGroup(dwUserid,NULL,3);
					CString szMsg;
					szMsg.Format(fMsg.GetKeyVal("IMDlg","MoveOneToBadList","���[%s]�Ѿ����뵽��ĺ�������"), sname);
					AFCTimerMsgBox(5, szMsg);
				}
			}
			break;
		}

		DeleteObject(mi.hbrBack);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}


	*pResult = 0;
}


void CIMWnd::SetIM(CIMMain* pIM)
{
	m_IMList = pIM;
}
//����
LRESULT CIMWnd::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	Invalidate();
	return LRESULT(0);
}


// ���ô���Բ��2012.10.15 yyf
void CIMWnd::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);

	//m_arPoints[0].x = winRect.left;
	//m_arPoints[0].y = winRect.top+4;
	//m_arPoints[1].x = winRect.left+4;
	//m_arPoints[1].y = winRect.top;
	//m_arPoints[2].x = winRect.right-4;
	//m_arPoints[2].y = winRect.left;
	//m_arPoints[3].x = winRect.right;
	//m_arPoints[3].y = winRect.left+4;
	//m_arPoints[4].x = winRect.right;
	//m_arPoints[4].y = winRect.bottom-4;
	//m_arPoints[5].x = winRect.right-4;
	//m_arPoints[5].y = winRect.bottom;
	//m_arPoints[6].x = winRect.left+4;
	//m_arPoints[6].y = winRect.bottom;
	//m_arPoints[7].x = winRect.left;
	//m_arPoints[7].y = winRect.bottom-4;

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
