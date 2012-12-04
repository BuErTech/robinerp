// PropSelUser.cpp : ʵ���ļ�
//

#include "../stdafx.h"
#include "PropSelUser.h"
//#include "GameFrameDlg.h"
// CPropSelUser �Ի���

IMPLEMENT_DYNAMIC(CPropSelUser, CDialog)

CPropSelUser::CPropSelUser(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROP_SEL_USER, pParent)
{
	dwUsedPropID=-1;
	dwLocalUserID=-1;
	memset(m_pUserItem,0,sizeof(m_pUserItem));
	maxUser=0;
	m_bkColor=RGB(192,192,192);
	imgDialog=NULL;
	propItemName="δ֪";
	m_bkBrush=NULL;
}

CPropSelUser::~CPropSelUser()
{
	DeleteObject(m_bkBrush);
}

void CPropSelUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLOSE, m_bnClose);
}


BEGIN_MESSAGE_MAP(CPropSelUser, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CLOSE, &CPropSelUser::OnBnClickedClose)
END_MESSAGE_MAP()


// CPropSelUser ��Ϣ�������

BOOL CPropSelUser::OnInitDialog()
{
	CDialog::OnInitDialog();

	Init();


	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPropSelUser::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	drawSkin();
}

void CPropSelUser::drawSkin(void)
{

	if(!imgDialog)
		return;
	CDC *pdc=this->GetWindowDC();
	if(!pdc)return;
	CRect rc;
	this->GetWindowRect(&rc);
	Graphics g(pdc->m_hDC);

	int topHeight=32;//GetSystemMetrics(SM_CYFRAME)+GetSystemMetrics(SM_CYSIZE)+6;
	g.DrawImage(imgDialog,RectF(0.0f,0.0f,10.0f,topHeight),0.0f,0.0f,10.0f,topHeight,UnitPixel);
	g.DrawImage(imgDialog,RectF(10.0f,0.0f,rc.Width()-20,topHeight),9.0f,0.0f,1.0f,topHeight,UnitPixel);
	g.DrawImage(imgDialog,RectF((REAL)rc.Width()-10,0.0f,10.0f,topHeight),imgDialog->GetWidth()-10,0.0f,10.0f,topHeight,UnitPixel);
	m_bkColor=pdc->GetPixel(9,topHeight-1);
	m_txtColor^=m_bkColor;
	int bottomHeight=10;
	g.DrawImage(imgDialog,RectF(0.0f,topHeight,10.0f,rc.Height()-topHeight-bottomHeight),0.0f,topHeight,10.0f,imgDialog->GetHeight()-topHeight-bottomHeight,UnitPixel);
	g.DrawImage(imgDialog,RectF(rc.Width()-10.0f,topHeight,10.0f,rc.Height()-topHeight-bottomHeight),
		imgDialog->GetWidth()-10,topHeight,10.0f,1,UnitPixel);

	g.DrawImage(imgDialog,RectF(0,rc.Height()-bottomHeight,imgDialog->GetWidth()-5,bottomHeight),0,imgDialog->GetHeight()-bottomHeight,10,bottomHeight,UnitPixel);
	g.DrawImage(imgDialog,RectF(10,rc.Height()-bottomHeight,rc.Width()-20,bottomHeight),10,imgDialog->GetHeight()-bottomHeight,1,bottomHeight,UnitPixel);
	g.DrawImage(imgDialog,RectF(rc.Width()-10,rc.Height()-bottomHeight,10,bottomHeight),imgDialog->GetWidth()-10,imgDialog->GetHeight()-bottomHeight,10,bottomHeight,UnitPixel);

	CString s="����";

	Font font(s.AllocSysString(),12,0,UnitPixel);
	SolidBrush brush(Color(255,255,255));
	s.Format("��ѡ����� [%s] ʹ�ö��󣬵�ǰ��Ϸ���� %d �������û�����",propItemName,maxUser);
	g.DrawString(s.AllocSysString(),-1,&font,PointF(5,5),0,&brush);
	
	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);

	m_bnClose.Invalidate(FALSE);
	brush.SetColor(Color(0,0,0));
	Pen pen(&brush);//20081128
	REAL x,y,w,h; //20081128���������ͼƬ��λ��
	for(int i=0;i<maxUser;i++)
	{
		x=userRect[i].left+(66-userImg[i]->GetWidth())/2;
		y=userRect[i].top+(105-userImg[i]->GetHeight())/2;
		w=userImg[i]->GetWidth();
		h=userImg[i]->GetHeight();
		g.DrawImage(userImg[i],x,y,w,h);//��ͷ���滻ԭ���Ļ�ͷ�����
		//��ѡ������߿�
		g.DrawRectangle(&pen,userRect[i].left , userRect[i].top , 
			userRect[i].Width() , userRect[i].Height());
		g.DrawString(userName[i].AllocSysString() ,-1 ,&font , 
			RectF(userRect[i].left,userRect[i].bottom+5,userRect[i].Width(),30),&sf,&brush);
	}
	g.ReleaseHDC(pdc->m_hDC);
}

void CPropSelUser::resetInformation(void)
{
	CString simg;
	maxUser=0;
	//{{add by zxd 2009-03-26������һ��Ŀ¼����ͷ��ͼƬ����szAppPathParent����ԭ����CINIFile::GetAppPath()
	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);
	//add by zxd}}

	for(int i=0;i<MAX_PEOPLE;i++)
	{
		if(m_pUserItem[i] && m_pUserItem[i]->GameUserInfo.dwUserID!=dwLocalUserID && m_pUserItem[i]->GameUserInfo.dwUserID!=0)
		{
			userRect[maxUser].left=maxUser*100+20;
			userRect[maxUser].top=40;
			userRect[maxUser].right=userRect[maxUser].left+66;//20081128
			userRect[maxUser].bottom=userRect[maxUser].top+105; //20081128
			simg.Format(TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"),szAppPathParent,m_pUserItem[i]->GameUserInfo.bBoy?_T("Boy"):_T("Girl"),(m_pUserItem[i]->GameUserInfo.bLogoID%0x100)%6);
			if(m_pUserItem[i]->GameUserInfo.bLogoID>=0xFF && m_pUserItem[i]->GameUserInfo.bLogoID<0x200)
			{
				simg.Format("%s/CustomFace\\%d.png",szAppPathParent,m_pUserItem[i]->GameUserInfo.dwUserID);
				CFileFind ff;
				BOOL bFoundFile=ff.FindFile(simg);
				ff.Close();
				if(!bFoundFile)
					simg.Format(TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"),szAppPathParent,m_pUserItem[i]->GameUserInfo.bBoy?_T("Boy"):_T("Girl"),(m_pUserItem[i]->GameUserInfo.bLogoID%0x100)%6);// .bBoy?0:6);// .bLogoID);
			}
#ifndef BZ_ZLIB
			if (!CBcfFile::IsFileExist(simg))
			{
				simg.Format(TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"),szAppPathParent,m_pUserItem[i]->GameUserInfo.bBoy?_T("Boy"):_T("Girl"),0/*m_pUserItem[i]->GameUserInfo.bLogoID%12*/);
			}
#endif
#ifndef BZ_ZLIB
			userImg[maxUser]=Image::FromFile(simg.AllocSysString());
#else
			CUnZipRes unZipRes;
			IStream * pStream=NULL;
			TCHAR * pchar=simg.GetBuffer(simg.GetLength()+1);
			pStream=unZipRes.LoadFileResFromZip(pchar);
			
			userImg[maxUser]=new Image(pStream);
			if(pStream!=NULL)
			{
				pStream->Release();
				pStream=NULL;
			}

#endif
			
			//userRect[maxUser].right=userRect[maxUser].left+(userImg[maxUser]->GetWidth()>0?userImg[maxUser]->GetWidth():50);
			//userRect[maxUser].bottom=userRect[maxUser].top+(userImg[maxUser]->GetHeight()>0?userImg[maxUser]->GetHeight():55);
			userName[maxUser]=m_pUserItem[i]->GameUserInfo.nickName;
			userID[maxUser]=m_pUserItem[i]->GameUserInfo.dwUserID;
			maxUser++;
		}
	}
	CRect r;	//ȡ��ǰ���ڴ�С
	GetClientRect(&r);
	int x=(r.Width()-maxUser*100)/2; //��һ��ͷ��λ��
	for(int i=0;i<maxUser;i++)
	{
		userRect[i].left=x+i*100;
		userRect[i].right=userRect[i].left+66;
	}
	//CRect r;
	//GetWindowRect(&r);
	//r.right=r.left+maxUser*100+50;
	//MoveWindow(r);
	//CenterWindow();
}

LRESULT CPropSelUser::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lrst=CDialog::DefWindowProc(message, wParam, lParam);

	if (!::IsWindow(m_hWnd))
		return lrst;

	if (message==WM_MOVE||message==WM_PAINT||message==WM_NCPAINT||message==WM_NCACTIVATE)// ||message == WM_NOTIFY)
	{
		drawSkin();
	}
	return lrst;
}

LRESULT CPropSelUser::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		//case WM_CTLCOLORSCROLLBAR :
		//case WM_CTLCOLORBTN:
		//case WM_CTLCOLORMSGBOX:
		//case WM_CTLCOLOREDIT:
		//case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC :
		SetTextColor((HDC)wParam, RGB(0,0,0)) ;
		SetBkColor((HDC)wParam, m_bkColor);
		
		if (m_bkBrush == NULL) m_bkBrush=CreateSolidBrush(m_bkColor);
		return (LRESULT)m_bkBrush;
	}


	return CDialog::WindowProc(message, wParam, lParam);
}

void CPropSelUser::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i=0;i<maxUser;i++)
	{
		if(PtInRect(userRect[i],point))
		{
			GetParent()->PostMessage(UM_USEITEM_USERSELECTED,dwUsedPropID,userID[i]);
			this->ShowWindow(SW_HIDE);
			return;
		}
	}
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CPropSelUser::OnMouseMove(UINT nFlags, CPoint point)
{
	for(int i=0;i<maxUser;i++)
	{
		if(PtInRect(userRect[i],point))
		{
			//SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND)));
			SetCursor(LoadCursor(GetModuleHandle(FACE_DLL_NAME),MAKEINTRESOURCE(IDC_HAND)));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CPropSelUser::Init(void)
{
	CString s;
	s.Format("..\\%sdialog\\dialog.bmp",m_skinmgr.GetSkinPath());

#ifndef BZ_ZLIB
	imgDialog=Image::FromFile(s.AllocSysString());
#else
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	TCHAR * pchar=s.GetBuffer(s.GetLength()+1);
	pStream=unZipRes.LoadFileResFromZip(pchar);

	imgDialog=new Image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}

#endif
	

	TCHAR path[MAX_PATH];

	wsprintf(path,"..\\%sdialog\\Rect_close.bmp",m_skinmgr.GetSkinPath());
	m_bnClose.LoadButtonBitmap(path,false);

	CRect rc;
	GetClientRect(&rc);
	GetDlgItem(IDC_CLOSE)->MoveWindow(rc.Width()-36,0,36,20);

}

//����Ƥ��
void CPropSelUser::LoadSkin()
{
	CString s;
	s.Format("..\\%sdialog\\dialog.bmp",m_skinmgr.GetSkinPath());

#ifndef BZ_ZLIB
	if (imgDialog != NULL)
		delete imgDialog;
	imgDialog=Image::FromFile(s.AllocSysString());
#else
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	TCHAR * pchar=s.GetBuffer(s.GetLength()+1);
	pStream=unZipRes.LoadFileResFromZip(pchar);
	if (imgDialog != NULL)
		delete imgDialog;
	imgDialog=new Image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}

#endif
	TCHAR path[MAX_PATH];
	wsprintf(path,"..\\%sdialog\\Rect_close.bmp",m_skinmgr.GetSkinPath());
	m_bnClose.LoadButtonBitmap(path,false);
}

void CPropSelUser::OnBnClickedClose()
{
	this->ShowWindow(SW_HIDE);
}

