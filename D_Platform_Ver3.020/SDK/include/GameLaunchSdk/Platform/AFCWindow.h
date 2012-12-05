#pragma once

#ifndef AFCWINDOW_HEAD_FILE
#define AFCWINDOW_HEAD_FILE

#include "PublicDefine.h"
#include "AFCImage.h"

//using namespace AFC::AFCImageGoGoGo;

//��Ϣ����
#define WM_SPLIT_WND						WM_USER+10						//�������Ϣ

/*namespace AFC
{
	namespace AFCWindow
	{*/
		/*******************************************************************************************************/

		//����ռ���
		class EXT_CLASS CWorldEdit/*C-AFCTitleBar*/ : public CStatic
		{
			//��������
		protected:
			HICON							m_hIcon;						//ͼ��
			CGameImage						m_TitlePic;						//������ͼ

			//��������
		public:
			//���캯��
			CWorldEdit();
			//��������
			virtual ~CWorldEdit();
			//����ͼ��
			bool SetTitleIcon(HICON hIcon);

			DECLARE_MESSAGE_MAP()

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
		};

		/*******************************************************************************************************/

		//��Ϣ����
		class EXT_CLASS CNewMapEx/*C-AFCTipWnd*/ : public CWnd
		{
			//��������
		protected:
			CString							m_strMessage;							//��ʾ��Ϣ
			CSize							m_FixSize;								//���ʴ�С

			//�������� 
		public:
			//���캯��
			CNewMapEx() {}
			//��������
			virtual ~CNewMapEx() {}

			//���ܺ���
		public:
			//��������
			bool CreateTipWnd(CWnd * pParentWnd, UINT uWndID);
			//���ô�����Ϣ
			CSize SetTipText(const TCHAR * szTipText, bool bFixSize);
			//��ȡ�ʺϴ�С
			CSize & GetFixSize() { return m_FixSize; };

			DECLARE_MESSAGE_MAP()

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
		};

		/*******************************************************************************************************/

		//���������
		class CAFCSplitWnd : public CWnd
		{
			friend class CControlMessage;

			//�������� 
		protected:
			//���캯��
			CAFCSplitWnd() {}
			//��������
			virtual ~CAFCSplitWnd() {}

			DECLARE_MESSAGE_MAP()

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
		};

		//�������
		class EXT_CLASS CControlMessage/*C-AFCSplitBar*/ : public CWnd
		{
			//��������
		protected:
			bool						m_bHorSplit;						//�Ƿ�����
			CAFCSplitWnd				m_SplitWnd;							//���������
			int							m_iLessPos;							//��Сλ��
			int							m_iMaxPos;							//���λ��
			CGameImage					m_bkimage;							//����ͼƬ
			CGameImage					m_bkimagel;							//����ͼƬ
			CGameImage					m_bkimager;							//����ͼƬ
			//��������
		public:
			//���캯��
			CControlMessage();
			//��������
			virtual ~CControlMessage();
			//��ʼ�������
			bool InitSplitBar(UINT uLessPos, UINT uMaxPos, bool bHorSplit);
			void SetbkImage(TCHAR imagepath1[],TCHAR imagepath2[],TCHAR imagepath3[]);
			COLORREF					m_Color;			//������ɫ
			DECLARE_MESSAGE_MAP()

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
			//����ƶ���Ϣ
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			//��갴����Ϣ
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			//��갴����Ϣ
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			//λ����Ϣ 
			afx_msg void OnSize(UINT nType, int cx, int cy);
			//���ù����Ϣ
			afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
			//ʧȥ����
			afx_msg void OnKillFocus(CWnd * pNewWnd);
		};

		/*******************************************************************************************************/

		//����������
		class EXT_CLASS CHyperLink/*C-AFCHyperLink*/ : public CStatic
		{
			//��������
		protected:
			bool							m_bOverControl;					//�Ƿ�Խ��
			bool							m_bVisited;						//�Ƿ����
			bool							m_bUnderline;					//�Ƿ���
			bool							m_bAdjustToFit;					//�Ƿ����
			CFont							m_Font;							//��ʾ����
			HCURSOR							m_hLinkCursor;					//�����
			CString							m_strURL;						//���ӵ�ַ
			COLORREF						m_crLinkColour;					//������ɫ
			COLORREF						m_crVisitedColour;				//������ɫ
			COLORREF						m_crHoverColour;				//Խ����ɫ
			CToolTipCtrl					m_ToolTip;						//��ʾ�ؼ�

			//��������
		public:
			//���캯��
			CHyperLink();
			//��������
			virtual ~CHyperLink();

			//���ܺ���
		public:
			//���ó�������
			void SetURL(CString strURL);
			//��ȡ��������
			CString GetURL() const;
			//������ɫ
			void SetColours(COLORREF crLinkColour, COLORREF crVisitedColour, COLORREF crHoverColour = -1);
			//��ȡ��ɫ
			COLORREF GetLinkColour() const;
			//��ȡ��ɫ
			COLORREF GetVisitedColour() const;
			//��ȡ��ɫ
			COLORREF GetHoverColour() const;
			//�����Ƿ����		
			void SetVisited(bool bVisited=true);
			//��ȡ�Ƿ����		
			bool GetVisited() const;
			//���ù��
			void SetLinkCursor(HCURSOR hCursor);
			//��ȡ���
			HCURSOR GetLinkCursor() const;
			//����Ĭ�Ϲ��
			void SetDefaultCursor();
			//�����Ƿ����»���
			void SetUnderline(bool bUnderline=true);
			//��ȡ�Ƿ����»���
			bool GetUnderline() const;
			//�����Ƿ��Զ����ڴ�С
			void SetAutoSize(bool bAutoSize=true);
			//��ȡ�Ƿ��Զ����ڴ�С
			bool GetAutoSize() const;
			//ת����ҳ
			HINSTANCE GotoURL(const TCHAR * szURL, int iShowCmd);
			//�滭����
			//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
			
			afx_msg void OnPaint();
			//�ڲ�����
		protected:
			//�������
			void ReportError(int iError);
			//��ȡע�����ֵ
			long int GetRegKey(HKEY hRegKey, TCHAR * szSubKey, TCHAR * szRetData);
			//�ƶ�����
			void PositionWindow();
			//�������໯���� 
			virtual void PreSubclassWindow();
			//��Ϣ���ͺ���
			virtual BOOL PreTranslateMessage(MSG * pMsg);

			//��Ϣ����
		protected:
			//������ɫ��Ϣ
			afx_msg HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);
			//���ù����Ϣ
			afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
			//����ƶ���Ϣ
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			//������Ϣ
			afx_msg void OnClicked();

			DECLARE_MESSAGE_MAP()
		};

		/*******************************************************************************************************/

		//����Ի�����
		class EXT_CLASS CBitmapRgnWindow/*C-AFCRgnWindow*/ : public CWnd
		{
			//��������
		protected:
			CRgn						m_WindowRgn;					//��������
			CGameImage					m_BackBitmap;					//���ڱ���

			//��������
		public:
			//���캯��
			CBitmapRgnWindow();
			//��������
			virtual ~CBitmapRgnWindow();

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//���ܺ���
		public:
			//��ȡ����
			bool SafeGetClientRect(UINT uID, CRect & ClientRect);
			//��ȡ����
			bool SafeGetClientRect(CWnd * pControlWnd, CRect & ClientRect);
			//�ƶ�����
			bool SafeMoveWindow(UINT uID, int x, int y, int iWidth, int iHeight);
			//�ƶ�����
			bool SafeMoveWindow(CWnd * pControlWnd, int x, int y, int iWidth, int iHeight);

			//���ܺ���
		public:
			//����λͼ
			bool LoadDialogBitmap(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTransparentColor=RGB(0,0,0), COLORREF crTolerance=RGB(0,0,0));
			//����λͼ
			bool LoadDialogBitmap(TCHAR * szFileName, COLORREF crTransparentColor=RGB(0,0,0), COLORREF crTolerance=RGB(0,0,0));

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
			//����Ҽ�������Ϣ
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			//������Ϣ
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		};

		/*******************************************************************************************************/
	/*};
};*/

#endif