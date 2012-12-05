#pragma once

#ifndef CAFCBUTTON_HEAD_FILE
#define CAFCBUTTON_HEAD_FILE

#include "AFCImage.h"

//using namespace AFC::AFCImageGoGoGo;

		/*******************************************************************************************************/

		//������ť��
		class EXT_CLASS CTrueBitmapButton/*CTrueBitmapButton*/ : public CButton
		{
			//��������
		public:
			bool						m_bIsMouseMove;					//����־

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//��������
		public:
			//���캯��
			CTrueBitmapButton();

			//��������
		protected:
			//��������
			virtual ~CTrueBitmapButton();
			//���󸽼ӵ����д���
			virtual void PreSubclassWindow();

			//ȫ�ֺ���
		public:
			//���������
			static bool LoadButtonCursor(HCURSOR hCursor);
			//���������
			static bool LoadButtonCursor(HINSTANCE hInstance, UINT uCursorID);
			//��������
			static bool EnableSound(bool bEnable=true);

			//��Ϣ����
		protected:
			//����뿪��Ϣ
			LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
			//����ƶ���Ϣ
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			//�������Ϣ
			afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
		public:
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		public:
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		};

		/*******************************************************************************************************/

		//��ͨ��ť��
		class EXT_CLASS CNormalBitmapButton/*C-AFCButton*/ : public CTrueBitmapButton
		{
			//��������
		public:
			//���캯��
			CNormalBitmapButton();
			//��������
			virtual ~CNormalBitmapButton();
			//������ɫ
			static void SetColor(COLORREF m_ActiveColor=RGB(255,120,80), COLORREF m_ActiveTextColor=RGB(0,0,255), 
				COLORREF m_NormalTextColor=RGB(0,0,0), COLORREF m_SelectTextColor=RGB(0,0,255),
				COLORREF m_FrameColor=RGB(0,64,128), COLORREF m_FrameHeight=RGB(230,230,230),
				COLORREF m_FrameShadow=RGB(128,128,128), COLORREF m_NormalColorTop=RGB(255,255,255), 
				COLORREF m_NormalColorBottom=RGB(0,128,255));

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//��������
		protected:
			//����滭����
			virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
			//�滭����
			void DrawFace(CDC * pDC, COLORREF crTop, COLORREF crBottom, CRect & rc, CRect & ButtonRect);
			//�滭���
			void DrawFrame(CDC * pDC, COLORREF HeightLight, COLORREF ShadowLight, COLORREF FrameColor, CRect & rc);
		};

		/*******************************************************************************************************/

		//λͼ��ť�� ��λͼ��ʽ�����棬���£����㣬��������ֹ��
		class EXT_CLASS CNormalBitmapButtonEx/*C-AFCPicButton*/ : public CTrueBitmapButton
		{
			//��������
		protected:
			bool						m_bExpand;						//�Ƿ�����
			bool						m_bSetColor;					//������ɫ
			HICON						m_hIcon;						//��ťͼ��
			COLORREF					m_crTextColor;					//������ɫ
			CGameImage					m_ButtonBitmap;					//��ť��ͼ
	
			CFont   *  m_resFont;
			DWORD						m_wuiHandle;					//wui���
			UINT						m_uControlID;					//�ؼ�ID

            // PengJiLin, 2010-9-27, �����ַ���ʾ����
            bool    m_bUseTextInfo;     // �Ƿ���ʾ������ַ����������Щû�б����ַ��İ�ť��ʹ�ô˹�����ʾ�ַ�
            CString m_strTextInfo;      // ��Ҫ�ڰ�ť����ʾ���ַ���

			//��������
		public:
			//���캯��
			CNormalBitmapButtonEx();
			//��������
			virtual ~CNormalBitmapButtonEx();

			//���ܺ���
		public:
			//����WUI����
			void SetWuiParameter(DWORD wuiHandle,UINT uControlID);
			//����λ��
			bool FixButtonSize();
			//����ͼ��
			bool SetButtonIcon(HICON hIcon);
			//������ɫ
			bool SetTextColor(COLORREF crTextColor, bool bSetColor);
			//����λͼ
			bool LoadButtonBitmap(HINSTANCE hInstance, UINT uBitmapID, bool bExpand);
			//����λͼ
			bool LoadButtonBitmap(TCHAR * szFileName, bool bExpand);
			//����λͼ
			static bool SetStaticBitmap(TCHAR * szFileName);
			//����λͼ
			static bool SetStaticBitmap(HINSTANCE hInstance, UINT uBitmapID);

            // PengJiLin, 2010-9-27, �����Ƿ���ʾ������ַ�
            void EnableToShowText(CString& strInfo, bool bUseText);
            void EnableToShowText(TCHAR* strInfo, bool bUseText);
            void EnableToShowText(int iInfo, bool bUseText);

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//��������
		protected:
			//����滭����
			virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		};

		/*******************************************************************************************************/

#endif