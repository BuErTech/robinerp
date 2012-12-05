#include "Stdafx.h"
#include <WinUser.h>
#include "WndAnimate.h"


BEGIN_MESSAGE_MAP(CWndAnimate, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()   
END_MESSAGE_MAP()

/****************************************************************************************************************************
** Memdc ʵ��
****************************************************************************************************************************/
///
/// ���캯��
CMemdc::CMemdc()
{
	m_hCompatibleBmp = NULL;
	m_hCompatibleDC = NULL;
	m_iWidth = 0;
	m_iHeight = 0;	
}

///
/// ��������
CMemdc::~CMemdc()
{
	//Delete();
}

///
/// ��ֵ����
CMemdc	& CMemdc::operator = (const CMemdc &other)
{
	/// ��ֹ�Ը�ֵ
	if (this == &other)
	{
		return *this;
	}

	/// ɾ��ԭ����
	if (m_hCompatibleBmp != NULL)
	{
		::DeleteObject(m_hCompatibleBmp );
		m_hCompatibleBmp = NULL;
	}
	if (m_hCompatibleDC != NULL)
	{
		::DeleteDC(m_hCompatibleDC );
		m_hCompatibleDC = NULL;
	}

	/// ����������
	m_hCompatibleDC = other.m_hCompatibleDC;
	m_hCompatibleBmp = other.m_hCompatibleBmp;
	m_iWidth = other.m_iWidth;
	m_iHeight = other.m_iHeight;

	return *this;
}

///
/// ���ƹ��캯��
CMemdc::CMemdc(const CMemdc &other)
{
	m_hCompatibleDC = other.m_hCompatibleDC;
	m_hCompatibleBmp = other.m_hCompatibleBmp;
	m_iWidth = other.m_iWidth;
	m_iHeight = other.m_iHeight;
}

///
/// �����ڴ�DC
void CMemdc::Create(HDC hSrcDC, int iWidth, int iHeight)
{
	Delete();
	m_hCompatibleDC = CreateCompatibleDC(hSrcDC);

//  m_hCompatibleBmp = CreateCompatibleBitmap(hSrcDC, iWidth, iHeight );
// 	m_BmpInfo.bmiHeader.biWidth   =   iWidth; 
// 	m_BmpInfo.bmiHeader.biHeight   =   iHeight; 
// 	m_BmpInfo.bmiHeader.biBitCount   =   32; 
// 	m_BmpInfo.bmiHeader.biCompression   =   BI_RGB; 
// 	m_BmpInfo.bmiHeader.biPlanes   =   1;                                       //default 
// 	::CreateDIBitmap(hSrcDC, &m_BmpInfo.bmiHeader, CBM_INIT, m_pData, &m_BmpInfo, DIB_RGB_COLORS);

	m_BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BmpInfo.bmiHeader.biBitCount = 4 * 8;
	m_BmpInfo.bmiHeader.biCompression = BI_RGB;
	m_BmpInfo.bmiHeader.biPlanes = 1;
	m_BmpInfo.bmiHeader.biWidth = iWidth;
	m_BmpInfo.bmiHeader.biHeight = iHeight;
	m_BmpInfo.bmiHeader.biSizeImage = 4 * iWidth * iHeight;
	
	m_hCompatibleBmp = CreateDIBSection(hSrcDC, &m_BmpInfo, DIB_RGB_COLORS, (void **)&m_pData, 0, 0);
	SelectObject(m_hCompatibleDC , m_hCompatibleBmp );

	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

///
/// �õ�����DC
const HDC CMemdc::GetCompatibleDC(void) const
{
	return m_hCompatibleDC;
}
/// 
/// �õ����
/// @param	void
/// @return	int		���ؿ��
int CMemdc::GetWidth()
{
	return m_iWidth;
}
/// �õ��߶�
/// @param	void
/// @return	int		���ظ߶�
int CMemdc::GetHeight()
{
	return m_iHeight;
}
///
/// ɾ���ڴ�DC
void CMemdc::Delete(void)
{
	if (m_hCompatibleBmp != NULL)
	{
		::DeleteObject(m_hCompatibleBmp );
		m_hCompatibleBmp = NULL;
	}
	if (m_hCompatibleDC != NULL)
	{
		::DeleteDC(m_hCompatibleDC );
		m_hCompatibleDC = NULL;
	}
}
/****************************************************************************************************************************
** CWndAnimate ʵ��
****************************************************************************************************************************/
CWndAnimate::CWndAnimate()
{
	m_iX = 0;
	m_iY = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	
	for (int i=0; i<200; i++)
	{
		m_iFlip[i] = 100;
	}
	//m_iFlip = 100;
	m_pParent = NULL;
	m_bShowWindow = true;

	m_nFrameCount = 0;
	m_nPos = 0;
	m_nTime = 0;
	m_nTotalRound = 1;

	m_bPlaying = false;
	m_IsLoop   = false;
}

CWndAnimate::~CWndAnimate()
{
	StopAnimate();
}
///
/// ����
///@param[in] CWnd *pParent ������
///@return void
bool CWndAnimate::Create(CWnd *pParent)
{
	if (NULL == pParent)
	{
		return false;
	}

	if (CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649))), NULL, WS_POPUP ,
		0, 0, 10, 10, pParent->GetSafeHwnd(), NULL) < 0)
	{
		return false;
	}

	m_pParent = pParent;

	DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
	{
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^(0x80000|WS_EX_TRANSPARENT));
	}

	return true;
}

//�����С����
void CWndAnimate::OnActivate(UINT uState, CWnd* pOldWnd, BOOL bMinisted)
{
	ShowWindow(0);
	return;
    /*if(m_pMainRoom->GetSafeHwnd() && WA_INACTIVE != uState)
    {
        m_pMainRoom->SetActiveToMainRoom(uState);
    }*/
}

/// �Ӱ��ж�ȡImage��ע��Ҫ�ֶ��ͷ� 
Image *CWndAnimate::InitImageFromPkg(const wchar_t *pszFileName)
{
	// ��ѹ�����ж�ȡ
	char *nData = NULL;
	int nLength = 0;
	USES_CONVERSION;

	CUnZipRes unZip;
	nData = unZip.LoadFileBufferFromZip(W2A(pszFileName), nLength);
	CString s;
	s.Format("pzl nLength=%d",nLength);
	OutputDebugString(s);
	if(nData && nLength)
	{
		// ������
		HGLOBAL   hMem   =   GlobalAlloc(GMEM_FIXED,   nLength); 
		BYTE*   pmem   =   (BYTE*)GlobalLock(hMem); 
		memcpy(pmem, nData, nLength); 
		IStream*   pstm; 
		CreateStreamOnHGlobal(hMem,FALSE,&pstm); 

		Image *img = Image::FromStream(pstm);		

		// �ͷ���Դ 
		GlobalUnlock(hMem); 
		pstm-> Release(); 
		if(nData) delete nData;
		nData = 0;

		if (NULL == img)
		{
			img = Image::FromFile(pszFileName);
		}

		return img;
	}
	else
	{
		Image *img = Image::FromFile(pszFileName);
		return img;
	}

	return NULL;
}
///
/// ����һ��PNGͼƬ
///@param[in] pszFileName* PNG�ļ���(Unicode)
void CWndAnimate::AppendPNG(const wchar_t *pszFileName)
{
	Image *img = InitImageFromPkg(pszFileName);

	if (NULL==img)
	{
		return;
	}

	if (img->GetWidth()<=0 || img->GetHeight()<=0)
	{
		return;
	}

	CMemdc memDC;
	memDC.Create(GetDC()->GetSafeHdc(), img->GetWidth(), img->GetHeight());

	/// ��ͼ
	Graphics grac(memDC.GetCompatibleDC());
	grac.DrawImage(img, Rect(0, 0, img->GetWidth(), img->GetHeight()),
		0, 0, img->GetWidth(), img->GetHeight(), UnitPixel);
	grac.ReleaseHDC(memDC.GetCompatibleDC());

	m_FrameList.push_back(memDC);
	m_nFrameCount ++;

	delete img;
	img = NULL;
}

/// 
/// ����һ��PNGͼƬ���ᶨ��С��λ��
/// @param[in]		const wchar_t	pszFileName*		ͼƬ�ļ���(Unicode)
/// @param[in]		int				nSrcX
/// @param[in]		int				nSrcY
/// @param[in]		int				nWidth
/// @param[in]		int				nHeight
void CWndAnimate::AppendPNG(const wchar_t *pszFileName, int nSrcX, int nSrcY, int nWidth, int nHeight)
{
	Image *img = InitImageFromPkg(pszFileName);

	if (NULL==img)
	{
		return;
	}

	if (img->GetWidth()<=0 || img->GetHeight()<=0)
	{
		return;
	}

	CMemdc memDC;
	memDC.Create(GetDC()->GetSafeHdc(), nWidth, nHeight);

	/// ��ͼ
	Graphics grac(memDC.GetCompatibleDC());
	grac.DrawImage(img, Rect(0, 0, memDC.GetWidth(), memDC.GetHeight()),
		nSrcX, nSrcY, nWidth, nHeight, UnitPixel);
	grac.ReleaseHDC(memDC.GetCompatibleDC());

	m_FrameList.push_back(memDC);
	m_nFrameCount ++;

	delete img;
	img = NULL;
}

/// ����һ��ͼƬ��ͼƬ����ֳɼ�֡
/// @param[in]		const wchar_t	pszFileName*		ͼƬ�ļ���(Unicode)
/// @param[in]		int				nCutInHor			����ֳɼ�֡
void CWndAnimate::AppendPNG(const wchar_t *pszFileName, int nCutInHor)
{
	Image *img = InitImageFromPkg(pszFileName);

	if (NULL==img)
	{
		return;
	}

	if (img->GetWidth()<=0 || img->GetHeight()<=0)
	{
		return;
	}

	for (int i=0; i<nCutInHor; ++i)
	{
		CMemdc memDC;
		memDC.Create(GetDC()->GetSafeHdc(), img->GetWidth()/nCutInHor, img->GetHeight());

		/// ��ͼ
		Graphics grac(memDC.GetCompatibleDC());
		grac.DrawImage(img, Rect(0, 0, memDC.GetWidth(), memDC.GetHeight()),
			img->GetWidth()/nCutInHor*i, 0, img->GetWidth()/nCutInHor, img->GetHeight(), UnitPixel);
		grac.ReleaseHDC(memDC.GetCompatibleDC());

		m_FrameList.push_back(memDC);
		m_nFrameCount ++;
	}	

	delete img;
	img = NULL;
}


/// ����һ��ͼƬ��ͼƬ����ֳɼ�֡�����ؼ�ɫ
/// @param[in]		const wchar_t	pszFileName*		ͼƬ�ļ���(Unicode)
/// @param[in]		int				nCutInHor			����ֳɼ�֡
void CWndAnimate::AppendPNG(const wchar_t *pszFileName, COLORREF clrKey, int nCutInHor)
{
	Image *img = NULL;

	img = InitImageFromPkg(pszFileName);


	if (NULL == img)
	{
		return;
	}

	if (img->GetWidth()<=0 || img->GetHeight()<=0)
	{
		return;
	}

	ImageAttributes ima;
	ima.SetColorKey(Color(0, GetRValue(clrKey), GetGValue(clrKey), GetBValue(clrKey)), 
		Color(255, GetRValue(clrKey), GetGValue(clrKey), GetBValue(clrKey)));
	for (int i=0; i<nCutInHor; ++i)
	{
		CMemdc memDC;
		memDC.Create(GetDC()->GetSafeHdc(), img->GetWidth()/nCutInHor, img->GetHeight());

		/// ��ͼ
		Graphics grac(memDC.GetCompatibleDC());
		grac.DrawImage(img, Rect(0, 0, memDC.GetWidth(), memDC.GetHeight()),
			img->GetWidth()/nCutInHor*i, 0, img->GetWidth()/nCutInHor, img->GetHeight(), UnitPixel, &ima);
		grac.ReleaseHDC(memDC.GetCompatibleDC());

		m_FrameList.push_back(memDC);
		m_nFrameCount ++;
	}	

	delete img;
	img = NULL;
}
void CWndAnimate::AppendGif(const wchar_t *pszFileName)
{

	Image *img = InitImageFromPkg(pszFileName);

	if (NULL==img)
	{
		return;
	}

	if (img->GetWidth()<=0 || img->GetHeight()<=0)
	{
		return;
	}
	int temp = img->GetWidth();
	//GUID   pageGuid = FrameDimensionTime;

	UINT nCutInHor = img->GetFrameDimensionsCount(); 
	GUID *m_pDimensionIDs =new GUID[nCutInHor]; 
	img->GetFrameDimensionsList(m_pDimensionIDs, nCutInHor); 
	WCHAR strGuid[39]; 
	StringFromGUID2(m_pDimensionIDs[0], strGuid, 39); 
	int m_FrameCount = img->GetFrameCount(&m_pDimensionIDs[0]); //PropertyTagFrameDelay��GDI+��Ԥ�����һ��GIG����IDֵ����ʾ��ǩ֡���ݵ��ӳ�ʱ�� 
	UINT TotalBuffer = img->GetPropertyItemSize(PropertyTagFrameDelay); 
	PropertyItem * m_pItem = (PropertyItem*)malloc(TotalBuffer); img->GetPropertyItem(PropertyTagFrameDelay,TotalBuffer,m_pItem); 


	for (int i=0; i<m_FrameCount; ++i)
	{
		CMemdc memDC;
		memDC.Create(GetDC()->GetSafeHdc(), img->GetWidth(), img->GetHeight());

		/// ��ͼ
		Graphics grac(memDC.GetCompatibleDC());
		GUID   Guid = FrameDimensionTime;
		img->SelectActiveFrame(&Guid,i); 
		m_iFlip[i] = ((UINT*)m_pItem[0].value)[i] * 10;
		grac.DrawImage(img,0,0, img->GetWidth(), img->GetHeight());
		grac.ReleaseHDC(memDC.GetCompatibleDC());

		m_FrameList.push_back(memDC);
		m_nFrameCount ++;
	}	

	delete img;
	img = NULL;

}

///
/// ��ʼ����
///@param[in] iX, iY ����
///@return void
void CWndAnimate::BeginAnimate(int iX, int iY, int iWidth, int iHeight, int nRound, int iFlip,bool IsLoop)
{
	if (GetIsPlaying())
	{
		return;
	}

	if (NULL == m_pParent)
	{
		return;
	}
	if (m_nFrameCount<=0)
	{
		return;
	}

	m_nPos = 0;
	m_nTime = 0;
	m_nTotalRound = nRound;
	m_iX = iX;
	m_iY = iY;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	if (iFlip != 100)
	{
		for (int i=0; i<m_nFrameCount; i++)
		{
			m_iFlip[i] = iFlip;
		}
		
	}
	m_IsLoop = IsLoop;

	if (0 == m_iWidth)
	{		
		m_iWidth = m_FrameList[0].GetWidth();
	}
	if (0 == m_iHeight)
	{
		m_iHeight = m_FrameList[0].GetHeight();
	}

	CPoint ptDst(m_iX, m_iY);
	m_pParent->ClientToScreen(&ptDst);

	MoveWindow(ptDst.x, ptDst.y, m_iWidth, m_iHeight);

	m_bPlaying = true;
	SetTimer(TIME_FRAME, m_iFlip[0], NULL);
	SetTimer(TIME_RENDER, 5, NULL);

	SetWindowPos(m_pParent,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
}

/// 
/// ��ʱ����Ӧ��Ϣ
/// @param[in]		UINT_PTR nIDEvent		��ʱ��ID
void CWndAnimate::OnTimer(UINT_PTR nIDEvent)
{
	if (!m_bShowWindow)
	{
		ShowWindow(SW_HIDE);
		return;
	}

	if (TIME_RENDER == nIDEvent)
	{
		/// ��Ⱦ����
		RenderAnimate();
	}
	else if (TIME_FRAME == nIDEvent)
	{
		KillTimer(TIME_FRAME);
		
		OnFrameMove();
	}

}


/**
* @brief ��������ƶ�����λ�á�������һ֡�ȵ�
*/
void CWndAnimate::OnFrameMove()
{
	m_nPos ++;
	m_nPos = m_nPos%m_FrameList.size();
	SetTimer(TIME_FRAME, m_iFlip[m_nPos], NULL);
	if (m_nPos==0)
		m_nTotalRound--;
	if (m_nPos==0 && m_nTotalRound <=0 && m_IsLoop == false)
	{
		m_nPos = 0;
		StopAnimate();
	}
}

/// ֹͣ����
void CWndAnimate::StopAnimate(void)
{
	m_bPlaying = false;
	KillTimer(TIME_FRAME);
	KillTimer(TIME_RENDER);
	ShowWindow(SW_HIDE);
}

/// ��Ⱦ����
void CWndAnimate::RenderAnimate(void)
{
	/// �ö�����
	//BringWindowToTop();
	//SetWindowPos(m_pParent,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	/// ��������
	CRect rct;
	GetWindowRect(&rct);
	POINT dst;
	dst.x = m_iX;//rct.left;
	dst.y = m_iY;//rct.top;
	m_pParent->ClientToScreen(&dst);

	BLENDFUNCTION blend;
	blend.AlphaFormat = 1;
	blend.BlendFlags = 0;
	blend.BlendOp = 0;
	blend.SourceConstantAlpha = 255;	

	if (m_nPos>=m_nFrameCount)
	{
		return;
	}

	/// ����ͼƬ�����λ�����
	HDC hScreendc = GetDC()->m_hDC;
	/// ���ڴ�С
	SIZE size;
	size.cx = m_FrameList[m_nPos].GetWidth();
	size.cy = m_FrameList[m_nPos].GetHeight();
	POINT scr;
	scr.x = 0;
	scr.y = 0;
	if (m_iWidth == size.cx && m_iHeight == size.cy)
	{
		BOOL bRet = ::UpdateLayeredWindow(m_hWnd,   hScreendc,   &dst,   &size,   m_FrameList[m_nPos].GetCompatibleDC(),   &scr,	RGB(255,0,255),   &blend,   2);   

	}
	else
	{
		CMemdc dc;
		dc.Create(hScreendc, m_iWidth, m_iHeight);
		SetStretchBltMode(dc.GetCompatibleDC(), COLORONCOLOR);
		StretchBlt(dc.GetCompatibleDC(), 0, 0, m_iWidth, m_iHeight, m_FrameList[m_nPos].GetCompatibleDC(), 
			0, 0, size.cx, size.cy, SRCCOPY);
		size.cx = m_iWidth;
		size.cy = m_iHeight;
		BOOL bRet = ::UpdateLayeredWindow(m_hWnd,   hScreendc,   &dst,   &size,  dc.GetCompatibleDC(),   &scr,	RGB(255,0,255),   &blend,   2);   
		dc.Delete();
	}	
	::ReleaseDC(m_hWnd, hScreendc);
	hScreendc = NULL;

	/// ��ʾ����
	if (size.cx<=0 || size.cy<=0)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_SHOWNOACTIVATE);
	}
}
///���ö���λ��
void CWndAnimate::SetAnimalPos(int iX,int iY)
{
	 m_iX = iX ; 
	 m_iY = iY ; 
	return ; 
}

/**
* @brief ����������
*/
void CWndAnimate::OnPaint()
{
	return __super::OnPaint();
}