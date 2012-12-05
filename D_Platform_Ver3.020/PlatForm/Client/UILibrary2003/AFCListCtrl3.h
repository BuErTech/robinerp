#pragma once

#ifndef AFCLISTCTRL_HEAD_FILE
#define AFCLISTCTRL_HEAD_FILE

#include "StdAfx.h"
#include "AFCImage.h"
#include "ClientComStruct.h"

//using namespace AFC::AFCImageGoGoGo;

//����λ��
#define SK_ENEMY_USER						0								//������������
#define SK_NORMAL_USER						1								//�����������

#define SK_NORMAL_MEMBER					2								//��ͨ��Ա����
#define SK_HIGHT_MEMBER	                    3								//�߼���Ա����
#define SK_SPECIAL_MEMBER					4								//�����α�����
#define SK_FOREVER_MEMBER					5								//���û�Ա����
#define SK_SENIOR_MEMBER					6								//Ԫ���������

#define SK_FRIEND_USER						7								//������������
#define SK_MASTER_USER						8								//������������
#define SK_ME_USER							9								//�Լ���������
#define SK_ALWAYS_USER						10								//�̶���������

//��������
#define IK_USERID							1								//ID ����
#define IK_EXPERIENCE						2								//����ֵ
#define IK_CREDIT							3								//����ֵ
#define IK_ACCID							4								//ACC ����
#define IK_POINT							5								//����
#define IK_MOBILENUM						6								//�ֻ�����
#define IK_LOGOID							7								//ͷ�� ID ����
#define IK_WINCOUNT							8								//ʤ����Ŀ
#define IK_LOSTCOUNT						9								//����Ŀ
#define IK_CUTCOUNT							10								//ǿ����Ŀ
#define IK_MIDCOUNT							11								//�;���Ŀ
#define IK_NAME								12								//��¼��
#define IK_CLASSNAME						13								//��Ϸ����
#define IK_DESKNO							14								//��Ϸ����
#define IK_DESKSTATION						15								//����λ��
#define IK_USERSTATE						16								//�û�״̬
#define IK_MEMBER							17								//��Ա�ȼ�
#define IK_GAMEMASTER						18								//����ȼ�
#define IK_USERIP							19								//��¼IP��ַ
#define IK_ALL_COUNT						20								//�ܾ���
#define IK_CUT_PERCENT						21								//������
#define IK_NOTE								22								//��ע����
#define IK_MENPAI							23								//����
#define IK_CHENGHAO							24								//���

//��ϵ����
#define CM_NORMAL							0								//��ͨ��ϵ
#define CM_ENEMY							1								//���˹�ϵ
#define CM_FRIEND							2								//���ѹ�ϵ

//�궨��
#define USER_STATE_COUNT						6						//״̬��Ŀ
#define USER_STATE_WIDTH						27						//״̬���
#define	USER_STATE_HEIGHT						17						//״̬�߶�

//λ��˳��
#define MAX_SORT_INDEX						8								//�����������ֵ

/*namespace AFC
{
	namespace AFCListCtrl
	{*/
		/*******************************************************************************************************/

		//�б�ͷ
		class EXT_CLASS CComBodyClass/*C-AFCHeaderCtrl*/ : public CHeaderCtrl
		{
			//��������
		public:
			//���캯��
			CComBodyClass();
			//��������
			virtual ~CComBodyClass();

			DECLARE_MESSAGE_MAP()
		protected:
			int			m_Type;		//��������,����
			CGameImage m_bkimg;
			CGameImage m_bkimg2;
			COLORREF	m_color;
			COLORREF	m_textcolor;
		public:
			void SetBkImage(TCHAR path[],TCHAR path2[],COLORREF color,COLORREF tcolor);
			//
			void SetBkImage(TCHAR path[],TCHAR path2[],COLORREF color,COLORREF tcolor, int type);
			//��Ϣ����
		protected:
			//�ػ�����
			afx_msg void OnPaint();
			//��������
			afx_msg BOOL OnEraseBkgnd(CDC * pDC);
		};

		/*******************************************************************************************************/

		//�б���ͼ������
		class EXT_CLASS CHappyGoDlg/*C-AFCBaseListCtrl*/ : public CListCtrl
		{
			//��������
		protected:
			bool							m_bAsc;								//˳������
			UINT							* m_pItemKind;						//��������
			GetOrderName					* m_pGetOrderFun;					//�ȼ�����
			static bool						m_bInitStatic;						//��ʼ��־
			
			static UINT						m_uSortPos[MAX_SORT_INDEX];			//��������
		public:
			static CImageList				m_UserStateList;					//״̬�б�
			//�ؼ�����
		protected:
			CComBodyClass					m_ListHeader;						//�б�ͷ
			COLORREF						m_bkColor;
			COLORREF						m_TextColor;
			COLORREF						m_FocusbkColor;
			COLORREF						m_FocusTextColor;

			//��������
		public:
			//���캯��
			CHappyGoDlg(UINT * pItemKind);  
			//��������
			virtual ~CHappyGoDlg();
			//��ȡ�û�״̬ ID
			UINT GetStateImageIndex(UserItemStruct * pUserItem);
			//������������
			bool SetSortPos(UINT uSortPos[MAX_SORT_INDEX]);
			//��ȡ��������
			UINT * GetSortPos(UINT uSortPos[MAX_SORT_INDEX]);
			
			//��������
		protected:
			//��ȡ��ɫ
			bool GetDrawColor(COLORREF & crTextColor, COLORREF & crBackColor, UserItemStruct * pUserItem, UINT uRow, bool bSelect);
			//�ؼ���
			virtual void PreSubclassWindow();
			//�滭����
			virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
			//���к���
			static int CALLBACK SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

			//���غ���
		public:
			//��ʼ��
			virtual void InitListCtrl(GetOrderName * pOrderFunction, UINT uComType);
			//�����û�
			virtual bool AddGameUser(UserItemStruct * pUserItem)=0;
			//ɾ���û�
			virtual bool DeleteGameUser(UserItemStruct * pUserItem)=0;
			//����״̬
			virtual bool UpdateGameUser(UserItemStruct * pUserItem)=0;

			//��Ϣӳ��
		protected:
			DECLARE_MESSAGE_MAP()

			//��Ϣ����
		public:
			//��������
			afx_msg BOOL OnEraseBkgnd(CDC * pDC);
			//�������Ϣ
			afx_msg void OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);
		public:
			void LoadSkin(void);
			//
			void SetColor(COLORREF bkcr, COLORREF textcr, COLORREF focusbkcr, COLORREF focustextcr);
		};

		/*******************************************************************************************************/

	/*};
};*/

#endif