#include "../stdafx.h"
#include "AFCEdit.h"
#include "AFCResource.h"
#include "AFCFunction.h"

#include "detours.h"
#include "skinsb.h"

//using namespace AFC;
//using namespace AFC::AFCEdit;
//using namespace AFC::AFCResource;

#define ID_RICH_COPY					WM_USER+20
#define ID_RICH_SELECT_ALL				WM_USER+21
#define ID_RICH_CLEAR					WM_USER+22
#define ID_RICH_AUTO_SCROOL				WM_USER+23
#define ID_RICH_CLEAR_ALL				WM_USER+24
#define ID_RICH_STOP_ALL				WM_USER+25
#define ID_RICH_SAVE					WM_USER+26
#define ID_RICH_COLOR					WM_USER+27
#define ID_RICH_DIY_CHAT				WM_USER+28

//�����߶�
#define BASE_HEIGHT						180
#define NORMAL_FONT						TEXT("����")

BEGIN_MESSAGE_MAP(CTrueEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CTrueEditEx, CTrueEdit)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CNewMiniDlg, CRichEditCtrl)
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(EN_LINK, OnEnLink)
	ON_COMMAND(ID_RICH_COPY, OnCopy)
	ON_COMMAND(ID_RICH_SELECT_ALL, OnSelectAll)
	ON_COMMAND(ID_RICH_CLEAR, OnClear)
	ON_COMMAND(ID_RICH_AUTO_SCROOL, OnAutoScroll)
	ON_COMMAND(ID_RICH_CLEAR_ALL, OnClearAll)
	ON_COMMAND(ID_RICH_STOP_ALL, OnStopAll)
	ON_COMMAND(ID_RICH_SAVE, OnSave)
	ON_COMMAND(ID_RICH_COLOR, OnSetColor)
   // ON_COMMAND(ID_RICH_DIY_CHAT,OnDIYChat)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/*******************************************************************************************************/

//���캯��
CTrueEdit::CTrueEdit()
{
	m_bFocus=false;
}

//��������
CTrueEdit::~CTrueEdit()
{
}

//�滭�ؼ�
HBRUSH CTrueEdit::CtlColor(CDC * pDC, UINT nCtlColor)
{
	pDC->SetBkMode(OPAQUE);
	if (IsWindowEnabled())
	{
		pDC->SetTextColor(CGameImageLink::m_EditResource.m_crNoFocusTXRGB);
		pDC->SetBkColor(CGameImageLink::m_EditResource.m_crNoFocusBKRGB);
		return (HBRUSH)CGameImageLink::m_EditResource.m_brNoFocus.GetSafeHandle();
	}
	pDC->SetTextColor(CGameImageLink::m_EditResource.m_crDisFocusBKRGB);
	pDC->SetBkColor(CGameImageLink::m_EditResource.m_crDisFocusBKRGB);
	return (HBRUSH)CGameImageLink::m_EditResource.m_brDiable.GetSafeHandle();
}

//�õ�����
void CTrueEdit::OnSetFocus(CWnd * pOldWnd)
{
	m_bFocus=true;
	Invalidate(TRUE);
	__super::OnSetFocus(pOldWnd);
}

//ʧȥ����
void CTrueEdit::OnKillFocus(CWnd * pNewWnd)
{
	m_bFocus=false;
	Invalidate(TRUE);
	__super::OnKillFocus(pNewWnd);
}

//������ɫ
void CTrueEdit::SetTextColor(COLORREF crFocus, COLORREF crNoFocue, COLORREF crDisble)
{
	CGameImageLink::m_EditResource.m_crFocusTXRGB=crFocus;		
	CGameImageLink::m_EditResource.m_crNoFocusTXRGB=crNoFocue;
	CGameImageLink::m_EditResource.m_crDisFocusTXRGB=crDisble;
	return;
}

//������ɫ
void CTrueEdit::SetBKColor(COLORREF crFocus, COLORREF crNoFocue, COLORREF crDisble)
{
	CGameImageLink::m_EditResource.m_crFocusBKRGB=crFocus;
	CGameImageLink::m_EditResource.m_crNoFocusBKRGB=crNoFocue;
	CGameImageLink::m_EditResource.m_crDisFocusBKRGB=crDisble;
	CGameImageLink::m_EditResource.m_brFouns.DeleteObject();
	CGameImageLink::m_EditResource.m_brNoFocus.DeleteObject();
	CGameImageLink::m_EditResource.m_brDiable.DeleteObject();
	CGameImageLink::m_EditResource.m_brFouns.CreateSolidBrush(crFocus);
	CGameImageLink::m_EditResource.m_brNoFocus.CreateSolidBrush(crNoFocue);
	CGameImageLink::m_EditResource.m_brDiable.CreateSolidBrush(crDisble);

	return;
}


/*******************************************************************************************************/

//���캯��
CTrueEditEx::CTrueEditEx()
{
	m_bProtected=true;
}

//��������
CTrueEditEx::~CTrueEditEx()
{
}

//���ں���
LRESULT CTrueEditEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message==WM_GETTEXT)||(message==EM_GETLINE)||(message==EM_SETPASSWORDCHAR))
	{
		if (m_bProtected)
		{
			return 0;
		}
		m_bProtected=true;
	}
	return __super::DefWindowProc(message, wParam, lParam);
}

//�Ƿ񱣻�״̬
bool CTrueEditEx::IsPortected() 
{ 
	return m_bProtected; 
}

//���ñ���
bool CTrueEditEx::SetProtected(bool bProtected)
{
	m_bProtected=bProtected;
	return m_bProtected;
}

//��ȡ����
int CTrueEditEx::GetEditPassWord(TCHAR * szBuffer, UINT uBufferLength)
{
	m_bProtected=false;
	return GetWindowText(szBuffer,uBufferLength);
}

/*******************************************************************************************************/

//���캯��
CImageDataObject::CImageDataObject()
{
	m_ulRefCnt=0;
	m_bRelease=false;
	memset(&m_Stgmed,0,sizeof(m_Stgmed));
	memset(&m_Format,0,sizeof(m_Format));
}

//��������
CImageDataObject::~CImageDataObject()
{
	if (m_bRelease) ::ReleaseStgMedium(&m_Stgmed);
}

//����ͼƬ
void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	STGMEDIUM Stgm;
	FORMATETC Format;
	Stgm.tymed=TYMED_GDI;
	Stgm.hBitmap=hBitmap;
	Stgm.pUnkForRelease=NULL;
	Format.cfFormat=CF_BITMAP;
	Format.ptd=NULL;
	Format.dwAspect=DVASPECT_CONTENT;
	Format.lindex=-1;
	Format.tymed=TYMED_GDI;
	SetData(&Format,&Stgm,TRUE);

	return;
}

//��ȡ����
IOleObject * CImageDataObject::GetOleObject(IOleClientSite * pOleClientSite, IStorage * pStorage)
{
	IOleObject * pOleObject=NULL;
	::OleCreateStaticFromData(this,IID_IOleObject,OLERENDER_FORMAT,&m_Format,pOleClientSite,pStorage,(void **)&pOleObject);
	return pOleObject;
}

//����ͼƬ
bool CImageDataObject::InsertBitmap(IRichEditOle * pRichEditOle, HBITMAP hBitmap)
{
	bool bSuccess=false;
	IStorage * pStorage=NULL;
	IOleObject * pOleObject=NULL;
	LPDATAOBJECT lpDataObject=NULL;
	IOleClientSite * pOleClientSite=NULL;
	CImageDataObject * pImageObject=NULL;
	try
	{
		//�����¶���
		pImageObject=new CImageDataObject;
		pImageObject->QueryInterface(IID_IDataObject,(void * *)&lpDataObject);
		pImageObject->SetBitmap(hBitmap);
		pRichEditOle->GetClientSite(&pOleClientSite);

		LPLOCKBYTES lpLockBytes=NULL;
		CreateILockBytesOnHGlobal(NULL,TRUE,&lpLockBytes);
		StgCreateDocfileOnILockBytes(lpLockBytes,STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,0,&pStorage);
		pOleObject=pImageObject->GetOleObject(pOleClientSite,pStorage);
		OleSetContainedObject(pOleObject,TRUE);

		REOBJECT ReObject;
		ZeroMemory(&ReObject,sizeof(ReObject));
		ReObject.cbStruct=sizeof(ReObject);

		CLSID ClsID;
		pOleObject->GetUserClassID(&ClsID);
		ReObject.clsid=ClsID;
		ReObject.cp=REO_CP_SELECTION;
		ReObject.dvaspect=DVASPECT_CONTENT;
		ReObject.poleobj=pOleObject;
		ReObject.polesite=pOleClientSite;
		ReObject.pstg=pStorage;
		pRichEditOle->InsertObject(&ReObject);
		bSuccess=true;
	}
	catch (...) { delete pImageObject; TRACE("%s\n",__FUNCTION__);}

	if (pOleObject!=NULL) pOleObject->Release();
	if (pOleClientSite!=NULL) pOleClientSite->Release();
	if (pStorage!=NULL) pStorage->Release();
	if (lpDataObject!=NULL) lpDataObject->Release();

	return bSuccess;
}


/*******************************************************************************************************/

//���캯��
CNewMiniDlg::CNewMiniDlg()
{
	m_bMenu=true;
	m_bAuto=true;
	m_bCanInsert=true;
	m_pRichEditOle=NULL;
	m_GameOrHall=0;
	m_FontName=TEXT("����");
	m_Cursor=LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));
	LoadScrollSkin();	
}

//��ʼ��������Ƥ��
void CNewMiniDlg::LoadScrollSkin()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + "..\\" + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder","..\\" + m_skinmgr.GetSkinPath());
	wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
	HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SkinSB_Init(this->GetSafeHwnd(), hbmScorll);
}

//��������
CNewMiniDlg::~CNewMiniDlg()
{
	if (m_pRichEditOle!=NULL) m_pRichEditOle->Release();
}

//��ʼ������
bool CNewMiniDlg::Init()
{
	if (m_pRichEditOle==NULL) 
		m_pRichEditOle=GetIRichEditOle();
	//ʵ����������
	SendMessage( (UINT)EM_SETEVENTMASK,(WPARAM)0, (LPARAM)ENM_LINK);
	return (m_pRichEditOle!=NULL);
}

BOOL CNewMiniDlg::ShowWindow(int nCmdShow)
{
	BOOL ret = CRichEditCtrl::ShowWindow(nCmdShow);
	return	ret;
}

HBITMAP   CNewMiniDlg::CopyBitmap(HBITMAP   hSourceHbitmap)  
{   
  CDC sourceDC;   
  CDC destDC;   
  sourceDC.CreateCompatibleDC(NULL);   
  destDC.CreateCompatibleDC(NULL);   
  //The   bitmap   information.   
  BITMAP   bm = {0};   
  //Get   the   bitmap   information.   
  ::GetObject(hSourceHbitmap,   sizeof(bm),   &bm);   
  //   Create   a   bitmap   to   hold   the   result   
  HBITMAP   hbmResult   =   ::CreateCompatibleBitmap(CClientDC(NULL),   bm.bmWidth,   bm.bmHeight);   
    
  HBITMAP   hbmOldSource   =   (HBITMAP)::SelectObject(   sourceDC.m_hDC,   hSourceHbitmap);   
  HBITMAP   hbmOldDest   =   (HBITMAP)::SelectObject(   destDC.m_hDC,   hbmResult   );   
  destDC.BitBlt(0,0,bm.bmWidth,   bm.bmHeight,   &sourceDC,   0,   0,   SRCCOPY   );   
    
  //   Restore   DCs   
  ::SelectObject(   sourceDC.m_hDC,   hbmOldSource   );   
  ::SelectObject(   destDC.m_hDC,   hbmOldDest   );   
  ::DeleteObject(sourceDC.m_hDC);   
  ::DeleteObject(destDC.m_hDC);   
    
  return   hbmResult;   
}  

HBITMAP CNewMiniDlg::CopyDCToBitmap(HDC hSrc,int x1,int y1,int x2,int y2)  //hSrcһ������Ϊ�ڴ�DC
{
	HBITMAP hBitmap,hOldBitmap;
	HDC hMemDC;
	int nWidth,nHeight;

	nWidth=x2-x1;
	nHeight=y2-y1;

	hMemDC=CreateCompatibleDC(hSrc);
	hBitmap=CreateCompatibleBitmap(hSrc,nWidth,nHeight);
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	StretchBlt(hMemDC,0,0,nWidth,nHeight,hSrc,x1,y1,nWidth,nHeight,SRCCOPY);
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	DeleteDC(hMemDC);
	DeleteObject(hOldBitmap);
	return hBitmap;
} 

//����ͼƬ
void CNewMiniDlg::InsertBitmap(HBITMAP bBitmap)
{
	if (m_pRichEditOle==NULL) return;

	//������Ϣ
	CHARRANGE CharRange;
	GetSel(CharRange);
	int iLineCount=GetLineCount();

	//����ͼƬ
	if (m_bCanInsert)
	{
		SetSel(GetWindowTextLength(),GetWindowTextLength());
		HBITMAP bBitmap2 = CopyBitmap(bBitmap);
		HBITMAP  hNewBitmap;
		BITMAP bit;
		CDC dest,src;
		CDC *pdc;
		pdc=GetDC();
		CBitmap bitmap;

		::GetObject(bBitmap,   sizeof(BITMAP),   &bit);
		bitmap.CreateCompatibleBitmap(pdc,bit.bmWidth,bit.bmHeight);  
		bitmap.GetBitmap(&bit);
		hNewBitmap=::CreateBitmapIndirect(&bit);
		//CBitmap bp;
		//bp.CreateCompatibleBitmap(pdc,24,24);

		COLORREF bkcolor;
		//m_bkColor=pdc->GetPixel(0,0);
		bkcolor=m_bkColor;
		byte r=GetRValue(bkcolor);
		byte b=GetBValue(bkcolor);
		byte g=GetGValue(bkcolor);
		
		if(bit.bmBitsPixel==32)
		{
			BYTE cbt[24*24*4];
			long lSize = bit.bmWidth*bit.bmHeight*4;
			BYTE *bt = new BYTE[lSize];
			if(bt == NULL)
			{
				bt = cbt;
				lSize = 24*24*4;
			}
			for(long i=0;i<lSize;i+=4)
			{
				bt[i]=b;
				bt[i+1]=g;//B
				bt[i+2]=r;//G
				bt[i+3]=0;//R
			}
			::SetBitmapBits(hNewBitmap,lSize,bt);
			if ((bt != NULL)&&(bt != cbt)){delete []bt;}
		}
		else if(bit.bmBitsPixel==24)
		{
			BYTE cbt[24*24*3];
			long lSize = bit.bmWidth*bit.bmHeight*3;
			BYTE *bt = new BYTE[lSize];
			if(bt == NULL)
			{
				bt = cbt;
				lSize = 24*24*3;
			}
			for(long i=0;i<lSize;i+=3)
			{
				bt[i]=b;
				bt[i+1]=g;//B
				bt[i+2]=r;//G
			}
			::SetBitmapBits(hNewBitmap,lSize,bt);
			if ((bt != NULL)&&(bt != cbt)){delete []bt;}
		}
		else if(bit.bmBitsPixel==16)
		{
			BYTE cbt[24*24*2];
			long lSize = bit.bmWidth*bit.bmHeight*2;
			BYTE *bt = new BYTE[lSize];
			if(bt == NULL)
			{
				bt = cbt;
				lSize = 24*24*2;
			}
			for(long i=0;i<lSize;i+=2)
			{
				WORD rgb=0;
				rgb=r>>3;
				rgb=(rgb<<6)|(g>>2);
				rgb=(rgb<<5)|(b>>3);
				//TCHAR sz[100];
				//wsprintf(sz,"%d %d %d %d",r,g,b,rgb);
				//MessageBox(sz);
				//break;
				memcpy(bt+i,&rgb,sizeof(WORD));
			}
			::SetBitmapBits(hNewBitmap,lSize,bt);
			if ((bt != NULL)&&(bt != cbt)){delete []bt;}
		}
		dest.CreateCompatibleDC(pdc);
		src.CreateCompatibleDC(pdc);
		dest.SelectObject(hNewBitmap);
		src.SelectObject(bBitmap);
		if(bit.bmBitsPixel==16)
		{
		}
		::TransparentBlt(dest.GetSafeHdc(),0,0,bit.bmWidth,bit.bmHeight,src.GetSafeHdc(),0,0,bit.bmWidth,bit.bmHeight,src.GetPixel(0,0));
		CImageDataObject::InsertBitmap(m_pRichEditOle,hNewBitmap);
		dest.DeleteDC();
		src.DeleteDC();

		if (bBitmap2 != NULL) DeleteObject(bBitmap2);
		if (hNewBitmap != NULL) DeleteObject(hNewBitmap);
		bitmap.DeleteObject();
		

	}
	//�ָ���Ϣ
	if (CharRange.cpMin != CharRange.cpMax)
	{
		SetSel(CharRange);
		HideSelection(FALSE, TRUE);
	}
	else
	{
		PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	}
	if(!SkinSB_IsValid(m_hWnd))
	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CBcfFile f( s + "skin.bcf");
		TCHAR path[MAX_PATH];
		CString skinfolder;
		skinfolder = f.GetKeyVal(Glb().m_skin,"skinfolder","..\\image\\client\\skin0\\");
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Init(m_hWnd, hbmScorll);
	}
	return;
}

//�����ַ�
void CNewMiniDlg::InsertCharString(const TCHAR * szCharString, COLORREF crColor, bool bLink, UINT uSize, TCHAR * szFontName,bool bBold)
{
	if(!this) 
		return; 
	if(!this->m_hWnd) 
		return; 

	//������Ϣ
	CHARRANGE CharRange;
	GetSel(CharRange);
	int iLineCount=GetLineCount();

	//���ø�ʽ
	CHARFORMAT2 CharFormat;
	::memset(&CharFormat,0,sizeof(CharFormat));
	CharFormat.cbSize=sizeof(CharFormat); 
	/////////////////////////////////////////////////////
	////Kylin 20090116 ��Ӵ�������ʾ
	if(bBold)
	{
		CharFormat.dwMask=CFM_FACE|CFM_SIZE|CFM_COLOR|CFE_BOLD;
		CharFormat.dwEffects=CFE_BOLD;
	}
	else 
	{
		CharFormat.dwMask=CFM_FACE|CFM_SIZE|CFM_COLOR;
		CharFormat.dwEffects &= ~CFE_BOLD;
	}
	/////////////////////////////////////////////////////
	CharFormat.crTextColor=crColor;
	CharFormat.yHeight=uSize*20+BASE_HEIGHT;
	
	if (szFontName!=NULL)
		lstrcpy(CharFormat.szFaceName,szFontName);
	else 
		lstrcpy(CharFormat.szFaceName,NORMAL_FONT);
	
	//if (bLink) 
	//{
	//	CharFormat.dwMask |=  CFM_EFFECTS;
	//	CharFormat.dwEffects = CFE_LINK;
	//}
	
	//�������������600�У����ǰ�������
	int nLineCount=GetLineCount();
	int nNum=0;
	CString str;
	if(nLineCount>600)
	{
		nNum=this->GetLine(0,str.GetBuffer(MAX_PATH),100);
		//this->LineScroll();
		//this->RepositionBars();
		SetSel(0,nNum);
		ReplaceSel("");
	}
	//������Ϣ
	if (m_bCanInsert)
	{
		int cnt = GetWindowTextLength();
		SetSel(cnt,cnt);
		ReplaceSel(szCharString);
		SetSel(cnt,GetWindowTextLength());
		SetSelectionCharFormat(CharFormat);
		SetSel(-1,0);
	}
	//�ָ���Ϣ
	//if (m_bAuto==false)
	//SetSel(CharRange);
	//CHARFORMAT2 CharFormattemp;
	//GetSelectionCharFormat(CharFormattemp);
	if (CharRange.cpMin != CharRange.cpMax)
	{
		SetSel(CharRange);
		HideSelection(FALSE, TRUE);
	}
	else
	{
		PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	}
	return;
}

//���ù��
BOOL CNewMiniDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest==HTCAPTION)
	{
		::SetCursor(m_Cursor);
		return TRUE;
	}
	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CNewMiniDlg::OnSize(UINT nType,int cx,int cy)
{
	__super::OnSize(nType,cx,cy);
}
//�Ҽ���Ϣ
void CNewMiniDlg::OnRButtonDown(UINT nFlags, CPoint Point)
{
	if (m_bMenu)
	{
		SetFocus();
		CMenu PopMenu;

		//�����˵�
		PopMenu.CreatePopupMenu();
		PopMenu.AppendMenu(0,ID_RICH_AUTO_SCROOL,TEXT("�������"));
		PopMenu.AppendMenu(0,MF_SEPARATOR);
		PopMenu.AppendMenu(0,ID_RICH_COPY,TEXT("����(&C) \tCtrl+C"));
		PopMenu.AppendMenu(0,ID_RICH_SELECT_ALL,TEXT("ȫѡ(&A) \tCtrl+A"));
		PopMenu.AppendMenu(0,ID_RICH_CLEAR,TEXT("ɾ��(&D)"));
		PopMenu.AppendMenu(0,ID_RICH_CLEAR_ALL,TEXT("�����Ļ"));
		PopMenu.AppendMenu(0,ID_RICH_STOP_ALL,TEXT("ֹͣ��ʾ������Ϣ"));
		//PopMenu.AppendMenu(0,ID_RICH_DIY_CHAT,TEXT("�Զ�����������"));
		PopMenu.AppendMenu(0,MF_SEPARATOR);
		//PopMenu.AppendMenu(0,ID_RICH_COLOR,TEXT("����������ɫ..."));
		PopMenu.AppendMenu(0,ID_RICH_SAVE,TEXT("����..."));

		//���ò˵�
		UINT nSel=((GetSelectionType()!=SEL_EMPTY)?0:MF_GRAYED);
		PopMenu.EnableMenuItem(ID_RICH_COPY,MF_BYCOMMAND|nSel);
		PopMenu.EnableMenuItem(ID_RICH_CLEAR,MF_BYCOMMAND|nSel);
		PopMenu.EnableMenuItem(ID_RICH_SELECT_ALL,MF_BYCOMMAND);
		PopMenu.CheckMenuItem(ID_RICH_AUTO_SCROOL,(m_bAuto?MF_CHECKED:MF_UNCHECKED)|MF_BYCOMMAND);
		PopMenu.CheckMenuItem(ID_RICH_STOP_ALL,(!m_bCanInsert?MF_CHECKED:MF_UNCHECKED)|MF_BYCOMMAND);

		//��ʾ�˵�
		ClientToScreen(&Point);
		PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON,Point.x,Point.y,this);
		PopMenu.DestroyMenu();
	}

	return;
}

//������Ϣ
afx_msg void CNewMiniDlg::OnSave()
{
//	CFileDialog FileDlg(FALSE,"RTF",0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�ı��ļ�(*.RTF)|*.RTF||",this);
	TCHAR path[MAX_PATH];
	
    // PengJiLin, 2010-6-7, GetCurrentDirectory()����ȫ�����
    //::GetCurrentDirectory(MAX_PATH,path);
    lstrcpy(path, CBcfFile::GetAppPath());

	CFileDialog FileDlg(FALSE,"txt",0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�ı��ļ�(*.txt)|*.txt||",this);
	if (FileDlg.DoModal()==IDOK)
	{
		CFile File;
		if (File.Open(FileDlg.GetPathName(),CFile::modeCreate|CFile::modeReadWrite|CFile::typeText)==TRUE)
		{
            EDITSTREAM EditStream;
			EditStream.dwCookie=(DWORD)&File;
			EditStream.pfnCallback=SaveCallBack; 
			StreamOut(SF_TEXT,EditStream);
			File.Close();
		}
		else 
			AFCMessageBox(TEXT("���������¼ʧ�ܣ�"));
	}
	::SetCurrentDirectory(path);
	return ;
}
void CNewMiniDlg::OnDIYChat()
{
	m_DiyChat.m_GameOrHall=m_GameOrHall;
	m_DiyChat.DoModal();
	if(GetParent()!=NULL)
		GetParent()->SendMessage(WM_RELOAD_TALKINI);
}
void CNewMiniDlg::OnSetColor()
{
	CColorDialog Color(CGameImageLink::m_ColorResource.m_crTalkTxColor,CC_FULLOPEN);
	if (Color.DoModal()==IDOK) 
		CGameImageLink::m_ColorResource.m_crTalkTxColor=Color.GetColor();
}

//�ص�����
DWORD CALLBACK CNewMiniDlg::SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb)
{
	CFile * pFile=(CFile *)dwCookie;
	pFile->Write(pbBuff,cb);
	*pcb=cb;
	return 0;		
}

//��������
void CNewMiniDlg::OnEnLink(NMHDR * pNMHDR, LRESULT * pResult)
{
	ENLINK * pEnLink=reinterpret_cast<ENLINK *>(pNMHDR);

	//������Ϣ
	if ((pEnLink->msg==WM_LBUTTONDOWN)||(pEnLink->msg==WM_LBUTTONDBLCLK))
	{
		CString strBuffer;
		strBuffer.GetBufferSetLength((pEnLink->chrg.cpMax-pEnLink->chrg.cpMin)*2);
		strBuffer.ReleaseBuffer();
		GetTextRange(pEnLink->chrg.cpMin,pEnLink->chrg.cpMax,strBuffer);

		CString s=CBcfFile::GetAppPath ();/////����·��
		CString szTitleNumber;
		CINIFile f( s + "news.ini");
		CString key="BZW_NEWS";
//		TCHAR path[MAX_PATH];
		int iNumber=0;
		CString szTitle,szAddr;
		iNumber=f.GetKeyVal(key,"Number",0);
		for(int i=1;i<=iNumber;i++)
		{
			szTitleNumber.Format("%d",i);
			szTitle=f.GetKeyVal(key,"Title"+szTitleNumber,"");
			if(strBuffer==szTitle) 
			{
				szAddr=f.GetKeyVal(key,"Addr"+szTitleNumber,"");
				if(!szAddr.IsEmpty())
				{
					ShellExecute(NULL,"open",szAddr,NULL,NULL,SW_MAXIMIZE);
					break;
				}
			}
		}

		//GetParent()->SendMessage(WM_RICH_EDIT_HIT,(WPARAM)&strBuffer,pEnLink->msg);
	}

	*pResult=0;
}

//��������
void CNewMiniDlg::OnDestroy()
{
	__super::OnDestroy();

	//������Դ
	if (m_pRichEditOle!=NULL) 
	{
		m_pRichEditOle->Release();
		m_pRichEditOle=NULL;
	}

	return;

}

//����
LRESULT CNewMiniDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadScrollSkin();
	Invalidate();

	return LRESULT(0);
}

/*******************************************************************************************************/

