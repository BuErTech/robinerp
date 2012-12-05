#ifndef AFCEDIT_HEAD_FILE
#define AFCEDIT_HEAD_FILE

#include "dlgDiyChat.h"
//��Ϣ����
#define WM_RICH_EDIT_HIT					WM_USER+27						//����������Ϣ
#define WM_RELOAD_TALKINI					WM_USER+28						//�ض�talk.ini

/*namespace AFC
{
	namespace AFCEdit
	{*/
		/*******************************************************************************************************/

		//�ı�����ؼ���
		class EXT_CLASS CTrueEdit/*C-AFCEdit*/ : public CEdit
		{
			//��������
		protected:
			bool							m_bFocus;						//�Ƿ�õ�����

			//��������
		public:
			//���캯��
			CTrueEdit();
			//��������
			virtual ~CTrueEdit();

			//���ܺ���
		public:
			//������ɫ
			static void SetTextColor(COLORREF crFocus=RGB(255,255,255), COLORREF crNoFocue=RGB(255,255,255), COLORREF crDisble=RGB(255,0,0));
			//������ɫ
			static void SetBKColor(COLORREF crFocus=RGB(255,255,0), COLORREF crNoFocue=RGB(255,255,255), COLORREF crDisble=RGB(255,255,255));

		protected:
			DECLARE_MESSAGE_MAP()

			//��Ϣ���� 
		protected:
			//�滭�ؼ�
			afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
			//�õ�����
			afx_msg void OnSetFocus(CWnd * pOldWnd);
			//ʧȥ����
			afx_msg void OnKillFocus(CWnd * pNewWnd);
		};

		/*******************************************************************************************************/

		//���������
		class EXT_CLASS CTrueEditEx/*C-AFCPassWordEdit*/ : public CTrueEdit
		{
			//��������
		protected:
			bool							m_bProtected;					//�Ƿ�����õ�����

			//��������
		public:
			//���캯��
			CTrueEditEx();
			//��������
			virtual ~CTrueEditEx();
			//���ں���
			virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

			//���ܺ���
		public:
			//�Ƿ񱣻�״̬
			bool IsPortected();
			//���ñ���
			bool SetProtected(bool bProtected);
			//��ȡ����
			int GetEditPassWord(TCHAR * szBuffer, UINT uBufferLength);

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()
		};

		/*******************************************************************************************************/

		//ͼ�� OLE ��
		class CImageDataObject : IDataObject
		{
			//��������
		private:
			bool								m_bRelease;						//��Ҫ�ͷ�
			ULONG								m_ulRefCnt;						//���ü���
			STGMEDIUM 							m_Stgmed;						//�ṹ��Ϣ
			FORMATETC 							m_Format;						//�ṹ��Ϣ

			//�������� 
		public:
			//���캯��
			CImageDataObject();
			//��������
			~CImageDataObject();

			//���ܺ���
		public:
			//����ͼƬ
			void SetBitmap(HBITMAP hBitmap);
			//��ȡ����
			IOleObject * GetOleObject(IOleClientSite * pOleClientSite, IStorage * pStorage);
			//����ͼƬ
			static bool InsertBitmap(IRichEditOle * pRichEditOle, HBITMAP hBitmap);

			//�ӿں���
		public:
			//��ѯ�ӿ�
			STDMETHOD(QueryInterface)(REFIID iid, void * * ppvObject)
			{
				if (iid==IID_IUnknown||iid==IID_IDataObject)
				{
					*ppvObject=this;
					AddRef();
					return S_OK;
				}
				return E_NOINTERFACE;
			}
			//��������
			STDMETHOD_(ULONG, AddRef)(void)
			{
				m_ulRefCnt++;
				return m_ulRefCnt;
			}
			//�ͷŶ���
			STDMETHOD_(ULONG, Release)(void)
			{
				if (--m_ulRefCnt==0) delete this;
				return m_ulRefCnt;
			}
			//��������
			STDMETHOD(SetData)(FORMATETC * pformatetc, STGMEDIUM * pmedium, BOOL  fRelease) 
			{
				m_Format=*pformatetc;
				m_Stgmed=*pmedium;
				return S_OK;
			}
			//��ȡ����
			STDMETHOD(GetData)(FORMATETC * pformatetcIn, STGMEDIUM * pmedium) 
			{
				HANDLE hDst =::OleDuplicateData(m_Stgmed.hBitmap,CF_BITMAP,NULL);
				if (hDst==NULL) return E_HANDLE;
				pmedium->tymed=TYMED_GDI;
				pmedium->hBitmap=(HBITMAP)hDst;
				pmedium->pUnkForRelease=NULL;
				return S_OK;
			}
			STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC * *  ppenumFormatEtc ) {	return E_NOTIMPL; }
			STDMETHOD(DAdvise)(FORMATETC * pformatetc, DWORD advf, IAdviseSink * pAdvSink,DWORD * pdwConnection) { return E_NOTIMPL; }
			STDMETHOD(DUnadvise)(DWORD dwConnection) { return E_NOTIMPL; }
			STDMETHOD(EnumDAdvise)(IEnumSTATDATA * * ppenumAdvise) { return E_NOTIMPL; }
			STDMETHOD(GetDataHere)(FORMATETC * pformatetc, STGMEDIUM *  pmedium ) { return E_NOTIMPL;	}
			STDMETHOD(QueryGetData)(FORMATETC *  pformatetc) { return E_NOTIMPL; }
			STDMETHOD(GetCanonicalFormatEtc)(FORMATETC * pformatectIn, FORMATETC * pformatetcOut) { return E_NOTIMPL; }
		};

		/*******************************************************************************************************/

		//��Ϣ����
		class EXT_CLASS CNewMiniDlg/*C-AFCRichEdit*/ : public CRichEditCtrl
		{
			//��������
		protected:
			bool								m_bMenu;						//�Ƿ�ʹ�ò˵�
			bool								m_bAuto;						//�Ƿ��Զ�����
			HCURSOR								m_Cursor;						//���
			CString								m_FontName;						//��������
			IRichEditOle						* m_pRichEditOle;				//OLE ָ��
			bool								m_bCanInsert;
			int									m_GameOrHall;					//�����ͷ��䱣�治ͬ��ini�ļ���1����Ϸ���䣬0�Ǵ���

		public:
			CDlgDIYChat							m_DiyChat;						//�޸����쳣����Ի���
			//��������
		public:
			//���캯��
			CNewMiniDlg();
			//��������
			virtual ~CNewMiniDlg();

			//���ܺ��� 
		public:
			DWORD							m_bkColor;
			//��ʼ������
			bool Init();
			//�����ͷ��䱣�治ͬ��ini�ļ���1����Ϸ���䣬0�Ǵ���
			void IsGameMessage(){m_GameOrHall=1;};
			void IsHallMessage(){m_GameOrHall=0;};
			//�Ƿ��Զ�����
			bool IsAutoScroll() { return m_bAuto; };
			//�ص�����
			static DWORD CALLBACK SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG * pcb);

			//������Ϣ
		public:
			//����ͼƬ
			void InsertBitmap(HBITMAP bBitmap);
			//����dc��λͼ
			HBITMAP CopyDCToBitmap(HDC hSrc,int x1,int y1,int x2,int y2);
			//����λͼ
			HBITMAP CopyBitmap(HBITMAP   hSourceHbitmap);  
			//�����ַ�
			void InsertCharString(const TCHAR * szCharString, COLORREF crColor, bool bLink=false, UINT uSize=0, TCHAR * szFontName=NULL,bool bBold=false);

			//��Ϣ����
		public:
			//�Ҽ���Ϣ
			afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
			//���ù��
			afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
			//����
			afx_msg void OnCopy() { Copy(); }
			//ȫ��ѡ��
			afx_msg void OnSelectAll() { SetSel(0, -1); }
			//���
			afx_msg void OnClear() { ReplaceSel(TEXT("")); }
			//����
			afx_msg void OnAutoScroll() { m_bAuto=!m_bAuto; }
			//�����Ļ
			afx_msg void OnClearAll() { SetSel(0,GetWindowTextLength()); ReplaceSel(NULL); }
			//ֹͣ��ʾ��Ļ�ϵ�������Ϣ
			afx_msg void OnStopAll() { m_bCanInsert=!m_bCanInsert; }
			//����
			afx_msg void OnSave();
			//����������ɫ
			void OnSetColor();
			//�Զ�����������
			void OnDIYChat();
			//��������
			afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
			//��������
			afx_msg void OnDestroy();

			DECLARE_MESSAGE_MAP()
		};

		/*******************************************************************************************************/
	/*};
};*/

#endif