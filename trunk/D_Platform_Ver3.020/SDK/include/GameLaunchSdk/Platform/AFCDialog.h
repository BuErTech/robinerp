#pragma once

#ifndef AFCDIALOG_HEAD_FILE
#define AFCDIALOG_HEAD_FILE

#include "AFCImage.h"

//using namespace AFC::AFCImageGoGoGo;

/*namespace AFC
{
	namespace AFCDialog
	{*/
		/*******************************************************************************************************/

		//�����Ի�����
		class EXT_CLASS CGameFaceGo/*CAFCBaseDialog*/ : public CDialog
		{
			//��������
		public:
			//���캯��
			CGameFaceGo(UINT uTemplate, CWnd * pParent=NULL);
			//��������
			virtual ~CGameFaceGo();
			//��ʼ������
			virtual BOOL OnInitDialog();

			//���ܺ���
		public:
			//��ȡ����
			bool SafeGetClientRect(CWnd * pControlWnd, CRect & ClientRect);
			//��ȡ����
			bool SafeGetClientRect(UINT uID, CRect & ClientRect);
			//�ƶ�����
			bool SafeMoveWindow(UINT uID, int x, int y, int iWidth, int iHeight);
			//�ƶ�����
			bool SafeMoveWindow(CWnd * pControlWnd, int x, int y, int iWidth, int iHeight);

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//���ܺ���
		protected:
			//�滭����
			void DrawBackFace(CDC * pDC, int iXPos, int iYPos, int iWdith, int iHeight);

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
			//�ؼ���ɫ��Ϣ
			LRESULT OnControlColor(WPARAM wparam, LPARAM lparam);
		};

		/*******************************************************************************************************/

		//Ƥ���Ի�����
		class EXT_CLASS CLoadFileBmp/*C-AFCSkinDialog*/ : public CGameFaceGo
		{
			//��������
		private:
			bool						m_bTitleBar;					//�����־
			HICON						m_hIcon;						//ͼ����
			bool						m_bMaxSize;						//���
			BYTE						m_bNowDownButton;				//���°�ť 
			BYTE						m_bNowHotButton;				//���ڽ���
			CRect						m_rcNormalSize;					//��ͨλ��
			CString						m_strTitleText;					//��������
			BYTE						m_ButtonState[3];				//��ť״̬
			CRect						m_rcButton[3];					//��ťλ��
			
			//��������
		public:
			//���캯��
			CLoadFileBmp(UINT uTemplate, CWnd * pParent=NULL);
			//��������
			virtual ~CLoadFileBmp();
			//��ʼ������
			virtual BOOL OnInitDialog();

			//���ܺ���
		public:
			//��ʾ��ԭ���� 
			void ShowRestoreSize();
			//����ͼ��
			bool SetDialogIcon(HICON hIcon, BOOL bBigIcon);
			//���ñ���
			bool SetWindowTitle(const TCHAR * szTitle);
			//���ñ���
			bool EnableTitleBar(bool bEnableBar);
			//��ȡ����߶�
			UINT GetTitleHight();
			//��ʾ��󻯴��� 
			virtual void ShowMaxSize();

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//�ڲ�����
		protected:
			//�滭���
			void DrawFrame(CDC * pDC, CRect & ClientRect);
			//�滭������
			void DrawTitleBar(CDC * pDC, CRect & ClientRect);
			//�滭��ť
			void DrawCloseButton(CDC * pDC);
			//�滭��ť
			void DrawMaxRestoreButton(CDC * pDC);
			//�滭��ť
			void DrawMinButton(CDC * pDC);
			//���±���
			bool UpdateTitleBar();

			//��Ϣ����
		public:
			//�ػ�����
			afx_msg void OnPaint();
			//����ƶ���Ϣ
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			//����Ҽ�������Ϣ
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			//����Ҽ�������Ϣ
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			//���˫����Ϣ
			afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
			//λ�ñ仯 
			afx_msg void OnSize(UINT nType, int cx, int cy);
			//��������
			afx_msg void OnDestroy();
		};

		/*******************************************************************************************************/

		//����Ի�����
		class EXT_CLASS CCoolView/*C-AFCRgnDialog*/ : public CGameFaceGo
		{
			//��������
		protected:
			CRgn						m_DialogRgn;					//�Ի�������
			CGameImage					m_BackBitmap;					//�Ի��򱳾�ͼ

			//��������
		public:
			//���캯��
			CCoolView(UINT uTemplate, CWnd * pParent=NULL);
			//��������
			virtual ~CCoolView();
			//��ʼ������
			virtual BOOL OnInitDialog();

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//���ܺ���
		public:
			//����λͼ
			bool LoadDialogBitmap(TCHAR * szFileName, COLORREF crTransparentColor=RGB(0,0,0), COLORREF crTolerance=RGB(0,0,0));
			//����λͼ
			bool LoadDialogBitmap(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTransparentColor=RGB(0,0,0), COLORREF crTolerance=RGB(0,0,0));

			//��Ϣ����
		public:
			//����Ҽ�������Ϣ
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			//�ػ�����
			afx_msg void OnPaint();
		};

		/*******************************************************************************************************/
	/*};
};*/

#endif