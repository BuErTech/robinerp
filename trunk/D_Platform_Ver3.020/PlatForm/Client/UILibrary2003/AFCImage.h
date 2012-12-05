#pragma once

#ifndef CAFCIMAGE_HEAD_FILE
#define CAFCIMAGE_HEAD_FILE

#include "PublicDefine.h"
#include "Vfw.h"
#include "AtlImage.h"

#include "GDIPlus_Image.h"

/*namespace AFC
{
	namespace AFCImageGoGoGo
	{*/
		/*******************************************************************************************************/

		//��Դ����
		enum AFCResourceType
		{
			AFC_NO_OBJECT,
			AFC_IMAGE,
			AFC_VIEW_IMAGE,
		};

		/*******************************************************************************************************/

		//�ӿ�˵��
		interface IAFC_GDI_Resource;

		//AFC ��Դ����ӿ�
		interface EXT_CLASS IAFC_GDI_Handle 
		{
			//��������
		public:
			//�Ƿ�򿪾��
			virtual bool IsOpenHandle()=0;
			//��ȡ����
			virtual AFCResourceType GetResourceKind()=0;
			//�򿪾��
			virtual bool OpenGDIHandle(IAFC_GDI_Resource * pGDIObject)=0;
			//�رվ��
			virtual bool CloseGDIHandle()=0;
		};

		//AFC GDI ��Դ�ӿ�
		interface EXT_CLASS IAFC_GDI_Resource
		{
			//��������
		protected:
			//��������
			virtual UINT LockResource()=0;
			//��������
			virtual UINT FreeResource()=0;

		public:
			//��ȡ����
			virtual AFCResourceType GetResourceKind()=0;
		};

		/*******************************************************************************************************/

		//��Ƶλͼ
		class EXT_CLASS CGameViewImage//CAFCViewImage
		{
			//��������
		protected:
			HANDLE								m_hFileMap;						//�ļ����
			HDRAWDIB							m_hDrawDib;						//��Ƶ���
			BITMAPFILEHEADER					* m_pImageHead;					//�ļ�ָ��
			BITMAPINFOHEADER					* m_pImageInfo;					//ͼ����Ϣ

			//��������
		public:
			//���캯��
			CGameViewImage();
			//��������
			virtual ~CGameViewImage();

			void SetWidth(){;};
			void SetHeight(){;};

			//���ܺ���
		public:
			//����λͼ
			bool Load(const TCHAR * szFileName);
			void LoadFromJpg(const TCHAR * szFileName){;};
			void LoadFromBmp(const TCHAR * szFileName){;};
			//����λͼ
			bool Destroy();
			//��ȡ���
			inline UINT GetWidth();
			//��ȡ�߶�
			inline UINT GetHeight();
			//�滭����
			inline bool BitBlt(CDC * pDesDC, UINT uXDes, UINT uYDes, UINT uDesWidth, UINT uDesHeight, UINT uXScr, UINT uYScr, UINT uScrWidth, UINT uScrHeight);
			void Save(){;};
		};

		/*******************************************************************************************************/

		//��˵��
		class CGameImageHelper;

		//AFC ͼ����Դ
		class EXT_CLASS CGameImage/*C-AFCImage*/ : public IAFC_GDI_Resource
		{
			friend class CGameImageHelper;

			//��������
		protected:
			bool								m_bAutoLock;					//�Զ�����
			UINT								m_uLock;						//��������
			UINT								m_uResourceID;					//��Դ ID
			CImage								m_Image;						//��ԴͼƬ
			CString								m_strFileName;					//�ļ�����
			HINSTANCE							m_hResourceDLL;					//��Դ���

			//��������
		public:
			//���캯��
			CGameImage(HINSTANCE hResourceDLL=NULL, UINT uResourceID=0, TCHAR * szFileName=NULL, bool bLock=false);
			//��������
			virtual ~CGameImage();

			//�ӿں���
		protected:
			//��������
			virtual UINT LockResource();
			//��������
			virtual UINT FreeResource();

			//�ӿں���
		public:
			//��ȡ����
			virtual AFCResourceType GetResourceKind() { return AFC_IMAGE; };

			//���ܺ���
		public:
			//���Ӷ���
			bool Attach(CGameImage * pImageObject, bool bLock);
			//�������
			bool Detach();
			//�Ƿ��
			inline bool IsNull();
			//���ü��ز���
			bool SetLoadInfo(TCHAR * szFileName, bool bLock);
			//���ü��ز���
			bool SetLoadInfo(HINSTANCE hResourceDLL, UINT uResourceID, bool bLock);

			//��������
		public:
			//��������
			inline operator HBITMAP () { return m_Image; };

			//��������
		public:
			//��ȡ��ɫ
			inline COLORREF GetPixel(int x, int y) { return m_Image.GetPixel(x,y); };
			//��ȡ���
			inline int GetWidth() {  return m_Image.IsNull()?0:m_Image.GetWidth();}
			//��ȡ�߶�
			inline int GetHeight() { return m_Image.IsNull()?0:m_Image.GetHeight(); }
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC, const POINT & pointDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC,const RECT & rectDest, const POINT & pointSrc, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP=SRCCOPY);
		};

		//AFC ͼ����Դ,��ȫ֧��PNG
		//JianGuankun 2012.3.15
		class EXT_CLASS CGameImageEx/*C-AFCImage*/ : public CGameImage
		{
			//��������
		private:
			CGDIPlus_Image m_ImagePlus;

			//�ӿں���
		protected:
			//��������
			virtual UINT LockResource();
			//��������
			virtual UINT FreeResource();

			//��������
		public:
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC, const POINT & pointDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL BitBlt(HDC hDestDC,const RECT & rectDest, const POINT & pointSrc, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline virtual BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP=SRCCOPY);
		};


		/*******************************************************************************************************/

		//AFC ��Դ���
		class EXT_CLASS CGameImageHelper/*C-AFCImageHandle*/ : public IAFC_GDI_Handle
		{
			//��������
		private:
			CGameImage							* m_pAFCImage;					//��Դָ��

			//��������
		public:
			//���캯��
			CGameImageHelper(IAFC_GDI_Resource * pGDIObject);
			//��������
			virtual ~CGameImageHelper();

			//�ӿں���
		public:
			//�Ƿ�򿪾��
			virtual bool IsOpenHandle() { return (m_pAFCImage!=NULL); };
			//��ȡ����
			virtual AFCResourceType GetResourceKind();
			//�򿪾��
			virtual bool OpenGDIHandle(IAFC_GDI_Resource * pGDIObject);
			//�رվ��
			virtual bool CloseGDIHandle();

			//��������
		public:
			//��ȡ��ɫ
			inline COLORREF GetPixel(int x, int y) { return m_pAFCImage->GetPixel(x,y); };
			//��ȡ���
			inline int GetWidth() { return m_pAFCImage->GetWidth(); }
			//��ȡ�߶�
			inline int GetHeight() { return m_pAFCImage->GetHeight(); }
			//��ͼ����
			inline BOOL BitBlt(HDC hDestDC, int xDest, int yDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL BitBlt(HDC hDestDC, const POINT & pointDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL BitBlt(HDC hDestDC,const RECT & rectDest, const POINT & pointSrc, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL StretchBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwROP=SRCCOPY);
			//��ͼ����
			inline BOOL StretchBlt(HDC hDestDC, const RECT& rectDest, const RECT& rectSrc, DWORD dwROP=SRCCOPY);

			//��������
		public:
			//��������
			inline CGameImageHelper & operator = (CGameImageHelper & ImageHandle);
			//��������
			inline operator CImage * () { return GetImage(); }
			//��������
			inline operator CBitmap * () { return GetBitmap(); }
			//��������
			inline operator HBITMAP ();
			//��ȡ����
			inline CImage * GetImage();
			//��ȡ����
			inline CBitmap * GetBitmap();
		};

		/*******************************************************************************************************/
	/*};
};*/

#endif