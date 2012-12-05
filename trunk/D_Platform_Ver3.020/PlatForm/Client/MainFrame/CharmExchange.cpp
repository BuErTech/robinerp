#include "CharmExchange.h"
// CharmExchange.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CharmExchange.h"
#include "MainRoomEx.h"

// CCharmExchange �Ի���

IMPLEMENT_DYNAMIC(CCharmExchange, CDialog)

CCharmExchange::CCharmExchange(CWnd* pParent /*=NULL*/)
	: CDialog(CCharmExchange::IDD, pParent)
{
	m_bkBrush = NULL;
}

CCharmExchange::~CCharmExchange()
{
	DeleteObject(m_bkBrush);
}

void CCharmExchange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);

	DDX_Control(pDX, IDC_BUTTON1, m_Change);

	DDX_Control(pDX, IDCANCEL2, m_Cancel);

	DDX_Control(pDX, IDC_BUTTON2, m_Close);
}

int cmp(const TCharmExchange_Item& t1, const TCharmExchange_Item& t2)
{
	return t1.iID < t2.iID;
}

void CCharmExchange::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if(pNetHead->bMainID == MDM_GP_CHARMEXCHANGE)
	{
		if (pNetHead->bAssistantID == ASS_GETLIST)
		{
			if (pNetHead->bHandleCode == 0)
			{
				TCharmExchange_Item *_p = (TCharmExchange_Item*)pNetData;
				if (_p != NULL)
				{
					m_ChangeList.push_back(*_p);
				}
			}
			else if (pNetHead->bHandleCode == 1)
			{
				sort(m_ChangeList.begin(), m_ChangeList.end(), cmp);

				m_list.DeleteAllItems();
				
				for (int i = 0; i < m_ChangeList.size(); i++)
				{
					CString s;
					s.Format("%d", m_ChangeList[i].iPoint);
					int r = m_list.InsertItem(i, s);
					//s.Format("%s", CGlobal().PropIDtoName(m_ChangeList[i].iPropID));
					m_list.SetItemText(r, 1, Glb().PropIDtoName(m_ChangeList[i].iPropID));
					s.Format("%d", m_ChangeList[i].iPropCount);
					m_list.SetItemText(r, 2, s);
					
				}
			}
		}
		else if (pNetHead->bAssistantID == ASS_EXCHANGE)
		{
			UpdateFascination();
		}
	}
}




// CCharmExchange ��Ϣ�������

BOOL CCharmExchange::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CCharmExchange::Init()
{
	DWORD dwstyleEX = m_list.GetExtendedStyle();
	dwstyleEX |= LVS_EX_FULLROWSELECT;
	m_list.SetExtendedStyle(dwstyleEX);

	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	//lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//����styl


	//m_list.MoveWindow(12,58,410,100);
	m_list.InitListCtrl(RGB(194,212,234),RGB(0,0,0),RGB(249,175,40),RGB(0,0,0),"");
	m_list.SetProcessImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP6),MAKEINTRESOURCE(IDB_BITMAP7));
	m_list.InsertColumn(0, "���ѵ���", LVCFMT_CENTER, 90);
	m_list.InsertColumn(1, "�һ���Ʒ", LVCFMT_CENTER, 226);
	m_list.InsertColumn(2, "�һ�����", LVCFMT_CENTER, 90);

	m_list.InitListHeader(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1),MAKEINTRESOURCE(IDB_BITMAP1),RGB(255,255,255),RGB(255,255,255),1);


	//m_bnBuy.LoadButtonBitmap(szButton,false);
	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("Bank");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	CGameImage	m_bt;

/*2012.10.15 yyf //����
	wsprintf(path,"%sdialog\\meilidlg.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CCharmExchangeConfig\\configDlg.bcf",s,skinfolder);//Glb().m_Path
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	TCHAR szButton[MAX_PATH];
	sprintf(szButton, "%s\\%sdialog\\normal_bt.BMP",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
	m_Change.LoadButtonBitmap(szButton,false);
	m_Cancel.LoadButtonBitmap(szButton, false);

	CRect rc;
	GetClientRect(&rc);
	wsprintf(szButton,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_Close.LoadButtonBitmap(szButton,false);
	CGameImage bt;
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Close.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	m_bkBrush = CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(32,55));
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}

void CCharmExchange::LoadSkin()
{
	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("Bank");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	CGameImage	m_bt;
/*2012.10.15 yyf //����
	wsprintf(path,"%sdialog\\meilidlg.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	 
	CGameImageHelper ImageHandle(&m_bkimage);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();*/
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CCharmExchangeConfig\\configDlg.bcf",s,skinfolder);//Glb().m_Path
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	TCHAR szButton[MAX_PATH];
	sprintf(szButton, "%s\\%sdialog\\normal_bt.BMP",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
	m_Change.LoadButtonBitmap(szButton,false);
	m_Cancel.LoadButtonBitmap(szButton, false);

	CRect rc;
	GetClientRect(&rc);
	wsprintf(szButton,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_Close.LoadButtonBitmap(szButton,false);
	CGameImage bt;
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Close.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	m_bkBrush = CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(32,55));

	SetWndRgn();// ���ô���Բ��2012.10.15 yyf
}
LRESULT CCharmExchange::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	Invalidate();
	return LRESULT(0);
}
afx_msg void CCharmExchange::OnBnClickedChange()
{
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			GetMainRoom()->SendData(&(m_ChangeList[i].iID), sizeof(int), MDM_GP_CHARMEXCHANGE, ASS_EXCHANGE, 0);
		}
	}	
}

afx_msg void CCharmExchange::OnBnClickedCancel()
{
	OnOK();
}

afx_msg void CCharmExchange::OnBnClickedClose()
{
	OnOK();
}

void CCharmExchange::Clear()
{
	m_ChangeList.clear();
	m_list.DeleteAllItems();
}

void CCharmExchange::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CLoadFileBmp::OnPaint()

/*//2012.10.15 yyf����
	CGameImageHelper	help(&m_bkimage);
	help.BitBlt(dc.GetSafeHdc(),0,0);*/

	//�滭��ܵ�ͼ 2012.10.15 yyf
	m_configDlg.DrawViewFrame(&dc);
}

LRESULT CCharmExchange::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case WM_CTLCOLORSTATIC:
		{
			SetTextColor((HDC)wParam, RGB(0,0,0)) ;
			SetBkColor((HDC)wParam, m_configDlg.GetBKImageEX().GetPixel(32,55));
			return (LRESULT)m_bkBrush;
		}
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


afx_msg void CCharmExchange::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

BEGIN_MESSAGE_MAP(CCharmExchange, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CCharmExchange::OnBnClickedChange)
	ON_BN_CLICKED(IDC_BUTTON2, &CCharmExchange::OnBnClickedClose)
	ON_BN_CLICKED(IDCANCEL2, &CCharmExchange::OnBnClickedCancel)
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CCharmExchange::OnExchangeSkin)
END_MESSAGE_MAP()


//��������ֵ
void CCharmExchange::UpdateFascination()
{
	CString strFascination;
	strFascination.Format("����ǰ�ɶһ�����������Ϊ:%d",GetMainRoom()->m_PlaceUserInfo.dwFascination);
	SetDlgItemText(IDC_STATIC,strFascination);
}

// ���ô���Բ�� 2012.10.15 yyf
void CCharmExchange::SetWndRgn(void)
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
