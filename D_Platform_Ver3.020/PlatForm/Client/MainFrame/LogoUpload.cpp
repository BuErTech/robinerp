// LogoUpload.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LogoUpload.h"
#include "MainRoomEx.h"

// CLogoUpload �Ի���

/*
Notes	:�Զ���ͷ��Ĵ�С�������˴�ͷ���Сͷ��
���ڱ��ļ��У���������ͷ�ļ����У�����Ϊ������ֵ�����ʱֻ��Ҫ���뱾�ļ����ɣ�������ͷ�ļ��У��������PROJECT���±��룬����ҵ��������û�б�Ҫ
Author	:Fred Huang
Date	:2008-03-19
*/

//#define LARGE_WIDTH		75
//#define LARGE_HEIGHT	105
//#define SMALL_WIDTH		52
//#define SMALL_HEIGHT	73
//2008.3.28���������ȫ������

#define LARGE_WIDTH		50
#define LARGE_HEIGHT	50
#define SMALL_WIDTH		45
#define SMALL_HEIGHT	52

IMPLEMENT_DYNAMIC(CLogoUpload, CDialog)

CLogoUpload::CLogoUpload(CWnd* pParent /*=NULL*/)
: CDialog(CLogoUpload::IDD, pParent)
, m_cutType(1)
{
	x=y=0;
	bMoving=false;
	pParentWnd=NULL;
	m_skinIndex=0;
	imgDialog=0;
	img=0;

	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key= m_skinmgr.GetKeyVal(strSkin);
	int r,g,b;
	r=f.GetKeyVal(key,"logobgcr",44);
	g=f.GetKeyVal(key,"logobgcg",114);
	b=f.GetKeyVal(key,"logobgcb",233);
	m_bkcolor=RGB(r,g,b);
	m_bkBrush = NULL;
}

CLogoUpload::~CLogoUpload()
{
	//�����ͼ���ڴ����Ҫɾ��
	if(img && img->GetFlags()!=0)
	{
		delete img;
	}
	DeleteObject(m_bkBrush);
}

void CLogoUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SMALLIMAGE, m_smallImage);
	DDX_Control(pDX, IDC_LARGEIMAGE, m_largeImage);
	DDX_Control(pDX, IDC_STATE, m_status);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_CLOSELOGO, m_bnClose);
	DDX_Control(pDX, IDC_LOADIMAGE, m_bnBrowse);
	DDX_Control(pDX, IDC_UPLOAD, m_bnLoad);
	DDX_Control(pDX, IDCANCEL, m_bnBack);
	DDX_Control(pDX, IDC_MOVEUP,m_bnMoveUp);
	DDX_Control(pDX, IDC_MOVELEFT,m_bnMoveLeft);
	DDX_Control(pDX, IDC_MOVEDOWN,m_bnMoveDown);
	DDX_Control(pDX, IDC_MOVERIGHT,m_bnMoveRight);
	DDX_Control(pDX, IDC_RESET,m_bnReset);

	DDX_Radio(pDX, IDC_PICCUT, m_cutType);
}


BEGIN_MESSAGE_MAP(CLogoUpload, CDialog)
	ON_BN_CLICKED(IDC_LOADIMAGE, &CLogoUpload::OnBnClickedLoadimage)
	ON_BN_CLICKED(IDC_UPLOAD, &CLogoUpload::OnBnClickedUpload)
	ON_BN_CLICKED(IDCANCEL, &CLogoUpload::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MOVEUP, &CLogoUpload::OnBnClickedMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, &CLogoUpload::OnBnClickedMovedown)
	ON_BN_CLICKED(IDC_MOVELEFT, &CLogoUpload::OnBnClickedMoveleft)
	ON_BN_CLICKED(IDC_MOVERIGHT, &CLogoUpload::OnBnClickedMoveright)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET, &CLogoUpload::OnBnClickedReset)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CLOSELOGO, &CLogoUpload::OnBnClickedCloselogo)
	ON_BN_CLICKED(IDC_PICCUT, &CLogoUpload::OnBnClickedPiccut)
	ON_BN_CLICKED(IDC_PICSCALE, &CLogoUpload::OnBnClickedPicscale)
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CLogoUpload::OnExchangeSkin)
END_MESSAGE_MAP()


// CLogoUpload ��Ϣ�������

BOOL CLogoUpload::OnInitDialog()
{
	CDialog::OnInitDialog();

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLogoUpload::Init()
{
	GetDlgItem(IDC_UPLOAD)->EnableWindow(FALSE);
	m_progress.SetRange(0,100);
	m_progress.SetPos(0);

	TCHAR path[MAX_PATH];
	wsprintf(path,"%slogon\\button.bmp",m_skinmgr.GetSkinPath());
	m_bnBrowse.LoadButtonBitmap(path,false);
	m_bnLoad.LoadButtonBitmap(path,false);
	m_bnBack.LoadButtonBitmap(path,false);

	wsprintf(path,"%slogon\\move.bmp",m_skinmgr.GetSkinPath());
	m_bnMoveUp.LoadButtonBitmap(path,false);
	m_bnMoveLeft.LoadButtonBitmap(path,false);
	m_bnMoveDown.LoadButtonBitmap(path,false);
	m_bnMoveRight.LoadButtonBitmap(path,false);
	m_bnReset.LoadButtonBitmap(path,false);

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	SetDlgItemText(IDC_STATIC_HEAD,fMsg.GetKeyVal("LogoUploadDlg","PreviewHead","ͷ��Ԥ��"));
	SetDlgItemText(IDC_STATIC_NOTE,fMsg.GetKeyVal("LogoUploadDlg","AdjustHead",
		"      ע������ͨ���ϡ��¡����Ҽ�ͷ������ڴ�ͷ�����϶����ƶ�ͼƬ��λ�á�"));
	SetDlgItemText(IDC_STATIC_POS,fMsg.GetKeyVal("LogoUploadDlg","AdjustPosition","λ�õ���"));
	SetDlgItemText(IDC_PICCUT,fMsg.GetKeyVal("LogoUploadDlg","CutPic","��ȡͼƬ"));
	SetDlgItemText(IDC_PICSCALE,fMsg.GetKeyVal("LogoUploadDlg","UploadPicDirectly","ֱ���ϴ�(�Զ�����)"));
	SetDlgItemText(IDC_LOADIMAGE,fMsg.GetKeyVal("LogoUploadDlg","BrowsePic","���ͼƬ"));
	SetDlgItemText(IDC_UPLOAD,fMsg.GetKeyVal("LogoUploadDlg","UploadPic","�ϴ�ͼƬ"));
	SetDlgItemText(IDCANCEL,fMsg.GetKeyVal("LogoUploadDlg","Cancel","ȡ������"));
	SetDlgItemText(IDC_STATE_UN,fMsg.GetKeyVal("LogoUploadDlg","UnuploadPic","δ����ͼƬ"));

	CRect rc;
	GetWindowRect(&rc);
	m_bnClose.MoveWindow(rc.Width()-25,2,17,17);
	setSkinIndex(0);
}
//����
void CLogoUpload::LoadSkin()
{
	TCHAR path[MAX_PATH];
	wsprintf(path,"%slogon\\button.bmp",m_skinmgr.GetSkinPath());
	m_bnBrowse.LoadButtonBitmap(path,false);
	m_bnLoad.LoadButtonBitmap(path,false);
	m_bnBack.LoadButtonBitmap(path,false);

	wsprintf(path,"%slogon\\move.bmp",m_skinmgr.GetSkinPath());
	m_bnMoveUp.LoadButtonBitmap(path,false);
	m_bnMoveLeft.LoadButtonBitmap(path,false);
	m_bnMoveDown.LoadButtonBitmap(path,false);
	m_bnMoveRight.LoadButtonBitmap(path,false);
	m_bnReset.LoadButtonBitmap(path,false);


	setSkinIndex(0);
}

BOOL CLogoUpload::PreTranslateMessage(MSG* pMsg)
{
	try
	{
		if(pMsg->message==WM_KEYDOWN)
		{ 
			if(pMsg->wParam==VK_RETURN || pMsg->wParam == VK_F1)
				return TRUE;
			if(pMsg->wParam==VK_ESCAPE)
			{
				this->ShowWindow(SW_HIDE);
				return TRUE;
			}
			//����Ѽ���ͼƬ
			if(img && img->GetFlags()!=0)
			{
				int w=img->GetWidth();//ͼ�Ŀ�ȣ���������ʱʹ�ã������ü򵥱���
				int h=img->GetWidth();//ͼ�ĸ߶ȣ���������ʱʹ�ã������ü򵥱���
				switch(pMsg->wParam)
				{
				case VK_DOWN:
					if(y>0)
						y--;
					break;
				case VK_UP:
					if(y<h-LARGE_HEIGHT)
						y++;
					break;
				case VK_RIGHT:
					if(x>0)
						x--;
					break;
				case VK_LEFT:
					if(x<w-LARGE_WIDTH)
						x++;
					break;
				default://����������ϼ�����������w��h��Ϊ-1.
					w=h=-1;
					break;
				}
				//��w!=-1�����������ϡ��¡�����ʱ����ˢ������ͼƬ���Խ�ʡ��Դ
				if(w!=-1)
					drawImage();
			}


		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}


	return CDialog::PreTranslateMessage(pMsg);
}

//����ͼƬ
void CLogoUpload::OnBnClickedLoadimage()
{
	try
	{
		////////////////////////////////////////////
		////Kylin 20090118 ����ͼƬ���͹���
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str;
		str = fMsg.GetKeyVal("LogoUploadDlg","PicFilter","ͼƬ�ļ� (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png||");

		CFileDialog fd(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,str.GetBuffer());
		////////////////////////////////////////////

		if(fd.DoModal()==IDCANCEL)
			return;

		if(img)
		{
			delete img;
			img=0;
		}
#ifndef BZ_ZLIB  //< add by wxx 
		img=Image::FromFile(fd.GetPathName().AllocSysString());
#else
		CString strTmp=fd.GetPathName();
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		TCHAR * pchar=strTmp.GetBuffer(strTmp.GetLength()+1);
		pStream=unZipRes.LoadFileResFromZip(pchar);

		img=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}

#endif
		
		if(!img || img->GetFlags()==0)
		{
			GetDlgItem(IDC_UPLOAD)->EnableWindow(FALSE);
			m_status.SetWindowText(fMsg.GetKeyVal("LogoUploadDlg","UnuploadPic","δ����ͼƬ"));
			str = fMsg.GetKeyVal("LogoUploadDlg","LoadPicFailed","�Բ��𣬼���ͼƬʧ�ܣ�����ͼƬ��ʽ��\n\n֧��BMP��JPG��PNGͼ");
			
			//AFCMessageBox(str);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
			img=0;
			return;
		}
		x=0;y=0;
		GetDlgItem(IDC_UPLOAD)->EnableWindow(TRUE);
		//����������Image���ص�ʱ�䣬�ռ��������ʾ����ͼƬ����ʱ100ms����ʾ��������
		SetTimer(1,200,NULL);
		return;
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}
	//if(!m_image.IsNull())
	//	m_image.Destroy();
	//if(m_image.Load(fd.GetPathName())!=S_OK)
	//{
	//	GetDlgItem(IDC_UPLOAD)->EnableWindow(FALSE);
	//	m_status.SetWindowText(TEXT("δ����ͼƬ"));
	//	MessageBox(TEXT("�Բ��𣬼���ͼƬʧ�ܣ�����ͼƬ��ʽ��\n\n֧��BMP��JPG��PNGͼ"),TEXT("����"),MB_ICONERROR);
	//	return;
	//}
	//x=0;y=0;
	//GetDlgItem(IDC_UPLOAD)->EnableWindow(TRUE);
	////����������Image���ص�ʱ�䣬�ռ��������ʾ����ͼƬ����ʱ100ms����ʾ��������
	//SetTimer(1,200,NULL);
}


void CLogoUpload::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	this->ShowWindow(SW_HIDE);
}

//ͼƬ����
void CLogoUpload::OnBnClickedMoveup()
{
	try
	{
		if(!img)
			return;
		if(img->GetFlags()==0)
			return;
		if(y<img->GetHeight()-LARGE_HEIGHT)
			y++;
		drawImage();
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

//ͼƬ����
void CLogoUpload::OnBnClickedMovedown()
{
	try
	{
		if(!img)
			return;
		if(img->GetFlags()==0)
			return;
		if(y>0)
			y--;
		drawImage();
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

//ͼƬ����
void CLogoUpload::OnBnClickedMoveleft()
{
	try
	{
		if(!img)
			return;
		if(img->GetFlags()==0)
			return;
		if(x<img->GetWidth()-LARGE_WIDTH)
			x++;
		drawImage();
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

//ͼƬ����
void CLogoUpload::OnBnClickedMoveright()
{
	try
	{
		if(!img)
			return;
		if(img->GetFlags()==0)
			return;
		if(x>0)
			x--;
		drawImage();
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

void CLogoUpload::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	drawSkin();
	drawImage();
}

/*
Function	:drawImage
Notes		:���ϴ�ǰԤ��ͼƬ��2����ʾ������ʾͼƬ��ֱ�ӽ�ȡͼƬ��Сͼ��ֱ���ô�ͼ���ӵģ���ʵ�Ͼ�1��ͼ
����ͼƬ��С����ڸ�ƽ̨����һ��������ʱͳһΪ����ͼ 75*105��Сͼ��52*73
Author		:Fred Huang
Date		:2008-03-18
Parameter	:na
return		:void
*/
void CLogoUpload::drawImage(void)
{
	try
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str;

		if(!img)
			return;
		if(img->GetFlags()==0)
		{
			img=0;
			return;
		}
		int w=img->GetWidth()>LARGE_WIDTH?LARGE_WIDTH:img->GetWidth();
		int h=img->GetHeight()>LARGE_HEIGHT?LARGE_HEIGHT:img->GetHeight();

		//��ʾʱ��ͼ��λ�ã�ֻ��ͼƬС�ڴ�ͼ��׼���ʱ��Ч
		int l=w<LARGE_WIDTH?(LARGE_WIDTH-w)/2:0;
		int t=h<LARGE_HEIGHT?(LARGE_HEIGHT-h)/2:0;

		Graphics g(this->m_hWnd);
		if(m_cutType==0)
			g.DrawImage(img,Rect(l+30,t+58,w,h),x,y,w,h,UnitPixel);
		else
		{
			g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			g.DrawImage(img,Rect(30,58,LARGE_WIDTH,LARGE_HEIGHT),0,0,img->GetWidth(),img->GetHeight(),UnitPixel);
		}
		//��ʾ��ǰ��ͼƬ��Ϣ
		CString status;
		str = fMsg.GetKeyVal("LogoUploadDlg","PicSize","ͼƬ��С��%d * %d �����Ͻ�λ��(x,y)��%d,%d");
		status.Format(str,img->GetWidth(),img->GetHeight(),x,y);
		m_status.SetWindowText(status);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

void CLogoUpload::OnTimer(UINT_PTR nIDEvent)
{
	try
	{
		if(nIDEvent==1)
		{
			//ֻ������ʱ��ʾ�����Ե���һ���Ժ�ɾ���ö�ʱ��
			KillTimer(1);
			Invalidate(TRUE);
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnTimer(nIDEvent);
}
/*
Function	:OnBnClickedReset
Notes		:������ʾλ�ã���ͼ�����Ͻǿ�ʼ
Author		:Fred Huang
Date		:2008-03-18
Parameter	:na
return		:void
*/
void CLogoUpload::OnBnClickedReset()
{
	x=y=0;
	Invalidate(TRUE);
}

void CLogoUpload::OnMouseMove(UINT nFlags, CPoint point)
{
	try
	{
		if(img && img->GetFlags()!=0 && m_cutType==0)
		{
			CRect rect;
			m_largeImage.GetWindowRect(&rect);
			this->ScreenToClient(&rect);
			if(PtInRect(rect,point))
			{
				//���ƶ�����
				if(bMoving)
				{
					//����µ�λ����ɵ�λ�õĲ�
					int dx=m_savePoint.x-point.x;
					int dy=m_savePoint.y-point.y;
					if(x+dx>=0 && x+dx<img->GetWidth()-LARGE_WIDTH)
						x+=dx;
					if(y+dy>=0 && y+dy<img->GetHeight()-LARGE_HEIGHT)
						y+=dy;
					m_savePoint=point;
					drawImage();
					SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
				}
			}
			else
				bMoving=false;
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CLogoUpload::OnLButtonDown(UINT nFlags, CPoint point)
{
	try
	{
		if(img && img->GetFlags()!=0 && m_cutType==0)
		{
			CRect rc;
			rc.left=21;rc.top=52;rc.right=rc.left+53;rc.bottom=rc.top+57;
			if(PtInRect(rc,point))
			{
				SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
				m_savePoint=point;
				bMoving=true;
			}
		}
		if(!bMoving)
		{
			PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}


	CDialog::OnLButtonDown(nFlags, point);
}

void CLogoUpload::OnLButtonUp(UINT nFlags, CPoint point)
{
	bMoving=false;
	CDialog::OnLButtonUp(nFlags, point);
}


/*
Function	:OnBnClickedUpload
Notes		:����ϴ��ļ�����Ҫ���Ĺ�����
1����ͼ
2�������ļ���MD5ֵ
3��д�뱾���ļ���
Author		:Fred Huang
Date		:2008-03-18
Parameter	:na
return		:void
*/
void CLogoUpload::OnBnClickedUpload()
{
	try
	{
		if(!img)
			return;
		if(img->GetFlags()==0)
			return;

		//����һ���µ�Image�����Ա����и�ͼƬ
		CImage imageSave;
		imageSave.Create(LARGE_WIDTH,LARGE_HEIGHT,16,BI_RGB);
		HDC hdc=imageSave.GetDC();
		int w=img->GetWidth()>LARGE_WIDTH?LARGE_WIDTH:img->GetWidth();
		int h=img->GetHeight()>LARGE_HEIGHT?LARGE_HEIGHT:img->GetHeight();

		int l=w<LARGE_WIDTH?(LARGE_WIDTH-w)/2:0;
		int t=h<LARGE_HEIGHT?(LARGE_HEIGHT-h)/2:0;

		Graphics g(hdc);
		//����ͼƬ
		CString tempImage;
		tempImage.Format(TEXT("%s\\CustomFace\\TempFace.png"),CBcfFile::GetAppPath());
		if(m_cutType==0)
			g.DrawImage(img,Rect(l,t,w,h),x,y,w,h,UnitPixel);
		//m_image.Draw(hdc,l,t,w,h,x,y,w,h);
		else
		{
			g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			g.DrawImage(img,Rect(0,0,LARGE_WIDTH,LARGE_HEIGHT),0,0,img->GetWidth(),img->GetHeight(),UnitPixel);
		}
		g.ReleaseHDC(hdc);
		//	m_image.Draw(hdc,0,0,LARGE_WIDTH,LARGE_HEIGHT,0,0,m_image.GetWidth(),m_image.GetHeight());
		//m_image.Draw(hdc,l,t,w,h,x,y,w,h);
		imageSave.Save(tempImage,Gdiplus::ImageFormatPNG);//Gdiplus::ImageFormatBMP);
		imageSave.ReleaseDC();
		imageSave.Destroy();

		//���¼���ͼƬ�ļ���MD5ֵ
		CFile fr(tempImage,CFile::modeRead);
		int len=fr.GetLength();
		byte *buf=new byte[len+1];
		ZeroMemory(buf,len+1);
		fr.Read(buf,len);
		fr.Close();

		//	MD5_CTX md5;
		//	md5.MD5Update(buf,len);
		//	ZeroMemory(md5filename,32);
		//	md5.MD5Final(md5filename);

		CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
		CString md5file;
		//��ȡ��ǰģ������
		TCHAR sz[200];
		GetModuleFileName(AfxGetInstanceHandle(),sz,sizeof(sz));
		char *p = strrchr(sz,'\\');
		p++;
		*p = '\0';
		//
		md5file.Format("%sCustomFace\\%d.png",sz,pWnd->m_PlaceUserInfo.dwUserID);

		CFile fw(md5file,CFile::modeCreate|CFile::modeWrite|CFile::modeNoInherit);

		fw.Write(buf,len);

		fw.Close();
		delete buf;

		uploadCustomFace(0);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return ;
}

/*
Function	:uploadCustomFace
Notes		:��ʽ�ϴ��ļ����ֿ��ϴ�
Author		:Fred Huang
Date		:2008-03-18
Parameter	:
int	nBlockIndex	�ļ��������
return		:void
*/
bool CLogoUpload::uploadCustomFace(int nBlockIndex)
{
	try
	{
		CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
		if(!pParentWnd)
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			
			//AFCMessageBox(fMsg.GetKeyVal("LogoUploadDlg","SystemError","ϵͳ����"),fMsg.GetKeyVal("LogoUploadDlg","Tip","��ʾ"));
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("LogoUploadDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("LogoUploadDlg","SystemError","ϵͳ����"));
			return false;
		}
		//��ȡ��ǰģ������
		TCHAR sz[200];
		GetModuleFileName(AfxGetInstanceHandle(),sz,sizeof(sz));
		char *p = strrchr(sz,'\\');
		p++;
		*p = '\0';

		CString logoFile;
		logoFile.Format("%sCustomFace\\%d.png",sz,pWnd->m_PlaceUserInfo.dwUserID);

		char buf[513]={0};
		CFile fr(logoFile,CFile::modeRead);
		int filelen=fr.GetLength();			//�ļ���С
		int packagesize=512;				//���ݿ��С
		if(nBlockIndex*512>filelen)			//�Ƿ񳬳����ļ��Ľ�β
		{
			fr.Close();
			return false;
		}
		if(nBlockIndex*512+packagesize>filelen)		//���һ���С�Ƿ񳬳����ļ��Ľ�β
			packagesize=filelen-nBlockIndex*512;	//�����С������

		fr.Seek(nBlockIndex*512,CFile::begin);		//�ļ���λ
		fr.Read(buf,packagesize);					//������
		fr.Close();									//�ر��ļ�

		//������ݽṹ
		MSG_UL_C_UPLOAD msg;
		ZeroMemory(&msg,sizeof(MSG_UL_C_UPLOAD));
		msg.dwUserID=pWnd->m_PlaceUserInfo.dwUserID;	//�û�ID
		msg.dwUserSourceLogo=pWnd->m_PlaceUserInfo.bLogoID%0x100;
		//if(pWnd->m_PlaceUserInfo.bLogoID%0x100)

		msg.nBlockIndex=nBlockIndex;					//���ݿ�����
		msg.nBlockSize=512;								//���ݿ��С
		msg.nPackageSize=packagesize;					//���ݰ��Ĵ�С
		msg.nFileSize=filelen;							//�ļ���С
		msg.nBlockCount=(filelen-1)/512+1;				//���ݿ�����
		//	memcpy(msg.szLogoFile,md5filename,16);			//�ļ�����
		memcpy(msg.szData,buf,packagesize);				//����

		if(nBlockIndex==0)
			m_progress.SetRange(0,msg.nBlockCount);

		pWnd->SendData(&msg,sizeof(msg),MDM_GR_USER_LOGO,ASS_ULC_UPLOAD,0);

	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

	//	if(nBlockIndex<msg.nBlockCount-1)
	//		uploadCustomFace(nBlockIndex+1);
	return false;
}

void CLogoUpload::SetParentWnd(CWnd* pWnd)
{
	pParentWnd=pWnd;
}
/*
Function	:OnNetMessage
Notes		:��ʽ�ϴ��ļ����ֿ��ϴ�����������������Ϣ
Author		:Fred Huang
Date		:2008-03-18
Parameter	:
NetMessageHead	:������Ϣͷ
void *			:����ָ��
UINT			:���ݰ��Ĵ�С
return		:void
*/
void CLogoUpload::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	try
	{
		switch(pNetHead->bAssistantID)
		{
		case ASS_ULS_UPLOADSUCCEED:
			OnUploadSucceed(pNetData);
			break;
		case ASS_ULS_UPLOADFAILED:
			{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			
			//AFCMessageBox(fMsg.GetKeyVal("LogoUploadDlg","UploadPicFailed","�Բ����Զ���ͷ���ϴ�ʧ�ܣ������Ժ����ԣ�"),fMsg.GetKeyVal("LogoUploadDlg","Error","����"));
	         DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("LogoUploadDlg","Error","����"),false,fMsg.GetKeyVal("LogoUploadDlg","UploadPicFailed","�Բ����Զ���ͷ���ϴ�ʧ�ܣ������Ժ����ԣ�"));
			OnBnClickedCancel();
			}
			break;
		case ASS_ULS_LOGOINFORMATION:
			OnCheckLocalLogo(pNetData);
			break;
		case ASS_ULS_DOWN:
			OnDownloadFace(pNetData);
			break;
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return;
}

/*
Function	:OnUploadSucceed
Notes		:�����������ϴ��ɹ�����Ҫע���������ֻ��˵�����ϴ���һ���ļ���ɹ��������������ļ��ϴ��ɹ������Ի�Ҫ���Ƿ������һ�������ϴ��ķ���
Author		:Fred Huang
Date		:2008-03-18
Parameter	:
void *			:��������
return		:void
*/
void CLogoUpload::OnUploadSucceed(void *pData)
{
	try
	{
		MSG_UL_S_UPLOADRESULT * msg=(MSG_UL_S_UPLOADRESULT*)pData;
		CMainRoomEx * pWnd=(CMainRoomEx*)pParentWnd;
		if(msg->dwUserID!=pWnd->m_PlaceUserInfo.dwUserID)//���Ǹ��Լ�����Ϣ
			return;
		if(msg->nBlockIndex==msg->nBlockCount-1)
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			m_progress.SetPos(msg->nBlockCount);
			if(msg->bNeedCheck)
			{
				//AFCMessageBox(fMsg.GetKeyVal("LogoUploadDlg","UploadPicSuccessWait","�Զ���ͷ�����ϴ����\n\n�ڹ���Ա���ͨ�������µ�¼������Ч��"),fMsg.GetKeyVal("LogoUploadDlg","Tip","��ʾ"));//�ļ����ϴ����
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("LogoUploadDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("LogoUploadDlg","UploadPicSuccessWait","�Զ���ͷ�����ϴ����\n\n�ڹ���Ա���ͨ�������µ�¼������Ч��"));
			}
			else
			{
				//AFCMessageBox(fMsg.GetKeyVal("LogoUploadDlg","UploadPicSuccess","�Զ���ͷ�����ϴ���ϣ�"),fMsg.GetKeyVal("LogoUploadDlg","Tip","��ʾ"));//�ļ����ϴ����
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("LogoUploadDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("LogoUploadDlg","UploadPicSuccess","�Զ���ͷ�����ϴ���ϣ�"));
				::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_USER+0x10000,0,0);
			}		
			OnBnClickedCancel();
		}
		else
		{
			m_progress.SetPos(msg->nBlockIndex++);
			uploadCustomFace(msg->nBlockIndex);//�����ϴ�����������
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

/*
Function	:checkUserLogo
Notes		:�������ѯ���û��Զ���ͷ���������ݣ��ӷ�������Ҫ�����û�ͷ���ļ���MD5ֵ
Author		:Fred Huang
Date		:2008-03-19
Parameter	:
int			:�û���ID�ţ��Լ���ͷ��ҲҪ�����жϣ���Ҫ��Ϊ�˴���Ӳ�ͬ�ĵ��Ե�¼
return		:void
*/
void CLogoUpload::checkUserLogo(int dwUserID)
{
	try
	{
		MSG_UL_C_INFORREQUEST msg;
		ZeroMemory(&msg,sizeof(MSG_UL_C_INFORREQUEST));
		msg.dwRequestUserID=dwUserID;
		CMainRoomEx * pWnd=(CMainRoomEx*)pParentWnd;		//�������ߵ�ID
		msg.dwUserID=pWnd->m_PlaceUserInfo.dwUserID;		//�����ߵ�ID�����Լ���ID

		pWnd->SendData(&msg,sizeof(MSG_UL_C_INFORREQUEST),MDM_GR_USER_LOGO,ASS_ULC_INFOREQUEST,0);

	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

/*
Function	:OnCheckLocalLogo
Notes		:�յ����������ص�ͷ���ļ�MD5���ͱ��صĽ���ƥ�䣬�����һ����������ӷ���������
Author		:Fred Huang
Date		:2008-03-19
Parameter	:
void*		:�ӷ��������ص�����ָ�룬ָ��ṹ�� MSG_UL_S_INFORMATION
return		:void
*/

void CLogoUpload::OnCheckLocalLogo(void * pData)
{
	try
	{
		MSG_UL_S_INFORMATION *UL_Info=(MSG_UL_S_INFORMATION*)pData;

		CString strmsg;

		strmsg.Format("dwUserID=%d LogoMD5Server=%s dwUserLogoID=%d\r\n",UL_Info->dwUserID,UL_Info->szFileMD5,UL_Info->dwUserLogoID);
		OutputDebugString(strmsg);

		if(UL_Info->dwUserLogoID<0xFF || UL_Info->dwUserLogoID>0x1FF)
			return;
		CString LogoMD5Server=UL_Info->szFileMD5;
		if(LogoMD5Server=="")
			return;

		bool bNeedDownload=false;//�Ƿ���Ҫ�����µ��ļ�
		CString logoFile;
		logoFile.Format("%sCustomFace\\%d.png",CBcfFile::GetAppPath(),UL_Info->dwUserID);
		CFileFind ff;
		if(!ff.FindFile(logoFile))
			bNeedDownload=true;	//�����ļ�δ�ҵ���ֱ����Ҫ���������µ��ļ�
		ff.Close();
		if(!bNeedDownload)
		{//���ش����ļ������MD5ֵ�Ƿ�һ��
			CFile fr(logoFile,CFile::modeRead);
			int len=fr.GetLength();
			byte *buf=new byte[len+1];
			ZeroMemory(buf,len+1);
			fr.Read(buf,len);
			fr.Close();

			MD5_CTX md5;
			md5.MD5Update(buf,len);
			byte *md5buf=new byte[16];
			md5.MD5Final(md5buf);

			CString md5str="",szchar;
			for(int i=0;i<16;i++)
			{
				szchar.Format("%2x",md5buf[i]);
				md5str+=szchar;
			}

			delete buf;
			delete md5buf;

			if(md5str!=LogoMD5Server)
				bNeedDownload=true;
		}

		if(!bNeedDownload)
			return;
		strmsg.Format("ASS_ULC_DOWNREQUEST\r\n");
		OutputDebugString(strmsg);

		//��������ͷ������
		MSG_UL_C_DOWNREQUEST msg;
		ZeroMemory(&msg,sizeof(MSG_UL_C_DOWNREQUEST));
		msg.dwRequestUserID=UL_Info->dwUserID;
		CMainRoomEx * pWnd=(CMainRoomEx*)pParentWnd;		//�������ߵ�ID
		msg.dwUserID=pWnd->m_PlaceUserInfo.dwUserID;		//�����ߵ�ID�����Լ���ID

		pWnd->SendData(&msg,sizeof(MSG_UL_C_DOWNREQUEST),MDM_GR_USER_LOGO,ASS_ULC_DOWNREQUEST,0);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return;
}

/*
Function	:OnDownloadFace
Notes		:�յ����������ص�ͷ�����ݣ�ֱ��д���ļ���
Author		:Fred Huang
Date		:2008-03-19
Parameter	:
void*		:�ӷ��������ص�����ָ�룬ָ��ṹ�� MSG_UL_S_DOWN
return		:void
*/
void CLogoUpload::OnDownloadFace(void * pData)
{
	try
	{
		MSG_UL_S_DOWN * msg=(MSG_UL_S_DOWN *)pData;

		CString savePath;
		savePath.Format("%s\\CustomFace\\%d.png",CBcfFile::GetAppPath(),msg->dwUserID);

		UINT dwFlag=CFile::modeWrite;
		if(msg->nBlockIndex==0)
			dwFlag|=CFile::modeCreate;

		MSG_UL_C_DOWNRESULT resultMsg;
		ZeroMemory(&resultMsg,sizeof(MSG_UL_C_DOWNRESULT));
		resultMsg.dwUserID=msg->dwUserID;
		resultMsg.nBlockCount=msg->nBlockCount;
		resultMsg.nBlockIndex=msg->nBlockIndex;
		try
		{
			CFile fw(savePath,dwFlag);
			fw.Seek(msg->nBlockIndex*512,CFile::begin);
			fw.Write(msg->szData,msg->nPackageSize);
			fw.Close();
		}
		catch (...) {
			return;
		}

		//	pWnd->SendData(uIndex,&resultMsg,sizeof(MSG_UL_S_UPLOADRESULT),MDM_GR_USER_LOGO,ASS_ULS_UPLOADSUCCEED,dwHandleID,0);
		TRACE("Receive Custom Logo File %d of %d at %s\n",msg->nBlockIndex,msg->nBlockCount,savePath);
		if(msg->nBlockIndex<msg->nBlockCount-1)
		{
			CMainRoomEx * pWnd=(CMainRoomEx*)pParentWnd;		//�������ߵ�ID
			pWnd->SendData(&resultMsg,sizeof(MSG_UL_C_DOWNRESULT),MDM_GR_USER_LOGO,ASS_ULC_DOWNRESULT,0);

		}
		else
		{
			TRACE("Download Receive Custom Logo Completed\n");
			//���������
			CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
			if(msg->dwUserID==pWnd->m_PlaceUserInfo.dwUserID)
			{
				::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_USER+0x10000,0,0);
			}
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

void CLogoUpload::drawSkin(void)
{
	try
	{
		if(!imgDialog)
			return;
		CDC *pdc=this->GetWindowDC();
		if(!pdc)return;
		CRect rc;
		this->GetWindowRect(&rc);
		Graphics g(pdc->m_hDC);
		int topHeight=GetSystemMetrics(SM_CYFRAME)+GetSystemMetrics(SM_CYSIZE)+6;
		g.DrawImage(imgDialog,RectF(0.0f,0.0f,10.0f,topHeight),0.0f,0.0f,10.0f,topHeight,UnitPixel);
		g.DrawImage(imgDialog,RectF(10.0f,0.0f,rc.Width()-20,topHeight),9.0f,0.0f,1.0f,topHeight,UnitPixel);
		g.DrawImage(imgDialog,RectF((REAL)rc.Width()-10,0.0f,10.0f,topHeight),15.0f,0.0f,10.0f,topHeight,UnitPixel);
		m_bkcolor=pdc->GetPixel(9,topHeight-1);

		int bottomHeight=GetSystemMetrics(SM_CYFRAME)+3;
		g.DrawImage(imgDialog,RectF(0.0f,topHeight,GetSystemMetrics(SM_CXFRAME)+5.0f,rc.Height()-topHeight-bottomHeight),0.0f,30.0f,GetSystemMetrics(SM_CXFRAME)+5.0f,60.0f,UnitPixel);
		g.DrawImage(imgDialog,RectF(rc.Width()-GetSystemMetrics(SM_CXFRAME)-5.0f,topHeight,GetSystemMetrics(SM_CXFRAME)+5.0f,rc.Height()-topHeight-bottomHeight),20-GetSystemMetrics(SM_CXFRAME),30.0f,GetSystemMetrics(SM_CXFRAME)+5.0f,60.0f,UnitPixel);
		//g.DrawImage(imgDialog,RectF(rc.Width()-15,30,15,rc.Height()-40),10,30,15,60,UnitPixel);

		g.DrawImage(imgDialog,RectF(0,rc.Height()-bottomHeight,10,bottomHeight),0,100-bottomHeight,10,bottomHeight,UnitPixel);
		g.DrawImage(imgDialog,RectF(10,rc.Height()-bottomHeight,rc.Width()-20,bottomHeight),10,100-bottomHeight,1,bottomHeight,UnitPixel);
		g.DrawImage(imgDialog,RectF(rc.Width()-10,rc.Height()-bottomHeight,10,bottomHeight),15,100-bottomHeight,10,bottomHeight,UnitPixel);

		CString s="����";

		Font font(s.AllocSysString(),12,0,UnitPixel);
		SolidBrush brush(Color(255,255,255));

		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		s = fMsg.GetKeyVal("LogoUploadDlg","UploadYourPic","�Զ���ͷ���ϴ�");
		g.DrawString(s.AllocSysString(),-1,&font,PointF(5,5),0,&brush);
		g.ReleaseHDC(pdc->m_hDC);
		m_bnClose.Invalidate(FALSE);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

void CLogoUpload::setSkinIndex(int sk)
{
	try
	{
		if(sk>=0 && sk<4)
			m_skinIndex=sk;
		else
			m_skinIndex=0;
		
		CString s;
		s.Format("%s\\%sdialog\\dialog.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
		//	s.Format("%s\\image\\skin%d\\dialog\\dialog.bmp",CBcfFile::GetAppPath(true),m_skinIndex);

		

#ifndef BZ_ZLIB  //< add by wxx 
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

		wsprintf(path,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
		m_bnClose.LoadButtonBitmap(path,false);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"LOGO_UP_TRY_CATCH : [%s]",__FUNCTION__);
	}


	//m_bkcolor=RGB(44,114,233);
}

void CLogoUpload::OnBnClickedCloselogo()
{
	CDialog::OnCancel();
}

LRESULT CLogoUpload::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CLogoUpload::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetBkColor((HDC)wParam, m_bkcolor);
		if (m_bkBrush == NULL) m_bkBrush = CreateSolidBrush(m_bkcolor); 
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CLogoUpload::OnBnClickedPiccut()
{
	m_cutType=0;
	Invalidate(TRUE);
}

void CLogoUpload::OnBnClickedPicscale()
{
	m_cutType=1;
	Invalidate(TRUE);
}
LRESULT CLogoUpload::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	//��ʼ��
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key= m_skinmgr.GetKeyVal(strSkin);
	int r,g,b;
	r=f.GetKeyVal(key,"logobgcr",44);
	g=f.GetKeyVal(key,"logobgcg",114);
	b=f.GetKeyVal(key,"logobgcb",233);
	m_bkcolor=RGB(r,g,b);

	LoadSkin();

	//��ͼ����
	drawSkin();
	drawImage();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}
