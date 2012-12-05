#include "StdAfx.h"
#include "AFCFunction.h"
#include <Wininet.h>		// MFC �׽�����չ

//wushuqun 2008.9.24
#include "ConfirmDlg.h"

//JianGuankun 2012.7.25
#include "DUIMessageWnd.h"

/*
//��ʾ��Ϣ��
UINT AFCMessage(UINT uResourceID, HINSTANCE hInstance, const TCHAR * const szTitle, UINT uType, CWnd * pParentWnd)
{
	CString strMessage;
	strMessage.LoadString(hInstance,uResourceID);
	return ::MessageBox(pParentWnd->GetSafeHwnd(),strMessage,szTitle,uType);
}

//��ʾ��Ϣ��
UINT AFCMessage(const TCHAR * const szText, const TCHAR * const szTitle, UINT uType, CWnd * pParentWnd)
{
	return ::MessageBox(pParentWnd->GetSafeHwnd(),szText,szTitle,uType);
}
*/
//�滭͸��ͼ
bool AFCStretchBlt(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight, CDC * pScrDC, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor)
{
	//�������
	COLORREF crTextColor=pDesDC->GetTextColor();

	//��������
	CDC ImageDC,MaskDC;
	CBitmap BmpImage,BmpMaskBmp;
	BmpImage.CreateCompatibleBitmap(pDesDC,iDesWidth,iDesHeight);
	BmpMaskBmp.CreateBitmap(iDesWidth,iDesHeight,1,1,NULL);
	ImageDC.CreateCompatibleDC(pDesDC);
	MaskDC.CreateCompatibleDC(pDesDC);
	CBitmap * pOldBmpImage=ImageDC.SelectObject(&BmpImage);
	CBitmap * pOldMaskBmp=MaskDC.SelectObject(&BmpMaskBmp);

	//�滭ͼ��
	if ((iDesWidth==iScrWidth)&&(iDesHeight==iScrHeight))
		ImageDC.BitBlt(0,0,iDesWidth,iDesHeight,pScrDC,iXScr,iYScr,SRCCOPY);
	else 
		ImageDC.StretchBlt(0,0,iDesWidth,iDesHeight,pScrDC,iXScr,iYScr,iScrWidth,iScrHeight,SRCCOPY);
	ImageDC.SetBkColor(crColor);
	//ImageDC.SetTextColor(RGB(0,0,0));
	MaskDC.BitBlt(0,0,iDesWidth,iDesHeight,&ImageDC,0,0,SRCCOPY);

	//����͸������Ϊ��ɫ���������򱣳ֲ����λͼ/�ĳ�255��
	ImageDC.SetBkColor(RGB(0,0,0));
	ImageDC.SetTextColor(RGB(255,255,255));
	ImageDC.BitBlt(0,0,iDesWidth,iDesHeight,&MaskDC,0,0,SRCAND);

	//�������ֱ�ɺ�ɫ
	//pDesDC->SetBkColor(RGB(255,255,255));
	//pDesDC->SetTextColor(RGB(0,0,0));
	pDesDC->BitBlt(iXDes,iYDes,iDesWidth,iDesHeight,&MaskDC,0,0,SRCAND);
	pDesDC->BitBlt(iXDes,iYDes,iDesWidth,iDesHeight,&ImageDC,0,0,SRCPAINT);

	//�������
	ImageDC.SelectObject(pOldBmpImage);
	ImageDC.DeleteDC();
	MaskDC.SelectObject(pOldMaskBmp);
	MaskDC.DeleteDC();
	BmpImage.DeleteObject();
	BmpMaskBmp.DeleteObject();

	//�ָ�����
	pDesDC->SetTextColor(crTextColor);
	return true;
}

//�滭͸��ͼ
bool AFCStretchImage(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight, HBITMAP hBitBmp, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDesDC);
	MemDC.SelectObject(hBitBmp);
	//return AFCStretchBlt(pDesDC,iXDes,iYDes,iDesWidth,iDesHeight,&MemDC,iXScr,iYScr,iScrWidth,iScrHeight,crColor);
	///{{Modified by zxd 20100709 �ͷŴ�����GDI����
	bool bRet = ::TransparentBlt(pDesDC->GetSafeHdc(),iXDes,iYDes,iDesWidth,iDesHeight,MemDC.GetSafeHdc(),iXScr,iYScr,iScrWidth,iScrHeight,crColor);
	MemDC.DeleteDC();
	return bRet;
	///Modified by zxd 20100709 �ͷŴ�����GDI����}}
}

//��������
bool AFCPlaySound(HINSTANCE hInstance, TCHAR * szResourceName)
{
	HRSRC hResour=FindResource(hInstance,szResourceName,TEXT("WAVE"));
	if (hResour==NULL) return false;
    HGLOBAL hGlobal=LoadResource(hInstance,hResour);
	if (hGlobal==NULL) return false;
    LPSTR lPLock=(LPSTR)LockResource(hGlobal);
	PlaySound(lPLock,hInstance,SND_ASYNC|SND_MEMORY|SND_NOSTOP|SND_NOWAIT);
	UnlockResource(hGlobal);
    FreeResource(hGlobal);
	return true;
}

//λͼת�������� ����Ҫ��д��
HRGN AFCBmpToRgn(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	if (hBmp==NULL) return NULL;

	HRGN hRgn=NULL;
	HDC hMemDC=::CreateCompatibleDC(NULL);
	if (hMemDC!=NULL)
	{
		BITMAP bm;
		GetObject(hBmp, sizeof(bm), &bm);
		VOID * pbits32=NULL; 
		BITMAPINFOHEADER RGB32BITSBITMAPINFO = {sizeof(BITMAPINFOHEADER),bm.bmWidth,bm.bmHeight,1,32,BI_RGB,0,0,0,0,0};
		HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
		if (hbm32)
		{
			HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);
			HDC hDC = CreateCompatibleDC(hMemDC);
			if (hDC)
			{
				BITMAP bm32;
				GetObject(hbm32,sizeof(bm32),&bm32);
				while (bm32.bmWidthBytes%4) bm32.bmWidthBytes++;
				HBITMAP holdBmp=(HBITMAP)SelectObject(hDC,hBmp);
				BitBlt(hMemDC,0,0,bm.bmWidth,bm.bmHeight,hDC,0,0,SRCCOPY);
				#define ALLOC_UNIT    100
				DWORD maxRects=ALLOC_UNIT;
				HANDLE hData=GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
				RGNDATA *pData=(RGNDATA *)GlobalLock(hData);
				pData->rdh.dwSize=sizeof(RGNDATAHEADER);
				pData->rdh.iType=RDH_RECTANGLES;
				pData->rdh.nCount=pData->rdh.nRgnSize=0;
				SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

				// Keep on hand highest and lowest values for the "transparent" pixels
				BYTE lr = GetRValue(cTransparentColor);
				BYTE lg = GetGValue(cTransparentColor);
				BYTE lb = GetBValue(cTransparentColor);
				BYTE hr = min(0xff, lr + GetRValue(cTolerance));
				BYTE hg = min(0xff, lg + GetGValue(cTolerance));
				BYTE hb = min(0xff, lb + GetBValue(cTolerance));

				// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
				BYTE *p32=(BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
				for (int y=0;y<bm.bmHeight;y++)
				{
					// Scan each bitmap pixel from left to right
					for (int x=0;x<bm.bmWidth;x++)
					{
						// Search for a continuous range of "non transparent pixels"
						int x0=x;
						LONG *p=(LONG *)p32+x;
						while (x<bm.bmWidth)
						{
							BYTE b=GetRValue(*p);
							if (b>=lr&&b<=hr)
							{
								b=GetGValue(*p);
								if (b>=lg&&b<=hg)
								{
									b = GetBValue(*p);
									if (b >= lb && b <= hb) break;
								}
							}
							p++;
							x++;
						}

						if (x>x0)
						{
							// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
							if (pData->rdh.nCount >= maxRects)
							{
								GlobalUnlock(hData);
								maxRects += 100;
								hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
								pData = (RGNDATA *)GlobalLock(hData);
							}
							RECT *pr = (RECT *)&pData->Buffer;
							SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
							if (x0 < pData->rdh.rcBound.left)
								pData->rdh.rcBound.left = x0;
							if (y < pData->rdh.rcBound.top)
								pData->rdh.rcBound.top = y;
							if (x > pData->rdh.rcBound.right)
								pData->rdh.rcBound.right = x;
							if (y+1 > pData->rdh.rcBound.bottom)
								pData->rdh.rcBound.bottom = y+1;
							pData->rdh.nCount++;

							// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
							// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
							if (pData->rdh.nCount == 2000)
							{
								HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
								if (hRgn)
								{
									CombineRgn(hRgn, hRgn, h, RGN_OR);
									DeleteObject(h);
								}
								else
									hRgn = h;
								pData->rdh.nCount = 0;
								SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
							}
						}
					}
					p32 -= bm32.bmWidthBytes;
				}
				HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
				if (hRgn)
				{
					CombineRgn(hRgn, hRgn, h, RGN_OR);
					DeleteObject(h);
				}
				else hRgn = h;

				// Clean up
				GlobalFree(hData);
				SelectObject(hDC, holdBmp);
				DeleteDC(hDC);
			}
			DeleteObject(SelectObject(hMemDC, holdBmp));
		}
		DeleteDC(hMemDC);
	}    

	return hRgn;
}
//bmpת��������
HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor)
{
	HRGN hRgn = NULL;
	HRGN hRgn1 = NULL;

	// Check for valid bitmap handle
	if ( hBitmap != NULL )
	{
		// Get bitmap object information
		BITMAP bitmap;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		DWORD dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
		int bitsPixel = bitmap.bmBitsPixel / 8;

		// Check bitmap color depth (only 24 or 32 bits per pixel allowed)
		if ( ( bitsPixel == 3 ) || ( bitsPixel == 4 ) )
		{
			// Get bitmap bits
			unsigned char* pBits = new unsigned char[dwSize];
			GetBitmapBits(hBitmap, dwSize, pBits);

			// Create region from bitmap
			unsigned char red, green, blue;
			for ( int y=0; y<bitmap.bmHeight; y++ )
			{
				for ( int x=0; x<bitmap.bmWidth; x++ )
				{
					// Get pixel color
					blue = pBits[y*bitmap.bmWidthBytes + bitsPixel*x];
					green = pBits[y*bitmap.bmWidthBytes + bitsPixel*x+1];
					red = pBits[y*bitmap.bmWidthBytes + bitsPixel*x+2];

					// Check transparent color
					if ( RGB(red,green,blue) != transparentColor )
					{
						// Combine regions
						if ( hRgn == NULL )
							hRgn = CreateRectRgn(x, y, x+1, y+1);
						else
						{
							// Delete temporary region
							if ( hRgn1 != NULL )
								DeleteObject(hRgn1);

							// Create temporary region
							hRgn1 = CreateRectRgn(x, y, x+1, y+1);

							// Combine regions
							CombineRgn(hRgn, hRgn, hRgn1, RGN_OR);
						}
					}
				}
			}

			// Free bitmap bits
			delete pBits;
		}
	}

	// Delete temporary region
	if ( hRgn1 != NULL )
		DeleteObject(hRgn1);

	return hRgn;
}
/*****************************************************
//ʵ��ͼƬ�������͸��
//destdc Ŀ��dc
//dx Ŀ��x����
//dy Ŀ��y����
//cx ���
//cy �߶�
//srcdc Դdc
//sx Դx����
//sy Դy����
//crTransparent Ҫ��͸������ɫ
//SourceConstantAlpha ������ɫ͸���� 0 ~ 255
����true��ʾ�ɹ� , false ��ʾʧ��
*****************************************************/
bool AFCAlphaBlend(HDC destdc, int dx, int dy, int cx, int cy, HDC srcdc, int sx, int sy, COLORREF crTransparent, int SourceConstantAlpha)
{
	HDC dc;
	dc = CreateCompatibleDC(destdc);
	HBITMAP bitmap;
	bitmap = CreateCompatibleBitmap(destdc,cx,cy);
	SelectObject(dc,bitmap);
	StretchBlt(dc,0,0,cx,cy,destdc,dx,dy,cx,cy,SRCCOPY);

	::TransparentBlt(dc,0,0,cx,cy,srcdc,sx,sy,cx,cy,crTransparent);

	BLENDFUNCTION bl;
	bl.BlendOp = AC_SRC_OVER;
	bl.BlendFlags = 0;
	bl.SourceConstantAlpha = SourceConstantAlpha;
	bl.AlphaFormat = 0; 
	BOOL result = AlphaBlend(destdc,dx,dy,cx,cy,dc,0,0,cx,cy,bl);
	DeleteObject(bitmap);
	DeleteDC(dc);
	return result;
}


//�����ͼ
void SaveScreen()
{
	HBITMAP hbitmap=CopyScreenToBitmap();
	TCHAR name[MAX_PATH];
	SYSTEMTIME st;
	GetLocalTime(&st);

    // PengJiLin, 2010-6-7, GetCurrentDirectory()����ȫ�����
    //::GetCurrentDirectory(MAX_PATH,name);
    lstrcpy(name, CBcfFile::GetAppPath());

	lstrcat(name,"\\��ͼ");
	::CreateDirectory(name,NULL);
	wsprintf(name,TEXT("%s\\%d-%d-%d-%d-%d-%d.bmp"),name,st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	SaveBitmapToFile(hbitmap,name);
}

//copy��
HBITMAP CopyScreenToBitmap()   
{   
	HDC	hScrDC,hMemDC;
	HBITMAP	hBitmap,hOldBitmap;
	int               nX,   nY,   nX2,   nY2; 
	int               nWidth,   nHeight;              
	int               xScrn,   yScrn;  
	hScrDC   =   CreateDC(TEXT("DISPLAY"),   NULL,   NULL,   NULL);
	hMemDC   =   CreateCompatibleDC(hScrDC);   

	xScrn   =   GetDeviceCaps(hScrDC,   HORZRES);   
	yScrn   =   GetDeviceCaps(hScrDC,   VERTRES);   
	nX   =   0;   
	nY   =   0; 
	nX2   =   xScrn;   
	nY2   =   yScrn;   
	nWidth   =   nX2   -   nX;   
	nHeight   =   nY2   -   nY;   

	hBitmap   =   CreateCompatibleBitmap(hScrDC,   nWidth,   nHeight);

	BitBlt(hMemDC,   0,   0,   nWidth,   nHeight,   
		hScrDC,   nX,   nY,   SRCCOPY);   

	DeleteDC(hScrDC);   
	DeleteDC(hMemDC); 
	return   hBitmap;   
}

//�����ͼ�ļ�
int  SaveBitmapToFile(HBITMAP   hBitmap  ,LPTSTR   lpFileName)   
{ 
	if(!hBitmap)
	{
		DeleteObject(hBitmap);
		return 0;
	}
	HDC                         hDC;                     
	int                         iBits;
	int						wBitCount; 
	DWORD          dwPaletteSize=0, dwBmBitsSize, 	dwDIBSize,   dwWritten;   
	BITMAP                     Bitmap;                   
	//λͼ���Խṹ   
	BITMAPFILEHEADER       bmfHdr;                   
	//λͼ�ļ�ͷ�ṹ   
	BITMAPINFOHEADER       bi;                           
	//λͼ��Ϣͷ�ṹ     
	LPBITMAPINFOHEADER   lpbi;                       
	//ָ��λͼ��Ϣͷ�ṹ   
	HANDLE                     fh,   hDib,   hPal,hOldPal=NULL;  
	hDC   =   CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);   
	iBits   =   GetDeviceCaps(hDC,BITSPIXEL)*GetDeviceCaps(hDC, PLANES);   
	DeleteDC(hDC);   
	if   (iBits<= 1)   
		wBitCount =  1;   
	else   if(iBits  <=  4)   
		wBitCount   =   4;   
	else   if(iBits  <=  8)   
		wBitCount   =   8;   
	else   if(iBits <=  24)   
		wBitCount   =   24;  
	else if(iBits <=  32) 
	{
		wBitCount   =   32; 
	}
	else
	{
		DeleteObject(hBitmap);
		::ReleaseDC(NULL,hDC);
		return 0;
	}
	if(wBitCount <=8)   
		dwPaletteSize   =   (1  <<  wBitCount) *sizeof(RGBQUAD);   
	//����λͼ��Ϣͷ�ṹ   
	GetObject(hBitmap,   sizeof(BITMAP),   (LPSTR)&Bitmap);   
	bi.biSize                         =   sizeof(BITMAPINFOHEADER);   
	bi.biWidth                       =   Bitmap.bmWidth;   
	bi.biHeight                     =   Bitmap.bmHeight; 
	if(Bitmap.bmWidth<=0||Bitmap.bmWidth>2000||Bitmap.bmHeight<=0||Bitmap.bmHeight>2000)
	{
		DeleteObject(hBitmap);
		::ReleaseDC(NULL,hDC);
		return 0;
	}
	bi.biPlanes                     =   1;   
	bi.biBitCount                   =   wBitCount;   
	bi.biCompression             =   BI_RGB;   
	bi.biSizeImage                 =   0;   
	bi.biXPelsPerMeter           =   0;   
	bi.biYPelsPerMeter           =   0;   
	bi.biClrUsed                   =   0;   
	bi.biClrImportant             =   0;   
	dwBmBitsSize   =   ((Bitmap.bmWidth  * wBitCount+31)/32)* 4 *Bitmap.bmHeight ;   
	//Ϊλͼ���ݷ����ڴ�   
	hDib     =   GlobalAlloc(GHND,dwBmBitsSize+   
		dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	if(!hDib)
	{
		DeleteObject(hBitmap);
		::ReleaseDC(NULL,hDC);
		return 0;
	}
	lpbi   =   (LPBITMAPINFOHEADER)GlobalLock(hDib);   
	*lpbi   =   bi;   
	//   �����ɫ��         
	hPal   =   GetStockObject(DEFAULT_PALETTE);   
	if   (hPal)   
	{   
		hDC     =   ::GetDC(NULL);   
		hOldPal   =   SelectPalette(hDC,   (HPALETTE)hPal,   FALSE);   
		RealizePalette(hDC);   
	}   
	//   ��ȡ�õ�ɫ�����µ�����ֵ   
	GetDIBits(hDC,   hBitmap,   0,   (UINT)   Bitmap.bmHeight,   
		(LPSTR)lpbi   +   sizeof(BITMAPINFOHEADER)   
		+dwPaletteSize,   
		(LPBITMAPINFO)lpbi,   DIB_RGB_COLORS);   
	//�ָ���ɫ��         
	if(hOldPal)   
	{   
		SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);   
		RealizePalette(hDC);   
		::ReleaseDC(NULL,   hDC);   
	}   
	//����λͼ�ļ�           
	fh   =   CreateFile(lpFileName, GENERIC_WRITE,0,  NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,   NULL);   
	if(fh   ==   INVALID_HANDLE_VALUE)   
	{
		GlobalUnlock(hDib);   
		GlobalFree(hDib);   
		DeleteObject(hBitmap);
		::ReleaseDC(NULL,hDC);
		return   FALSE;   
	}
	//   ����λͼ�ļ�ͷ   
	bmfHdr.bfType   =   0x4D42; 
	dwDIBSize		=   sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+  dwPaletteSize + dwBmBitsSize;       
	bmfHdr.bfSize   =   dwDIBSize;   
	bmfHdr.bfReserved1   =   0;   
	bmfHdr.bfReserved2   =   0;   
	bmfHdr.bfOffBits   =   (DWORD)sizeof (BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER) +  dwPaletteSize;   
	WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);   
	WriteFile(fh,  (LPSTR)lpbi,   dwDIBSize,&dwWritten,   NULL);   
	//���         
	GlobalUnlock(hDib);   
	GlobalFree(hDib); 
	DeleteObject(hBitmap);
	::ReleaseDC(NULL,hDC);
	CloseHandle(fh); 
	return 0;
}
//�����ַ�����������ȡ�ַ�
bool GetChar(TCHAR * src, TCHAR * dest, int count)
{
	int j=0;
	byte a,b;
	int i,cn=0,en=0;
	for(i=0; i<count && i<lstrlen(src); )
	{
		a=src[i];
		b=src[i+1];
		//�ж��Ƿ���
		if((a>=176&&a<=247&&b>=160&&b<=254)||//GB2312
			(((a>=129&&a<=255)||(a>=64&&a<=126))&&b>=161&&b<=254)||///GB5
			(a>=129&&a<=254&&b>=64&&b<=254))//GBK
		{
			i += 2;
			cn++;
			if(en+cn>4)
				break;
		}
		else
		{
			i++ ;
			en++;
			if(en+cn>count)
				break;
		}
	}
	memcpy(dest,src,i);
	dest[i]='\0';
	return true;
}

//ɾ��
void Dellog(int iRoomID,int iSmallRoomID)
{
	TCHAR szFileName	[50];
	wsprintf(szFileName,"%d-%d.txt",iRoomID,iSmallRoomID);
	try
	{
		::DeleteFile(szFileName);
	}
	catch(...)
	{
	TRACE("%s\n",__FUNCTION__);	return ;
	}
	return ;
}
void WriteStr(int iValue)
{
	TCHAR sz[200];
	wsprintf(sz,"iValue = %d",iValue);
	WriteStr(sz);
}

void WriteStr(long iValue)
{
	TCHAR sz[200];
	wsprintf(sz,"iValue = %ld",iValue);
	WriteStr(sz);
}
void WriteStr(DWORD iValue)
{
	TCHAR sz[200];
	wsprintf(sz,"iValue = %ld",iValue);
	WriteStr(sz);
}
//д������־
void WriteStr(TCHAR *str,int iRoomID,int iSmallRoomID)
{
	try
	{
		CFile file;
		TCHAR szFileName	[50];
		TCHAR szNewLine[]="\r\n";
		wsprintf(szFileName,"%d-%d.txt",iRoomID,iSmallRoomID);
		file.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);//�������ļ�
		file.SeekToEnd();
		file.Write(szNewLine,strlen(szNewLine));			//����	
		file.Write(str,strlen(str));						//д����Ϸ��Ϣ
		file.Close();
	}
	catch(CException *e)
	{		
	TRACE("%s\n",__FUNCTION__);	//AfxMessageBox("some exception!");
		e->Delete();
		return ; 
	}
}
//д������־
void  WriteError(TCHAR *pMessage ,int iRoomID,int iSmallRoomID )
{
	try
	{
		char FileName[50];
		TCHAR path[MAX_PATH];
		
        // PengJiLin, 2010-6-7, GetCurrentDirectory()����ȫ�����
        //::GetCurrentDirectory (MAX_PATH,path);
        lstrcpy(path, CBcfFile::GetAppPath());

		wsprintf(FileName,"%s\\Error%d-%d.txt\0",path,iRoomID,iSmallRoomID);//GetCurrentProcessId());
		CFile syslog;
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		syslog.SeekToEnd();
		syslog.Write(pMessage,strlen(pMessage));

		syslog.SeekToEnd();
		TCHAR nextLine[] = "\r\n";
		syslog.Write(nextLine,strlen(nextLine));

		syslog.Close();
	}
	catch(CException *e)
	{		
	TRACE("%s\n",__FUNCTION__);	//AfxMessageBox("some exception!");
		e->Delete();
		return ; 
	}
}


//���غ���
int InternetGetFile(TCHAR szUrl[], TCHAR szFileName[],CString &filename)
{
	CString title;
	CString name;//Դ�ļ���
	CString destname;//Ŀ���ļ���
	CString Fullpath;//Ŀ��ȫ·��
	CString path;	//Ŀ��ȫ·���������ļ���
	//	TCHAR szMsg[1000];
	//	DWORD err;
	//��ȡԴ�ļ���
	name=szUrl;
	int cur=name.ReverseFind('/');
	name=name.Mid(cur+1,name.GetLength()-1-cur);
	TCHAR g_CurDir[MAX_PATH+1];
	
    // PengJiLin, 2010-6-7, GetCurrentDirectory()����ȫ�����
    //::::GetCurrentDirectory(MAX_PATH,g_CurDir);
    lstrcpy(g_CurDir, CBcfFile::GetAppPath());

	if(szFileName)
	{//���ָ����Ŀ¼�ļ���,����Ŀ¼�ļ���
		destname=szFileName;
		if(destname.GetAt(0)=='\\')
			destname=destname.Mid(1,destname.GetLength()-1);
		//���ȫ·����
		Fullpath.Format(TEXT("%s\\%s"),g_CurDir,destname);
	}
	else
	{//û��ָ������Դ�ļ���
		TCHAR tempath[MAX_PATH+1];
		DWORD len=GetTempPath(MAX_PATH,tempath);
		if(len == 0 || len > MAX_PATH)
		{
			return -1;
		}
		if(len<=MAX_PATH)
		{
			tempath[len*sizeof(TCHAR)+1]='\0';
		}
		TCHAR tempname[MAX_PATH+1];
		if(!::GetTempFileName(tempath,TEXT("bzw"),0,tempname))
		{
			return -1;
		}
		Fullpath=tempname;
	}
	//��ȡĿ��ȫ·���������ļ���
	path=Fullpath;
	cur=path.ReverseFind('\\');
	destname=path;
	path=destname.Mid(cur+1,destname.GetLength()-1-cur);
	destname=Fullpath;
	if(filename)
		filename=destname;

	DWORD dwFlags;
	InternetGetConnectedState(&dwFlags, 0);
	TCHAR strAgent[64];
	memset(strAgent,0,sizeof(strAgent));
	wsprintf(strAgent, TEXT("Agent%ld"), timeGetTime());
	HINTERNET hOpen;
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
		hOpen = InternetOpen(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		hOpen = InternetOpen(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hOpen)
	{//��ʾ������Ϣ
		return -1;
	}

	DWORD dwSize;
	TCHAR   szHead[] = TEXT("Accept: */*\r\n\r\n");
	VOID* szTemp[16384];
	HINTERNET  hConnect;
	CFile file;

	if ( !(hConnect = InternetOpenUrl( hOpen, szUrl, szHead,
		lstrlen(szHead), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
	{//��ʾ������Ϣ
		InternetCloseHandle(hOpen);
		return -1;
	}
	bool CreateDir=false;
retry:
	if(file.Open(destname.GetBuffer(),CFile::modeWrite|CFile::modeCreate)==FALSE)
	{//��ʾ������Ϣ
		int err=GetLastError();
		if(!CreateDir)
		{//����Ŀ¼,Ȼ������
			::CreateDirectory(path.GetBuffer(),NULL);
			CreateDir = true;
			goto retry;
		}
		return -1;
	} 

	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;

	if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		(LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}
	memset(szTemp,0,sizeof(szTemp));
	do
	{
		if(false)
		{
			file.Close();
			file.Remove(destname.GetBuffer());
			InternetCloseHandle(hConnect);
			InternetCloseHandle(hOpen);
			return -1;
		}
		if (!InternetReadFile (hConnect, szTemp, 16384,  &dwSize))
		{//��ʾ������Ϣ
			file.Close();
			file.Remove(destname.GetBuffer());
			InternetCloseHandle(hConnect);
			InternetCloseHandle(hOpen);
			return -1;
		}
		if (dwSize==0)
			break;
		else
		{
			file.Write(szTemp,dwSize);
		}
	}while (TRUE);
	file.Close();
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hOpen);
	return 0;
}

//�쳣����
LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	static bool showex=false;
	if(!showex)
		showex=true;
	else
		return EXCEPTION_EXECUTE_HANDLER;
	TCHAR modulename[MAX_PATH],szError[1000],filename[MAX_PATH],*p;
	MEMORY_BASIC_INFORMATION mbinfo;
	VirtualQuery(pExceptionInfo->ExceptionRecord->ExceptionAddress,&mbinfo,sizeof(mbinfo));
	GetModuleFileName((HMODULE)mbinfo.AllocationBase,filename,MAX_PATH);
	p=strrchr(filename,'\\');
	p++;
	lstrcpy(modulename,p);
	try
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		char FileName[MAX_PATH];
		::CreateDirectory("Error",NULL);
		wsprintf(FileName,"Error\\errorlog-%d-%d.txt",st.wYear,st.wMonth);
		CFile syslog;
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		syslog.SeekToEnd();

		char szDate[40];
		wsprintf(szDate,"%d-%d-%d %d:%d:%d\r\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		syslog.Write(szDate,strlen(szDate));
		wsprintf(szError,"�쳣: ģ��:%s, ģ���ַ:0x%x, �쳣����:0x%x, ָ���ַ:0x%x",
			modulename,mbinfo.AllocationBase,pExceptionInfo->ExceptionRecord->ExceptionCode,pExceptionInfo->ExceptionRecord->ExceptionAddress);
		syslog.Write(szError,strlen(szError));
		char nextLine[] = "\r\n";
		syslog.Write(nextLine,strlen(nextLine));
		syslog.Close();
	}
	catch(CException *e)
	{TRACE("%s\n",__FUNCTION__);
		e->Delete();
	}
	wsprintf(szError,"����������������Ҫ�ر�,�Դ˸��������Ĳ����ʾ��Ǹ.\nģ��:%s,ģ���ַ:0x%x,�쳣����:0x%x,ָ���ַ:0x%x",
		modulename,mbinfo.AllocationBase,pExceptionInfo->ExceptionRecord->ExceptionCode,pExceptionInfo->ExceptionRecord->ExceptionAddress);
	
	//AFCMessageBox(szError,"BZWϵͳ");
	 DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"BZWϵͳ��ʾ",false,szError);

	return EXCEPTION_EXECUTE_HANDLER;
}



//wushuqun 2008.9.24
//�Զ�����Ϣ��
UINT AFCMessageBox(CString strInfo,CString strTitel,UINT style)
{
    ConfirmDlg confirmDlg(strInfo,strTitel,style);
    return confirmDlg.DoModal();
}

UINT AFCTimerMsgBox(int iTimer, CString strInfo,CString strTitel,UINT style)
{
	TimerDlg timerDlg(iTimer,strInfo,strTitel,style, NULL);
	return timerDlg.DoModal();
}

//��Ϣ��,DirectUI��

/// ��ʾ
/// @param pToCtrl ��ĳһ�ؼ�����ʾ
/// @param hParentWnd �����ھ��
/// @param style ��ʽ����WinAPI��MessageBox�ṩ����ʽһ��
/// @param lpTitle ��Ϣ��ı���
/// @param lpMainText ��Ϣ�����ı���֧���ı��ĸ�ʽ��
/// @return UINT ����ѯ�ʽ������WinAPI��MessageBox�ṩ�ķ���ֵһ��
/// @author JianGuankun,2012.7.25
///
EXT_CLASS UINT DUIMessageBox(HWND hParentWnd,UINT style,LPCTSTR lpTitle,bool bFormat,LPCTSTR lpMainText,...)
{
	// ���ǵ���xml�༭����ʹ��<>���Ų����㣬����ʹ��{}���Ŵ���
	// ֧�ֱ�ǩǶ�ף���<l><b>text</b></l>�������ǽ���Ƕ����Ӧ�ñ���ģ���<l><b>text</l></b>��
	// The string formatter supports a kind of "mini-html" that consists of various short tags:
	//
	//   Bold:             <b>text</b>
	//   Color:            <c #xxxxxx>text</c>  where x = RGB in hex
	//   Font:             <f x>text</f>        where x = font id
	//   Italic:           <i>text</i>
	//   Image:            <i x y z>            where x = image name and y = imagelist num and z(optional) = imagelist id
	//   Link:             <a x>text</a>        where x(optional) = link content, normal like app:notepad or http:www.xxx.com
	//   NewLine           <n>                  
	//   Paragraph:        <p x>text</p>        where x = extra pixels indent in p
	//   Raw Text:         <r>text</r>
	//   Selected:         <s>text</s>
	//   Underline:        <u>text</u>
	//   X Indent:         <x i>                where i = hor indent in pixels
	//   Y Indent:         <y i>                where i = ver indent in pixels 

	char szFormat[BUFSIZ];
	strcpy(szFormat,lpMainText);

	va_list pArg;
	va_start(pArg,lpMainText);
	char szMessage[BUFSIZ];

#ifdef WIN32
	_vsnprintf(szMessage,BUFSIZ - 1,szFormat,pArg);
#else
	vsnprintf(szMessage,BUFSIZ - 1,szFormat,pArg);
#endif

	va_end(pArg);

	char szHead[10] = {0};
	::memcpy(szHead,szMessage,8);

	if (::strcmp(szHead,"<format>") == 0)
	{
		bFormat = true;
		::memcpy(szMessage,szMessage + 8,strlen(szMessage) - 8);
		szMessage[strlen(szMessage) - 8] = '\0';
	}


	CMessageWndDu wndMsg;
	wndMsg.Create(hParentWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	wndMsg.CenterWindow();
	wndMsg.ShowWindow(SW_SHOW);
	wndMsg.SetTitle(lpTitle);
	wndMsg.SetMainText(szMessage,bFormat);
	wndMsg.SetStyle(style);
	wndMsg.ShowModal();

	return wndMsg.m_uRetFlag;
}


//ֻ��һ��OK��ť�����ҹ̶�����
EXT_CLASS UINT DUIOkeyMsgBox(HWND hParentWnd,bool bFormat,LPCTSTR lpMainText,...)
{
	// ���ǵ���xml�༭����ʹ��<>���Ų����㣬����ʹ��{}���Ŵ���
	// ֧�ֱ�ǩǶ�ף���<l><b>text</b></l>�������ǽ���Ƕ����Ӧ�ñ���ģ���<l><b>text</l></b>��
	// The string formatter supports a kind of "mini-html" that consists of various short tags:
	//
	//   Bold:             <b>text</b>
	//   Color:            <c #xxxxxx>text</c>  where x = RGB in hex
	//   Font:             <f x>text</f>        where x = font id
	//   Italic:           <i>text</i>
	//   Image:            <i x y z>            where x = image name and y = imagelist num and z(optional) = imagelist id
	//   Link:             <a x>text</a>        where x(optional) = link content, normal like app:notepad or http:www.xxx.com
	//   NewLine           <n>                  
	//   Paragraph:        <p x>text</p>        where x = extra pixels indent in p
	//   Raw Text:         <r>text</r>
	//   Selected:         <s>text</s>
	//   Underline:        <u>text</u>
	//   X Indent:         <x i>                where i = hor indent in pixels
	//   Y Indent:         <y i>                where i = ver indent in pixels 

	char szFormat[BUFSIZ];
	strcpy(szFormat,lpMainText);

	va_list pArg;
	va_start(pArg,lpMainText);
	char szMessage[BUFSIZ];

#ifdef WIN32
	_vsnprintf(szMessage,BUFSIZ - 1,szFormat,pArg);
#else
	vsnprintf(szMessage,BUFSIZ - 1,szFormat,pArg);
#endif

	va_end(pArg);

	char szHead[10] = {0};
	::memcpy(szHead,szMessage,8);

	if (::strcmp(szHead,"<format>") == 0)
	{
		bFormat = true;
		::memcpy(szMessage,szMessage + 8,strlen(szMessage) - 8);
		szMessage[strlen(szMessage) - 8] = '\0';
	}

	CMessageWndDu wndMsg;
	wndMsg.Create(hParentWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	wndMsg.CenterWindow();
	wndMsg.ShowWindow(SW_SHOW);
	wndMsg.SetTitle(_T("ϵͳ��ʾ"));
	wndMsg.SetMainText(szMessage,bFormat);
	wndMsg.SetStyle(MB_OK|MB_ICONINFORMATION);
	wndMsg.ShowModal();

	return wndMsg.m_uRetFlag;
}
