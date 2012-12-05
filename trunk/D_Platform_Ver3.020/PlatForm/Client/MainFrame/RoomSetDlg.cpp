#include "StdAfx.h"
#include "Resource.h"
#include "RoomSetDlg.h"

//�궨��
#define BUTTON_HEIGHT							22						//��ť�߶�
#define LEFT_HIT_BUTTON							WM_USER+200				//�������
#define SETUP_TITLE_HEIGHT						26						//�������߶�

BEGIN_MESSAGE_MAP(CAFCButtonBar, CWnd)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CRoomSetDlg, CLoadFileBmp)
	ON_WM_SIZE()
	ON_MESSAGE(LEFT_HIT_BUTTON,OnHitButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDCANCEL2, &CRoomSetDlg::OnBnClickedCancel2)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBaseSetDlg, CGameFaceGo)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CRoomRuleSetDlg, CBaseSetDlg)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CGameRuleSetDlg, CBaseSetDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/*******************************************************************************************************/

//���캯��
CAFCButtonBar::CAFCButtonBar()
{
	m_uHotItem=0;
	m_uActiveItem=0;
}

//��������
CAFCButtonBar::~CAFCButtonBar()
{
}

//���밴ť
INT_PTR CAFCButtonBar::AddButton(UINT uButtonID, TCHAR * szButtonTitle)
{
	StructButtonItem ButtonInfo;
	ButtonInfo.uButtonID=uButtonID;
	ButtonInfo.strButtonText=szButtonTitle;
	INT_PTR uIndex=m_ButtonInfoArray.Add(ButtonInfo);
	if (m_uActiveItem>uIndex) m_uActiveItem=uIndex;
	Invalidate(FALSE);
	return uIndex;
}

//ɾ����ť
INT_PTR CAFCButtonBar::DeleteButton(UINT uButtonID)
{
	for (INT_PTR i=0;i<m_ButtonInfoArray.GetCount();i++)
	{
		if (m_ButtonInfoArray[i].uButtonID==uButtonID)
		{
			m_uActiveItem=0;
			m_ButtonInfoArray.RemoveAt(i);
			Invalidate(FALSE);
			break;
		}
	}
	return m_uActiveItem;
}

//���ü�����
void CAFCButtonBar::SetActiveButton(UINT uButtonID)
{
	for (INT_PTR i=0;i<m_ButtonInfoArray.GetCount();i++)
	{
		if (m_ButtonInfoArray.GetAt(i).uButtonID==uButtonID)
		{
			m_uActiveItem=i;
			Invalidate(FALSE);
			return;
		}
	}
	return;
}

//�ػ�����
void CAFCButtonBar::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//��������
	CDC BufferDC;
	CBitmap BufferBmp;
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	CBitmap * pOldBmp=BufferDC.SelectObject(&BufferBmp);

	//���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(255,255,255));
	BufferDC.SelectObject(&CGameImageLink::m_HFont);

	//�滭����
	BufferDC.FillSolidRect(&ClientRect,RGB(255,182,107));

	//�滭��ť
	CRect DrawRect;
	for (int i=0;i<m_ButtonInfoArray.GetCount();i++)
	{
		DrawRect.SetRect(70*i+5,5,70*i+5+ClientRect.Width()/2,BUTTON_HEIGHT);
		DrawButtonItem(&BufferDC,DrawRect,i);
	}

	//�滭����
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.SelectObject(pOldBmp);
	BufferBmp.DeleteObject();
	BufferDC.DeleteDC();

	return;
}

//�滭����
void CAFCButtonBar::DrawButtonItem(CDC * pDC, CRect & rcDraw, INT_PTR uIndex)
{
	//���ñ���
	COLORREF crBackColor=RGB(255,182,87),crTextColor=RGB(255,255,255);
	if ((m_uHotItem==uIndex)||(m_uActiveItem==uIndex)) 
	{
		crBackColor=RGB(255,182,127);
		if (m_uActiveItem==uIndex) 
			crTextColor=RGB(255,0,0);
	}

	//��������
	CBrush FrameBrush(crBackColor);
	CPen FramePen(PS_SOLID,1,RGB(255,255,255));

	//�滭��ť
	rcDraw.DeflateRect(2,0,2,0);
	pDC->FillSolidRect(&rcDraw,crBackColor);
	CPen * pOldPen=pDC->SelectObject(&FramePen);
	CBrush * pOldBrush=pDC->SelectObject(&FrameBrush);
	pDC->RoundRect(&rcDraw,CPoint(2,2));
	rcDraw.DeflateRect(0,2,0,2);
	pDC->SetTextColor(crTextColor);
	pDC->DrawText(m_ButtonInfoArray[uIndex].strButtonText,rcDraw,DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER|DT_SINGLELINE);

	//������Դ
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	FramePen.DeleteObject();
	FrameBrush.DeleteObject();
	
	return;
}

//��������
INT_PTR CAFCButtonBar::HitTest(CPoint MousePoint)
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
	if ((MousePoint.x>2)&&(MousePoint.x<(ClientRect.Width()-2))&&
		(MousePoint.y>5)&&((MousePoint.y-10)%(BUTTON_HEIGHT+2)<BUTTON_HEIGHT))
	{
		INT_PTR iIndex=(MousePoint.x-10)/(70+2);
		if (iIndex<m_ButtonInfoArray.GetCount()) return iIndex;
	}
	return m_ButtonInfoArray.GetCount();
}

//�������Ϣ
BOOL CAFCButtonBar::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	INT_PTR uOldHotItem=m_uHotItem;
	m_uHotItem=HitTest(MousePoint);
	if (uOldHotItem!=m_uHotItem) 
	{
		CRect DrawRect,ClientRect;
		GetClientRect(&ClientRect);
		if (uOldHotItem<m_ButtonInfoArray.GetCount())
		{
			DrawRect.SetRect(0,BUTTON_HEIGHT*(int)uOldHotItem+10+2*(int)uOldHotItem,ClientRect.Width(),
				BUTTON_HEIGHT*((int)uOldHotItem+1)+10+2*(int)uOldHotItem);
			InvalidateRect(&DrawRect,FALSE);
		}
		if (m_uHotItem<m_ButtonInfoArray.GetCount())
		{
			DrawRect.SetRect(0,BUTTON_HEIGHT*(int)m_uHotItem+10+2*(int)m_uHotItem,ClientRect.Width(),
				BUTTON_HEIGHT*((int)m_uHotItem+1)+10+2*(int)m_uHotItem);
			InvalidateRect(&DrawRect,FALSE);
		}
	}
	if (m_uHotItem<m_ButtonInfoArray.GetCount()) 
	{
		SetCursor(CGameImageLink::m_hHandCursor);
		return TRUE;
	}
	return __super::OnSetCursor(pWnd, nHitTest, message);
}

//����Ҽ�����
void CAFCButtonBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	INT_PTR uHotItem=HitTest(point);
	if (uHotItem<m_ButtonInfoArray.GetCount())
	{
		GetParent()->PostMessage(LEFT_HIT_BUTTON,(WPARAM)&m_ButtonInfoArray[uHotItem],uHotItem);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//��Ϣ����
BOOL CAFCButtonBar::PreTranslateMessage(MSG* pMsg)
{
	//����ȡ����Ϣ�ͻس���Ϣ
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
        GetParent()->PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_F1))
	{
		return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

/*******************************************************************************************************/

//���캯��
CBaseSetDlg::CBaseSetDlg() : CGameFaceGo(0)
{
}

//��������
CBaseSetDlg::~CBaseSetDlg()
{
}

//�ػ�����
void CBaseSetDlg::OnPaint() 
{
	CPaintDC dc(this);

	//�滭����
	CRect ClipRect;
	dc.GetClipBox(&ClipRect);
	dc.FillSolidRect(&ClipRect,CGameImageLink::m_DialogResource.m_crBackColor);

	return;
}

/*******************************************************************************************************/

//���캯��
CRoomRuleSetDlg::CRoomRuleSetDlg()
{
	m_bSaveTalk=TRUE;
	m_bShowUser=TRUE;
	m_bShowMessage=TRUE;
}

//��������
CRoomRuleSetDlg::~CRoomRuleSetDlg()
{
}

//DDX/DDV ֧��
void CRoomRuleSetDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CRoomRuleSetDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	if (m_bSaveTalk) ((CButton *)GetDlgItem(IDC_SAVE_TALK))->SetCheck(BST_CHECKED);
	if (m_bShowUser) ((CButton *)GetDlgItem(IDC_SHOW_USER))->SetCheck(BST_CHECKED);
	
	return TRUE;
}

//�������
bool CRoomRuleSetDlg::SaveSetting()
{
	//��ȡ����
	m_bSaveTalk=(((CButton *)GetDlgItem(IDC_SAVE_TALK))->GetCheck()==BST_CHECKED);
	m_bShowUser=(((CButton *)GetDlgItem(IDC_SHOW_USER))->GetCheck()==BST_CHECKED);

	return true;
}

/*******************************************************************************************************/

//���캯��
CGameRuleSetDlg::CGameRuleSetDlg()
{
	m_szPass[0]=0;
	m_bCutPercent=10;
	m_bPass=FALSE;
	m_bSameIP=false;
	m_bSameIPNumber=0;
	m_bLimitCut=FALSE;
	m_bLimitPoint=FALSE;
	m_bInvaliMode=INVALI_ALL;
	m_dwLowPoint=-100000L;
	m_dwHighPoint=100000L;

	m_bSaveTalk=TRUE;
	m_bShowUser=TRUE;
	m_bShowMessage=TRUE;

	m_bkBrush=NULL;
	
}

//��������
CGameRuleSetDlg::~CGameRuleSetDlg()
{
	DeleteObject(m_bkBrush);
}

//DDX/DDV ֧��
void CGameRuleSetDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUT_COUNT, m_CutInput);
	DDX_Control(pDX, IDC_LOW_POINT, m_LessPoint);
	DDX_Control(pDX, IDC_HIGH_POINT, m_HightPoint);
	DDX_Control(pDX, IDC_PASS, m_PassWord);
}

//��ʼ������
BOOL CGameRuleSetDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	((CEdit *)GetDlgItem(IDC_CUT_COUNT))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_HIGH_POINT))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_LOW_POINT))->LimitText(9);
	((CEdit *)GetDlgItem(IDC_PASS))->LimitText(sizeof(m_szPass)-1);

	//��������
	SetDlgItemInt(IDC_CUT_COUNT,m_bCutPercent);
	SetDlgItemInt(IDC_LOW_POINT,m_dwLowPoint);
	SetDlgItemInt(IDC_HIGH_POINT,m_dwHighPoint);

	if (m_bSameIP) ((CButton *)GetDlgItem(IDC_SAME_IP))->SetCheck(BST_CHECKED);
	if (m_bSameIPNumber) 
	{
		TCHAR sz[10];
		((CComboBox*)GetDlgItem(IDC_IP_LIMITE))->SelectString(0,ltoa(m_bSameIPNumber,sz,10));
	}
	if (m_bLimitCut) ((CButton *)GetDlgItem(IDC_LIMIT_CUT))->SetCheck(BST_CHECKED);
	if (m_bLimitPoint) ((CButton *)GetDlgItem(IDC_LIMIT_POINT))->SetCheck(BST_CHECKED);
	if (m_bPass) ((CButton *)GetDlgItem(IDC_SET_PASS))->SetCheck(BST_CHECKED);
	GetDlgItem(IDC_PASS)->SetWindowText(m_szPass);

	//����IP
	/*if(Glb().m_release)
	{
		m_bSameIP=true;
		((CButton *)GetDlgItem(IDC_SAME_IP))->SetCheck(BST_CHECKED);
		((CButton *)GetDlgItem(IDC_SAME_IP))->EnableWindow(0);
	}*/
	//�������뷽ʽ
	if (m_bInvaliMode==INVALI_ALL) ((CButton *)GetDlgItem(IDC_INVALI_ALL))->SetCheck(BST_CHECKED);
	else if (m_bInvaliMode==INVALI_ONLY_FRIEND) ((CButton *)GetDlgItem(IDC_FRIEND_INVALI))->SetCheck(BST_CHECKED);
	else ((CButton *)GetDlgItem(IDC_NO_INVALI))->SetCheck(BST_CHECKED);

	//��������
	if (m_bSaveTalk) ((CButton *)GetDlgItem(IDC_SAVE_TALK))->SetCheck(BST_CHECKED);
	if (m_bShowUser) ((CButton *)GetDlgItem(IDC_SHOW_USER))->SetCheck(BST_CHECKED);
	
//	int r,g,b;
	CString s=CBcfFile::GetAppPath ();//����·��
	CBcfFile f( s + "skin0.bcf");
	CString key=Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal("skin0","skinfolder","image\\client\\skin0\\");
	wsprintf(path,"%sdialog\\set_bk.bmp",skinfolder);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	CRect ClientRect;
	GetClientRect(&ClientRect);
	//this->MapWindowPoints(this->GetParent(),&ClientRect);
	CGameImageHelper	help(&m_bk);
	this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());

    // PengJiLin, 2010-9-14, �ѻ��ָ�Ϊ��� 
    int iGold = f.GetKeyVal("GameRoom","UseGoldName",0);
    if(iGold > 0)
    {
        GetDlgItem(IDC_LIMIT_POINT)->SetWindowText("ֻ�ͽ����");
    }


	m_bkBrush=CreateSolidBrush(m_bk.GetPixel(50,100));

	return TRUE;
}

//�������
bool CGameRuleSetDlg::SaveSetting()
{
	//��ȡ����
	m_bSameIP=(((CButton *)GetDlgItem(IDC_SAME_IP))->GetCheck()==BST_CHECKED);
	if(((CComboBox *)GetDlgItem(IDC_IP_LIMITE))->GetCurSel()==0)
		m_bSameIPNumber=3;
	else if(((CComboBox *)GetDlgItem(IDC_IP_LIMITE))->GetCurSel()==1)
		m_bSameIPNumber=4;
	else m_bSameIPNumber=0;
	m_bPass=(((CButton *)GetDlgItem(IDC_SET_PASS))->GetCheck()==BST_CHECKED);
	m_bLimitCut=(((CButton *)GetDlgItem(IDC_LIMIT_CUT))->GetCheck()==BST_CHECKED);
	m_bLimitPoint=(((CButton *)GetDlgItem(IDC_LIMIT_POINT))->GetCheck()==BST_CHECKED);

	//��ȡ���뷽ʽ
	if (((CButton *)GetDlgItem(IDC_INVALI_ALL))->GetCheck()==BST_CHECKED) m_bInvaliMode=INVALI_ALL;
	else if (((CButton *)GetDlgItem(IDC_FRIEND_INVALI))->GetCheck()==BST_CHECKED) m_bInvaliMode=INVALI_ONLY_FRIEND;
	else m_bInvaliMode=INVALI_NO;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	
	if (m_bLimitPoint) 
	{
		long int dwLowPoint=0,dwHighPoint=0;
		CString strBuffer;
		GetDlgItemText(IDC_LOW_POINT,strBuffer);
		dwLowPoint=atol(strBuffer);
		GetDlgItemText(IDC_HIGH_POINT,strBuffer);
		dwHighPoint=atol(strBuffer);
		if (dwHighPoint<=dwLowPoint)
		{
			//AFCMessageBox(fMsg.GetKeyVal("GameRuleSetDlg","ConfigScoreArray","���������û��ַ�Χ��"));
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRuleSetDlg","ConfigScoreArray","���������û��ַ�Χ��"));
			GetDlgItem(IDC_LOW_POINT)->SetFocus();
			return false;
		}
		m_dwLowPoint=dwLowPoint;
		m_dwHighPoint=dwHighPoint;
	}
	if (m_bLimitCut) 
	{
		BOOL bCutPercent=0;
		GetDlgItemInt(IDC_CUT_COUNT,&bCutPercent,FALSE);
		if ((bCutPercent<0)||(bCutPercent>99))
		{
			//AFCMessageBox(fMsg.GetKeyVal("GameRuleSetDlg","ConfigEscapeRate","���������������ʣ�"));
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRuleSetDlg","ConfigEscapeRate","���������������ʣ�"));
			GetDlgItem(IDC_CUT_COUNT)->SetFocus();
			return false;
		}
	}
	if (m_bPass) 
	{
		GetDlgItem(IDC_PASS)->GetWindowText(m_szPass,sizeof(m_szPass));
		if (lstrlen(m_szPass)==0)
		{
			//AFCMessageBox(fMsg.GetKeyVal("GameRuleSetDlg","ConfigPW","���벻��Ϊ�գ�"));
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRuleSetDlg","ConfigPW","���벻��Ϊ�գ�"));
			GetDlgItem(IDC_PASS)->SetFocus();
			return false;
		}
	}

	//��ȡ����
	m_bSaveTalk=(((CButton *)GetDlgItem(IDC_SAVE_TALK))->GetCheck()==BST_CHECKED);
	m_bShowUser=(((CButton *)GetDlgItem(IDC_SHOW_USER))->GetCheck()==BST_CHECKED);

	return true;
}

//�õ�����
void CGameRuleSetDlg::OnSetFocus(CWnd * pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	GetDlgItem(IDC_CUT_COUNT)->SetFocus();

	return;
}
void CGameRuleSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	//CBaseSetDlg::OnLButtonDown(nFlags, point);
}

void CGameRuleSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	//this->MapWindowPoints(this->GetParent(),&ClientRect);
	CGameImageHelper	help(&m_bk);
	//this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	help.BitBlt(dc.GetSafeHdc(),0,0);
}

LRESULT CGameRuleSetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetBkColor((HDC)wParam, m_bk.GetPixel(50,100));
		return (LRESULT)m_bkBrush;
	}
	return CBaseSetDlg::WindowProc(message, wParam, lParam);
}

/*******************************************************************************************************/

//���캯��
CRoomSetDlg::CRoomSetDlg(CWnd * pParent) : CLoadFileBmp(IDD_GAME_SET/*IDD_GAME_RULE*/, pParent)
{
	m_iActiveIndex=-1;
	m_pActiveSet=NULL;
	
	m_rect.left=73;
	m_rect.top=374;
	m_rect.right=340;
	m_rect.bottom=387;
}

//��������
CRoomSetDlg::~CRoomSetDlg()
{
}

//���ݰ󶨺���
void CRoomSetDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BtOK);
	DDX_Control(pDX, IDCANCEL, m_BtCancel);
	DDX_Control(pDX, IDCANCEL2, m_BtClose);
}

//��ʼ������
BOOL CRoomSetDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	SetWindowTitle(fMsg.GetKeyVal("GameRuleSetDlg","ConfigParameter","��������"));

	//�����ؼ�
	AfxSetResourceHandle(GetModuleHandle(NULL));
	m_GameRuleSet.Create(IDD_GAME_RULE,this);
	m_GameRuleSet.ShowWindow(SW_SHOW);
	//m_GameRuleSet.ShowWindow(SW_HIDE);
	m_pActiveSet=&m_GameRuleSet;
	m_iActiveIndex=1;
//	int r,g,b;
	CString s=CBcfFile::GetAppPath ();//����·��
	CBcfFile f( s + "skin0.bcf");
	CString key=Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal("skin0","skinfolder","image\\client\\skin0\\");

	/*// 2012.12.17 yyf ����
	wsprintf(path,"%sdialog\\setparent_bk.png",skinfolder);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bk);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bk.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CRoomSetDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	//-----------------------------------------------------*/
	wsprintf(path,"%sdialog\\cancel_bt.bmp",skinfolder);
	m_BtCancel.LoadButtonBitmap(path,false);
	wsprintf(path,"%sdialog\\ok_bt.bmp",skinfolder);
	m_BtOK.LoadButtonBitmap(path,false);

	wsprintf(path,"%sdialog\\log_exit_bt.bmp",skinfolder);//win_close.bmp
	m_BtClose.LoadButtonBitmap(path,false);

	//�ƶ��ؼ�
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CGameImage bt;
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_BtClose.MoveWindow(ClientRect.right-ClientRect.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	//this->MapWindowPoints(this->GetParent(),&ClientRect);
	FixControlSize(ClientRect.Width(),ClientRect.Height());
	CGameImageHelper	help(&m_bk);
	CenterWindow();
	//this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	return TRUE;
}

//ȷ��
void CRoomSetDlg::OnOK()
{
	//��������
	if ((m_pActiveSet!=NULL)&&(m_pActiveSet->SaveSetting()==false)) return;

	return __super::OnOK();
}

//λ�ñ仯
void CRoomSetDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	
	FixControlSize(cx,cy);

	return;
}

//�����ؼ� 
void CRoomSetDlg::FixControlSize(int iWidth, int iHeight)
{
	RECT rtChild;
	m_GameRuleSet.GetWindowRect(&rtChild);
	int wC = (rtChild.right-rtChild.left);
	int hC = (rtChild.bottom-rtChild.top);
	//�ƶ��ؼ�
	SafeMoveWindow(&m_GameRuleSet, (iWidth - wC)/2, (iHeight-hC)/2,wC,hC);
	RECT rt;
	m_BtCancel.GetClientRect(&rt);
	int w = rt.right - rt.left;
	int h = rt.bottom - rt.top;
	SafeMoveWindow(&m_BtCancel, (iWidth ) / 2 + 10, iHeight - h - 15, w, h);
	SafeMoveWindow(&m_BtOK, (iWidth ) / 2 - w - 10, iHeight - h - 15, w, h);
	return;
}

//������ť
LRESULT CRoomSetDlg::OnHitButton(WPARAM wparam, LPARAM lparam)
{
	//�����ؼ�
	StructButtonItem * pButtonInfo=(StructButtonItem *)wparam;
	if ((pButtonInfo!=NULL)&&(pButtonInfo->uButtonID!=m_iActiveIndex))
	{
		//�������
		CBaseSetDlg * pNowActive=NULL;
		if ((m_pActiveSet!=NULL)&&(m_pActiveSet->SaveSetting()==false)) return 0;

		//��ʾ�ؼ�
		switch (pButtonInfo->uButtonID)
		{
		case 1:		//��Ϸ����
			{
				if (m_GameRuleSet.GetSafeHwnd()==NULL)
				{
					AfxSetResourceHandle(GetModuleHandle(NULL));
					m_GameRuleSet.Create(IDD_GAME_RULE,this);
				}
				pNowActive=&m_GameRuleSet;
				break;
			}
		default: return 0;
		}

		if (pNowActive!=NULL)
		{
			//�ƶ�λ��
			CRect ClientRect;
			GetClientRect(&ClientRect);
			SafeMoveWindow(pNowActive,10,GetTitleHight()+10+SETUP_TITLE_HEIGHT,ClientRect.Width()-20,ClientRect.Height()-GetTitleHight()-50-SETUP_TITLE_HEIGHT);

			//��ʾ����
			if (m_pActiveSet!=NULL) m_pActiveSet->ShowWindow(SW_HIDE);
			m_pActiveSet=pNowActive;
			m_pActiveSet->ShowWindow(SW_SHOW);
			m_iActiveIndex=pButtonInfo->uButtonID;
		}
	}

	return 0;
}

bool CRoomSetDlg::SaveSetting()
{
	return true;	
}

/*******************************************************************************************************/

void CRoomSetDlg::OnPaint()
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

void CRoomSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rect.PtInRect(point))
	{
		//ShellExecute(NULL,"open",TEXT("http://www.bzw.cn"),NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	//CLoadFileBmp::OnLButtonDown(nFlags, point);
}

void CRoomSetDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_rect.PtInRect(point))
	{
		//HCURSOR hcur=LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_HAND));
		//ShowCursor(false);
		//SetCursor(hcur);
		//ShowCursor(true);
	}
	//CLoadFileBmp::OnMouseMove(nFlags, point);
}


void CRoomSetDlg::OnBnClickedCancel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

// ���ô���Բ�� 2012.10.15 yyf
void CRoomSetDlg::SetWndRgn(void)
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