#include "../stdafx.h"
#include "AFCImage.h"

//using namespace AFC;
//using namespace AFC::AFCImageGoGoGo;

/*******************************************************************************************************/

//���캯��
CGameViewImage::CGameViewImage()
{
	m_hFileMap=NULL;
	m_hDrawDib=NULL;
	m_pImageHead=NULL;
	m_pImageInfo=NULL;
}

//��������
CGameViewImage::~CGameViewImage()
{
	Destroy();
}

//����λͼ
bool CGameViewImage::Load(const TCHAR * szFileName)
{
	if (m_pImageHead==NULL)
	{
		try
		{
			//��ȡ�ļ�
			CFile ImageFile;
			if (ImageFile.Open(szFileName,CFile::modeRead)==FALSE) throw TEXT("OpenFileError");
			m_hFileMap=CreateFileMapping(ImageFile.m_hFile,NULL,PAGE_READONLY,0,(DWORD)ImageFile.GetLength(),NULL);
			if (m_hFileMap==NULL) throw TEXT("CreateFileMapping Error");
			m_pImageHead=(BITMAPFILEHEADER *)MapViewOfFile(m_hFileMap,FILE_MAP_READ,0,0,0);
			if (m_pImageHead==NULL) throw TEXT("MapViewOfFile Error");
			m_pImageInfo=(BITMAPINFOHEADER *)(m_pImageHead+1);
			m_hDrawDib=DrawDibOpen();
			if (m_hDrawDib==NULL) throw TEXT("DrawDibOpen Error");
			return true;
		}
		catch (...) {TRACE("%s\n",__FUNCTION__);}
		
		//������Դ
		Destroy();
	}
	return false;
}
//����λͼ
bool CGameViewImage::Destroy()
{
	if (m_pImageHead!=NULL) UnmapViewOfFile(m_pImageHead);
	if (m_hFileMap!=NULL) CloseHandle(m_hFileMap);
	if (m_hDrawDib!=NULL) DrawDibClose(m_hDrawDib);
	m_pImageHead=NULL;
	m_pImageInfo=NULL;
	return true;
}
//��ȡ���
UINT CGameViewImage::GetWidth()
{
	if (m_pImageInfo==NULL) return 0;
	return m_pImageInfo->biWidth;
}

//��ȡ�߶�
UINT CGameViewImage::GetHeight()
{
	if (m_pImageInfo==NULL) return 0;
	return m_pImageInfo->biHeight;
}
//�滭����
bool CGameViewImage::BitBlt(CDC * pDesDC, UINT uXDes, UINT uYDes, UINT uDesWidth, UINT uDesHeight, UINT uXScr, UINT uYScr, UINT uScrWidth, UINT uScrHeight)
{
	DrawDibDraw(m_hDrawDib,pDesDC->GetSafeHdc(),uXDes,uYDes,uDesWidth,uDesHeight,
		(LPBITMAPINFOHEADER)((BYTE *)m_pImageHead+sizeof(BITMAPFILEHEADER)),
		((BYTE *)m_pImageHead)+m_pImageHead->bfOffBits,uXScr,uYScr,uScrWidth,uScrHeight,0);
	return true;
}
/*******************************************************************************************************/

//���캯��
CGameImageHelper::CGameImageHelper(IAFC_GDI_Resource * pGDIObject)
{
	m_pAFCImage=NULL;
	OpenGDIHandle(pGDIObject);
}

//��������
CGameImageHelper::~CGameImageHelper()
{
	CloseGDIHandle();
}

//�򿪾��
bool CGameImageHelper::OpenGDIHandle(IAFC_GDI_Resource * pGDIObject)
{
	CloseGDIHandle();
	if ((pGDIObject!=NULL)&&(pGDIObject->GetResourceKind()==AFC_IMAGE))
	{
		m_pAFCImage=static_cast<CGameImage *>(pGDIObject);
		m_pAFCImage->LockResource();
		return true;
	}
	return false;
}

//�رվ��
bool CGameImageHelper::CloseGDIHandle()
{
	if (m_pAFCImage!=NULL) 
	{
		m_pAFCImage->FreeResource();
		return true;
	} 
	return false;
}

//��ȡ����
AFCResourceType CGameImageHelper::GetResourceKind()
{
	if (m_pAFCImage!=NULL) return m_pAFCImage->GetResourceKind();
	return AFC_NO_OBJECT;
}

//��ͼ����
BOOL CGameImageHelper::BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP)
{
	return m_pAFCImage->BitBlt(hDestDC,xDest,yDest,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::BitBlt(HDC hDestDC, const POINT & pointDest, DWORD dwROP)
{
	return m_pAFCImage->BitBlt(hDestDC,pointDest,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP)
{
	return m_pAFCImage->BitBlt(hDestDC,xDest,yDest,nDestWidth,nDestHeight,xSrc,ySrc,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::BitBlt(HDC hDestDC,const RECT & rectDest, const POINT & pointSrc, DWORD dwROP)
{
	return m_pAFCImage->BitBlt(hDestDC,rectDest,pointSrc,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP)
{
	return m_pAFCImage->StretchBlt(hDestDC,xDest,yDest,nDestWidth,nDestHeight,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP)
{
	return m_pAFCImage->StretchBlt(hDestDC,rectDest,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP)
{
	return m_pAFCImage->StretchBlt(hDestDC,xDest,yDest,nDestWidth,nDestHeight,xSrc,ySrc,nSrcWidth,nSrcHeight,dwROP);
}

//��ͼ����
BOOL CGameImageHelper::StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP)
{
	return m_pAFCImage->StretchBlt(hDestDC,rectDest,rectSrc,dwROP);
}

//��������
CGameImageHelper & CGameImageHelper::operator = (CGameImageHelper & ImageHandle)
{
	m_pAFCImage->FreeResource();
	m_pAFCImage=ImageHandle.m_pAFCImage;
	m_pAFCImage->LockResource();
	return (*this);
}

//��������
CGameImageHelper::operator HBITMAP ()
{
	if (IsOpenHandle()==false) return NULL;
	return (HBITMAP)m_pAFCImage->m_Image;
}

//��ȡ����
CImage * CGameImageHelper::GetImage()
{
	if (IsOpenHandle()==false) return NULL;
	return &m_pAFCImage->m_Image;
}

//��ȡ����
CBitmap * CGameImageHelper::GetBitmap()
{
	if (IsOpenHandle()==false) return NULL;
	return CBitmap::FromHandle(m_pAFCImage->m_Image);
}

/*******************************************************************************************************/

//���캯��
CGameImage::CGameImage(HINSTANCE hResourceDLL, UINT uResourceID, TCHAR * szFileName, bool bLock)
{
	m_uLock=0;
	m_bAutoLock=bLock;
	m_strFileName=szFileName;
	m_uResourceID=uResourceID;
	m_hResourceDLL=hResourceDLL;
	if (m_bAutoLock==true) LockResource();
}

//��������
CGameImage::~CGameImage()
{
}

//���Ӷ���
bool CGameImage::Attach(CGameImage * pImageObject, bool bLock)
{
	if (m_uLock==0)
	{
		m_bAutoLock=bLock;
		m_uResourceID=pImageObject->m_uResourceID;
		m_hResourceDLL=pImageObject->m_hResourceDLL;
		m_strFileName=pImageObject->m_strFileName;
		if (m_bAutoLock) LockResource();
		return true;
	}
	return false;
}

//�������
bool CGameImage::Detach()
{
	if (IsNull()==false)
	{
		m_uResourceID=0L;
		m_hResourceDLL=NULL;
		m_strFileName.Empty();
		if (m_bAutoLock==true) 
		{
			m_bAutoLock=false;
			FreeResource();
		}
		return true;
	}
	return false;
}

//��������
UINT CGameImage::LockResource()
{
	if (m_uLock==0)
	{
		if (m_hResourceDLL!=NULL) m_Image.LoadFromResource(m_hResourceDLL,m_uResourceID);
		else 
		{
#ifndef BZ_ZLIB
			m_Image.Load(m_strFileName);
#else
			CUnZipRes unZipRes;
			IStream * pStream=NULL;
			///DebugPrintf("LockResource() [m_strFileName =%s][%d��]",m_strFileName,__LINE__);
			char * pchar=m_strFileName.GetBuffer(m_strFileName.GetLength()+1);
			pStream=unZipRes.LoadFileResFromZip(pchar);
			if(pStream!=NULL)
			{
				m_Image.Load(pStream);
			}else
			{
				m_Image.Load("");
			}
			if(pStream!=NULL)
			{
				
				pStream->Release();
				pStream=NULL;
			}
			
#endif
		}

	}
	return ++m_uLock;
}

//��������
UINT CGameImage::FreeResource()
{
	if (m_uLock>0) m_uLock--;
	if (m_uLock==0) m_Image.Destroy();
	return m_uLock;
}

//�Ƿ��
bool CGameImage::IsNull()
{
	return ((m_hResourceDLL==NULL)&&(m_strFileName.IsEmpty()==true));
}

//���ü��ز���
bool CGameImage::SetLoadInfo(TCHAR * szFileName, bool bLock)
{
	Detach();
	m_bAutoLock=bLock;
	m_strFileName=szFileName;
	if (m_bAutoLock==true) 
		LockResource();
	return true;
}

//���ü��ز���
bool CGameImage::SetLoadInfo(HINSTANCE hResourceDLL, UINT uResourceID, bool bLock)
{
	Detach();
	m_bAutoLock=bLock;
	m_uResourceID=uResourceID;
	m_hResourceDLL=hResourceDLL;
	if (m_bAutoLock==true) LockResource();
	return true;
}

//��ͼ����
BOOL CGameImage::BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.BitBlt(hDestDC,xDest,yDest,dwROP);
}

//��ͼ����
BOOL CGameImage::BitBlt(HDC hDestDC, const POINT & pointDest, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.BitBlt(hDestDC,pointDest,dwROP);
}

//��ͼ����
BOOL CGameImage::BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.BitBlt(hDestDC,xDest,yDest,nDestWidth,nDestHeight,xSrc,ySrc,dwROP);
}

//��ͼ����
BOOL CGameImage::BitBlt(HDC hDestDC,const RECT & rectDest, const POINT & pointSrc, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.BitBlt(hDestDC,rectDest,pointSrc,dwROP);
}

//��ͼ����
BOOL CGameImage::StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.StretchBlt(hDestDC,xDest,yDest,nDestWidth,nDestHeight,dwROP);
}

//��ͼ����
BOOL CGameImage::StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.StretchBlt(hDestDC,rectDest,dwROP);
}

//��ͼ����
BOOL CGameImage::StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.StretchBlt(hDestDC,xDest,yDest,nDestWidth,nDestHeight,xSrc,ySrc,nSrcWidth,nSrcHeight,dwROP);
}

//��ͼ����
BOOL CGameImage::StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP)
{
	if(m_Image.IsNull())
		return FALSE;
	else
		return m_Image.StretchBlt(hDestDC,rectDest,rectSrc,dwROP);
}

/*******************************************************************************************************/
