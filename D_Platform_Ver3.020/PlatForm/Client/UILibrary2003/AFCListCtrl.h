#pragma once

#ifndef AFCLISTCTRL_HEAD_FILE
#define AFCLISTCTRL_HEAD_FILE

#include "AFCImage.h"
#include "ClientComStruct.h"
#include "PublicDefine.h"
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

// //��������
enum{
	IK_NAME,			//�û���
	IK_USERID,			//�û�ID
	IK_DESKNO,			//����
	IK_MONEY,			//���
	IK_WEALTH_LEVEL,	//�Ƹ�����
	IK_POINT,			//����
	IK_LEVEL,			//���ֵȼ�
	IK_CHARM,			//����
	IK_CHARM_LEVEL,		//�����ȼ�
	IK_SEX,				//�Ա�
	IK_ALL_COUNT,		//�ܾ���
	IK_WINCOUNT,		//ʤ��
	IK_LOSTCOUNT,		//���
	IK_MIDCOUNT,		//�;�
	IK_CUT_PERCENT		//������
};


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
			void SetBkImage(TCHAR path[],TCHAR path2[],COLORREF color,COLORREF tcolor, int type=0);
			//��Ϣ����
		protected:
			//�ػ�����
			afx_msg void OnPaint();
			//��������
			afx_msg BOOL OnEraseBkgnd(CDC * pDC);
		};

		/*******************************************************************************************************/

		#include <string>
		#include <map>	
		using namespace std;

		struct ColumnStruct{
			int  index;  ///< �е�λ��
			int  Width;  ///< �п�
			char ColumnName[MAX_PATH/2]; ///< ����
			char Value[32]; ///< ������ʾ��ֵ
		};

		typedef map<string,ColumnStruct> MapColumn;

		//�б���ͼ������
		class EXT_CLASS CHappyGoDlg/*C-AFCBaseListCtrl*/ : public CListCtrl
		{
			//��������
		protected:
			bool						m_bAsc;								///< ˳������
			UINT					  * m_pItemKind;						///< ��������
			GetOrderName			  * m_pGetOrderFun;						///< �ȼ�����
			bool						m_bInitStatic;						///< ��ʼ��־
			
			UINT						m_uSortPos[MAX_SORT_INDEX];			///< ��������
		public:
			CImageList			m_UserStateList;					///< ״̬�б�
			CBitmap						m_NewUserStateBmp;					///< ͼ���б�
			
			MapColumn                   m_MapColumn;                        ///< �����б���Ϣ
			
			//wushuqun 2009.6.18
			//�������,��ͬ�������ӵ�в�ͬ�����ͱ�ʶͼ��
			/*
			   0 :�ޱ�ʾ���
			   1 :���ӱ������
			   2 :VIP ���
			   3 :���ӱ���VIP���

			*/
			CImageList               m_UserTypeList;
			CBitmap					m_UserTypeBmp;		///< �û�����

			void   InitUserTypeImage();

			CToolTipCtrl  m_toolTip;    //��ʾ��Ϣ
			int   m_nSubItem,m_nItem;

			///////////////////////////////////////
			
			//������ʯ��ݱ�ʶ add by huangYuanSong 09.07.14
			CImageList               m_UserDiamondTypeList;
			CBitmap					m_UserDiamondTypeBmp;		///< �û����

			void   InitUserDiamondTypeImage();
			////--------------------------------

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
			virtual bool GetDrawColor(COLORREF & crTextColor, COLORREF & crBackColor, UserItemStruct * pUserItem, UINT uRow, bool bSelect);
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

			//����ƶ���Ϣ
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);

			virtual BOOL PreTranslateMessage(MSG * pMsg);
		public:
			virtual void LoadSkin(void);
			//
			void SetColor(COLORREF bkcr, COLORREF textcr, COLORREF focusbkcr, COLORREF focustextcr);
			//���ñ���ɫ
			void SetBKColor(COLORREF bkrgb){m_bkColor = bkrgb ;}
		};

		/*******************************************************************************************************/

	/*};
};*/

#endif