#pragma once
#ifndef GIF_HEAD_H
#define GIF_HEAD_H
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

#include "IGif.h"

class CObjectFactory;

class EXT_CLASS CGif : public IGif
{
	friend class CObjectFactory;

protected:
	CGif(void);

	virtual ~CGif(void);

public:
	static HRESULT Initialize();

	static HRESULT Uninitialize();

public:

	virtual ULONG Release();

	virtual HRESULT LoadFromFile(LPCTSTR lpstrFileName) ;

	virtual HRESULT LoadFromRes(LPCTSTR lpstrResDirectory, LPCTSTR lpstrResName);

	virtual HRESULT Bitblt(DWORD dwFrameIndex, CDC *pDestDC, int ixDest, int iyDest);

	virtual HRESULT Bitblt(CDC *pDestDC, int ixDest, int iyDest);

	virtual HRESULT StretchBlt(DWORD dwFrameIndex, CDC *pDestDC, int ixDest, int iyDest, int iWidth, int iHeight) ;

	virtual DWORD GetImageWidth();

	virtual DWORD GetImageHeight();

	virtual DWORD GetFrameCount();

	virtual void Reset();

private:
	Gdiplus::Image		*m_pImage;
	UINT				m_uiFrameCount;
	UINT				*m_pFrameDelay;
	UINT				m_uiCurSelFrameID;
	DWORD				m_LastTimeSwitchFrame;

	static ULONG_PTR    m_GdiplusToken;
	static int			m_iInitializeCount;
};
#endif
//
//class CGif : public IGif
//{
//	friend class CObjectFactory;
//
//protected:
//	CGif(void);
//
//	virtual ~CGif(void);
//
//public:
//	static HRESULT Initialize();
//
//	static HRESULT Uninitialize();
//
//public:
//
//	virtual ULONG Release();
//
//	virtual HRESULT LoadFromFile(LPCSTR lpstrFileName) ;
//
//	virtual HRESULT LoadFromFile(LPCWSTR lpstrFileName) ;
//
//	virtual HRESULT LoadFromRes(LPCSTR lpstrResDirectory, LPCTSTR lpstrResName);
//
//	virtual HRESULT LoadFromRes(LPCWSTR lpstrResDirectory, LPCTSTR lpstrResName);
//
//	virtual HRESULT Bitblt(DWORD dwFrameIndex, CDC *pDestDC, int ixDest, int iyDest) ;
//
//	virtual HRESULT StretchBlt(DWORD dwFrameIndex, CDC *pDestDC, int ixDest, int iyDest, int iWidth, int iHeight) ;
//
//	virtual DWORD GetImageWidth();
//
//	virtual DWORD GetImageHeight();
//
//	virtual DWORD GetFrameCount();
//
//private:
//	Gdiplus::Image		*m_pImage;
//
//
//	static ULONG_PTR    m_GdiplusToken;
//	static int			m_iInitializeCount;
//};
